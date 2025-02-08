/* jh_mem.h - v0.01 - Jason Hilder

Heavily inspired and designed from:
* https://github.com/nothings/stb/
* https://github.com/gingerBill/gb/

This is a single header file that contains:
- allocators

References:
https://www.gingerbill.org/series/memory-allocation-strategies/

A implementation of the above with changes were I felt I needed them.

To use this library, do this in *one* C:
	#define JH_MEM_IMPLEMENTATION
	#include "jh_io.h"

NOTE:
This is intended to be used with the jh.h header.
And will require jh.h as it uses the typedefs defined in that file. 

*/

#ifndef JH_MEM_INCLUDED
#define JH_MEM_INCLUDED

#ifndef DEFAULT_ALIGNMENT
#define DEFAULT_ALIGNMENT (2*sizeof(void *))
#endif

typedef struct Arena Arena;
struct Arena {
	u8      *data;
	usize   data_len;
	usize   prev_offset;
	usize   curr_offset;
};

bool is_power_of_two(uptr x);
uptr align_forward(uptr ptr, usize align);
void *arena_alloc_align(Arena *a, usize size, usize align);
void *arena_alloc(Arena *a, usize size);

// -------------------------------------- 
// -------------------------------------- 
// implementation Below
// -------------------------------------- 


// -------------------------------------- 
// Arena Functions Start
// -------------------------------------- 

bool is_power_of_two(uintptr_t x) {
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
	uptr current_ptr = (uptr)a->data + (uptr)a->curr_offset;
	uptr offset = align_forward(current_ptr, align);

	// Change to relative offset
	offset -= (uptr)a->data;
}

void *arena_alloc(Arena *a, size_t size) {
	return arena_alloc_align(a, size, DEFAULT_ALIGNMENT);
}

// -------------------------------------- 
// Arena Functions END
// -------------------------------------- 

#endif
