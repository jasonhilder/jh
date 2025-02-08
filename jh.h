/* jh.h - v0.01 - Jason Hilder

Heavily inspired and designed from:
* https://github.com/nothings/stb/
* https://github.com/gingerBill/gb/

This is a single header file that contains:
- consistent type defintions 
- useful macros
- comparison functions
- array helpers

To use this library, do this in *one* C:
	#define JH_IMPLEMENTATION
	#include "jh.h"

*/

#ifndef JH_INCLUDED
#define JH_INCLUDED

typedef uint8_t   u8;
typedef uint16_t  u16;
typedef uint32_t  u32;
typedef uint64_t  u64;

typedef char      i8;
typedef int16_t   i32;
typedef int32_t   i32;
typedef float     f32;
typedef double    f64;
typedef char      byte;
typedef char16_t  c16;

typedef uintptr_t uptr;
typedef ptrdiff_t size;
typedef size_t    usize;

void jh_sort_int_array(int* array, size_t arrlen, const char* order);

// -------------------------------------- 
// -------------------------------------- 
// implementation Below
// -------------------------------------- 

#ifdef JH_H_IMPLEMENTATION

// -------------------------------------- 
// Macros:
// -------------------------------------- 

#define max(a,b)             \
({                           \
    __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b;       \
})

#define min(a,b)             \
({                           \
    __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a < _b ? _a : _b;       \
})

#define countof(a)    (size)(sizeof(a) / sizeof(*(a)))
#define lengthof(s)   (countof(s) - 1)
#define assert(c)  while (!(c)) __builtin_unreachable()

// -------------------------------------- 
// Comparison helpers:
// -------------------------------------- 

static inline int int_compare_asc(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

static inline int int_compare_desc(const void* a, const void* b) {
    return (*(int*)b - *(int*)a);
}

// -------------------------------------- 
// array helpers:
// -------------------------------------- 

/*
   Sorts an integer array in ascending or descending order.
   
   This function uses the `qsort` function to sort an array of integers either
   in ascending or descending order, based on the provided `order` argument.
   The comparison is done using the standard integer comparison logic.
   
   @param array Pointer to the integer array to be sorted.
   @param arrlen The length of the array.
   @param order A string that specifies the sorting order. It can be:
                - "asc" for ascending order
                - "desc" for descending order
                If an invalid value is provided, the function will print an error message.
   
   @note If the `order` parameter is not one of "asc" or "desc", an error message will be printed.
   @see qsort(3) for the underlying sorting algorithm used.
*/
void jh_sort_int_array(int* array, size_t arrlen, const char* order) {
    if (strcmp(order, "asc") == 0) {
        qsort(array, arrlen, sizeof(int), int_compare_asc);
    } else if (strcmp(order, "desc") == 0) {
        qsort(array, arrlen, sizeof(int), int_compare_desc);
    } else {
        printf("Invalid order: %s. Use 'asc' or 'desc'.\n", order);
    }
}

#endif /* JH_H_IMPLEMENTATION */
#endif /* JH_INCLUDED */
