/* jh_mem.h - v0.01 - Jason Hilder

Heavily inspired and designed from:
* https://github.com/nothings/stb/
* https://github.com/gingerBill/gb/

This is a single header file that contains:
- Arena allocator
- Stack allocator (todo)
- Pool allocator  (todo)

References:
https://www.gingerbill.org/series/memory-allocation-strategies/

A implementation of the above with changes were I felt I needed them.

To use this library, do this in *one* C:
	#define JH_MEM_IMPLEMENTATION
	#include "jh_mem.h"

NOTE:
This is intended to be used with the jh.h header.
And will require jh.h as it uses the typedefs defined in that file. 

*/

#ifndef JH_MEM_INCLUDED
#define JH_MEM_INCLUDED

#ifndef DEFAULT_ALIGNMENT
#define DEFAULT_ALIGNMENT (2*sizeof(void *))
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

typedef struct Arena Arena;
struct Arena {
	u8      *data;
	usize   data_len;
	usize   prev_offset;
	usize   curr_offset;
	// TODO
	// u8      growable;
};

typedef struct Temp_Arena_Memory Temp_Arena_Memory;
struct Temp_Arena_Memory {
	Arena *arena;
	size_t prev_offset;
	size_t curr_offset;
};

u8 is_power_of_two(uptr x);
uptr align_forward(uptr ptr, usize align);

void *arena_alloc_align(Arena *a, usize size, usize align);
void *arena_alloc(Arena *a, usize size);

void *arena_resize_align(Arena *a, void *old_memory, usize old_size, usize new_size, usize align);
void *arena_resize(Arena *a, void *old_memory, usize old_size, usize new_size);

// TODO
// arena_grow();

// Free the allocated arena
void arena_free(Arena *arena);

// "clears" the arena by placing the offset to the beggining.
// Allowing reuse of arena without having to free/alloc.
void arena_reset(Arena *arena);

// Initializes an arena instance. 
// On malloc fail it will return the arena with data as NULL 
// REQUIRED to arena.data after used
Arena arena_new(usize arena_init_size);

// Temp arena using the passed arena instance
Temp_Arena_Memory temp_arena_memory_begin(Arena *a);
// Clear the temp arena
void temp_arena_memory_end(Temp_Arena_Memory temp);

// -------------------------------------- 
// -------------------------------------- 
// implementation Below
// -------------------------------------- 


// -------------------------------------- 
// Arena Functions Start
// -------------------------------------- 

u8 is_power_of_two(uintptr_t x) {
	return (x & (x-1)) == 0;
}

uptr align_forward(uptr ptr, usize align) {
	uptr p, a, modulo;
	
	assert(is_power_of_two(align));

	p = ptr;
	a = (uptr)align;
	modulo = p & (a-1);

	if (modulo != 0) {
		// If 'p' address is not aligned. 
		// push the address to the next value which is aligned
		p += a - modulo;
	}

	return p;
}

void *arena_alloc_align(Arena *a, usize size, usize align) {
	// get the pointer
	uptr current_ptr = (uptr)a->data + (uptr)a->curr_offset;

	// align the offset if needed
	uptr offset = align_forward(current_ptr, align);

	// Change the pointer to relative offset number
	offset -= (uptr)a->data;

	if (offset+size <= a->data_len) {
		void *ptr = &a->data[offset];
		a->prev_offset = offset;
		a->curr_offset = offset+size;

		// Zero new memory
		memset(ptr, 0, size);
		return ptr;
	}

	// out of memory in arena, handle by doubling arena size;
	return NULL;
}

void *arena_alloc(Arena *a, usize size) {
	return arena_alloc_align(a, size, DEFAULT_ALIGNMENT);
}

void *arena_resize_align(Arena *a, void *old_memory, usize old_size, usize new_size, usize align) {
	u8 *old_mem = (u8 *)old_memory;

	assert(is_power_of_two(align));

	if (old_mem == NULL || old_size == 0) {
		return arena_alloc_align(a, new_size, align);
	} else if (a->data <= old_mem && old_mem < a->data + a->data_len) {

		if (a->data + a->prev_offset == old_mem) {
			a->curr_offset = a->prev_offset + new_size;
			if (new_size > old_size) {
				memset(&a->data[a->curr_offset], 0, new_size - old_size);
			}
			return old_memory;
		} else {
			void *new_memory = arena_alloc_align(a, new_size, align);
			usize copy_size = old_size < new_size ? old_size : new_size;

			// Copy old to new memory
			memmove(new_memory, old_memory, copy_size);
			return new_memory;
		}

	} else {
		assert(0 && "Memory is out of bounds of the buffer in this arena");
		return NULL;
	}
} 

void *arena_resize(Arena *a, void *old_memory, size_t old_size, size_t new_size) {
	return arena_resize_align(a, old_memory, old_size, new_size, DEFAULT_ALIGNMENT);
}

void arena_reset(Arena *arena) {
	arena->curr_offset = 0;
	arena->prev_offset = 0;
}

void arena_free(Arena* arena) {
    if (arena != NULL && arena->data != NULL) {
        free(arena->data);
        arena->data = NULL;
    }
}

Temp_Arena_Memory temp_arena_memory_begin(Arena *a) {
	Temp_Arena_Memory temp;
	temp.arena = a;
	temp.prev_offset = a->prev_offset;
	temp.curr_offset = a->curr_offset;
	return temp;
}

void temp_arena_memory_end(Temp_Arena_Memory temp) {
	temp.arena->prev_offset = temp.prev_offset;
	temp.arena->curr_offset = temp.curr_offset;
}

Arena arena_new(usize arena_init_size) {
	Arena arena = {0}; 
	
	arena.data = (u8 *)malloc(arena_init_size);
	if(arena.data == NULL) {
		return arena;
	}

	arena.data_len = arena_init_size;
	arena.curr_offset = 0;
	arena.prev_offset = 0;

	return arena;
}

// -------------------------------------- 
// Arena Functions END
// -------------------------------------- 

#endif
