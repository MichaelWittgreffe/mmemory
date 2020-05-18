#ifndef MALLOC_MMEMORY_H
#define MALLOC_MMEMORY_H

#include <sys/types.h>

// ALIGN is used to keep the MemHeader union at a constant size
typedef char ALIGN[16];

// MemHeader is a header for each block of memory allocated
typedef union MemHeader {
    struct {
        size_t size;
        unsigned isFree;
        union MemHeader* next;
    } data;
    ALIGN stub;
} MemHeader;

// malloc assigns and returns memory of the given size
void* malloc(size_t size);

// free releases memory back to the OS or marks for reuse
void free(void* block);

// calloc allocates for an array of 'num' elements of 'nsize' bytes, memory all set to 0's
void* calloc(size_t num, size_t nsize);

// realloc changes the size of the given memory block to the size given
void* realloc(void* block, size_t size);

// getFreeBlock returns a free memory block that has been already allocated, if one is avalible
MemHeader* getFreeBlock(size_t size);

#endif // MALLOC_MMEMORY_H