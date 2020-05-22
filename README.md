# MMemory
Small program to build a memory allocator in C. Currently implemented are malloc, free, realloc & calloc. 

The allocation/reuse is pretty simple and no real optimization is applied to these (for example, when finding a block of memory that is free for re-use, the first block we can fit into is returned rather than selecting the most appropriate block), so please do not use this for anything serious. This is just me playing around.