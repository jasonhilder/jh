/* jh.h - v0.01 - Jason Hilder

Heavily inspired and designed from:
* https://github.com/nothings/stb/
* https://github.com/gingerBill/gb/

This is a single header file with things I reach for often

To use this library, do this in *one* C or C++ file:
	#define JH_IMPLEMENTATION
	#include "jh.h"

*/

#ifndef JH_INCLUDE_JH_H
#define JH_INCLUDE_JH_H

int jh_read_file_to_buffer(char* file_path, char** out_buffer, int add_terminator);
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
// FILE I/O helpers:
// -------------------------------------- 

// Takes file path and a buffer which will get reallocated to the full file size. 
// Note: buffer still needs to be manually free'd
int hf_read_file_to_buffer(char* file_path, char** out_buffer, int add_terminator)
{
    FILE *file = fopen(file_path, "rb");
    if(file == NULL)
    {
        perror("Failed to open file.");
        return -1;
    }

    fseek(file, 0L, SEEK_END);
    long file_size = ftell(file);
    if (file_size == -1)
    {
        perror("Failed to determine file size");
        fclose(file);
        return -1;
    }
    rewind(file);

    // Reallocate memory for the buffer, check if realloc is successful
    // Realloc with extra space if terminator is needed
    char *buffer = realloc(*out_buffer, file_size + (add_terminator ? 1 : 0));
    if (buffer == NULL)
    {
        perror("Memory allocation failed");
        fclose(file);
        return -1;
    }

    // Update the pointer to the newly allocated buffer
    *out_buffer = buffer; 

    size_t bytes_read = fread(*out_buffer, 1, file_size, file);
    if (bytes_read != file_size)
    {
        perror("Error reading file");
        fclose(file);
        return -1;
    }

    // Add null terminator if needed
    if (add_terminator == 1)
    {
        (*out_buffer)[file_size] = '\0';
    }

    fclose(file);
    return 0;
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
#endif /* JH_INCLUDE_JH_H */
