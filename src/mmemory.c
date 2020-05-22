#include "mmemory.h"

#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

MemHeader* globalHead = NULL;
MemHeader* globalTail = NULL;
pthread_mutex_t globalMemLock;

void* malloc(size_t size) {
    if (!size) {
        return NULL;
    }

    pthread_mutex_lock(&globalMemLock);
    MemHeader* header = getFreeBlock(size); // reuse free'd memory if we can

    if (header) {
        header->data.isFree = false;
        pthread_mutex_unlock(&globalMemLock);
        return (void*)(header + 1);
    }

    void* block = sbrk(sizeof(MemHeader) + size);

    if (block == (void*)-1) {
        pthread_mutex_unlock(&globalMemLock);
        return NULL;
    }

    header = block;
    header->data.size = size;
    header->data.isFree = false;
    header->data.next = NULL;

    if (!globalHead) {
        globalHead = header;
    }

    if (globalTail) {
        globalTail->data.next = header;
    }

    globalTail = header;
    pthread_mutex_unlock(&globalMemLock);
    return (void*)(header + 1);
}

MemHeader* getFreeBlock(size_t size) {
    MemHeader* current = globalHead;

    while (current) {
        if (current->data.isFree && current->data.size >= size) {
            return current;
        }
        current = current->data.next;
    }

    return NULL;
}

void free(void* block) {
    if (!block) {
        return;
    }

    pthread_mutex_lock(&globalMemLock);
    MemHeader* header = (MemHeader*)block - 1;
    void* programBreak = sbrk(0);

    // if we're free'ing the block at the top of the heap, we can release to the OS
    if (((char*)block + header->data.size) == programBreak) {
        if (globalHead == globalTail) {
            globalHead = globalTail = NULL;
        } else {
            MemHeader* tmp = globalHead;

            while (tmp) {
                if (tmp->data.next == globalTail) {
                    tmp->data.next = NULL;
                    globalTail = tmp;
                }
                tmp = tmp->data.next;
            }
        }
        sbrk(0 - sizeof(MemHeader) - header->data.size);
        pthread_mutex_unlock(&globalMemLock);
        return;
    }

    // otherwise just mark the memory as avalible for re-use
    header->data.isFree = true;
    pthread_mutex_unlock(&globalMemLock);
}

void* calloc(size_t num, size_t nsize) {
    if (!num || !nsize) {
        return NULL;
    }

    // check multiplication overflow for size
    size_t size = num * nsize;
    if (nsize != (size / num)) {
        return NULL;
    }

    void* block = malloc(size);
    if (!block) {
        return NULL;
    }

    memset(block, 0, size);
    return block;
}

void* realloc(void* block, size_t size) {
    // invalid block/size, just treat as malloc
    if (!block || !size) {
        return malloc(size);
    }

    // given block size is more than incoming allocation, just return the block
    MemHeader* header = (MemHeader*)block - 1;
    if (header->data.size >= size) {
        return block;
    }

    // assign larger block
    void* newBlock = malloc(size);
    if (newBlock) {
        memcpy(newBlock, block, header->data.size);
        free(block);
    }
    return newBlock;
}