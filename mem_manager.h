#ifndef MEM_MANAGER_H
#define MEM_MANAGER_H

#include <stddef.h>  // for size_t

// Initialize the memory pool
int mem_init(void *pool, size_t size);

// Allocate memory from the pool
void *mem_alloc(size_t size);

// Free memory previously allocated
void mem_free(void *ptr);

// Copy contents from one block to another
void *mem_copy(void *dest, const void *src, size_t size);

// Search for a specific byte in a block of memory
void *mem_search_byte(void *block_ptr, size_t block_size, unsigned char byte);

// Search for a pattern (sequence of bytes) in a block of memory
void *mem_search_pattern(void *block_ptr, size_t block_size, const unsigned char *pattern, size_t pattern_size);

// Defragment memory by merging free blocks and shifting used blocks
void mem_defrag(void);

#endif // MEM_MANAGER_H
