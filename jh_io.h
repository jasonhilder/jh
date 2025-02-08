/* jh_io.h - v0.01 - Jason Hilder

Heavily inspired and designed from:
* https://github.com/nothings/stb/
* https://github.com/gingerBill/gb/

This is a single header file that contains:
- useful/common I/O helpers

To use this library, do this in *one* C:
	#define JH_IO_IMPLEMENTATION
	#include "jh_io.h"
*/

#ifndef JH_IO_INCLUDED
#define JH_IO_INCLUDED

int jh_read_file_to_buffer(char* file_path, char** out_buffer, int add_terminator);

// -------------------------------------- 
// -------------------------------------- 
// implementation Below
// -------------------------------------- 

#ifdef JH_IO_IMPLEMENTATION

// -------------------------------------- 
// FILE I/O helpers:
// -------------------------------------- 

// Takes file path and a buffer which will get reallocated to the full file size. 
// Note: buffer still needs to be manually free'd
int jh_read_file_to_buffer(char* file_path, char** out_buffer, int add_terminator)
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


#endif
#endif
