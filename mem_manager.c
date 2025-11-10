#include "mem_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// Define the memory block structure
typedef struct Block {
    size_t size;           // Size of the block in bytes
    struct Block *next;    // Pointer to the next block
    int free;              // Flag indicating if the block is free (1 = free, 0 = used)
} Block;

static void *memory_pool = NULL;  // The starting address of the memory pool
static size_t pool_size = 0;      // Total size of the memory pool
static Block *free_list = NULL;   // Head of the free list

// Initialize the memory pool
int mem_init(void *pool, size_t size) {
    if (!pool || size == 0) {
        return -1;  // Invalid input
    }

    memory_pool = pool;
    pool_size = size;

    // Initialize the free list with the entire memory pool
    free_list = (Block *)memory_pool;
    free_list->size = size - sizeof(Block);  // Subtract the size of the Block structure
    free_list->next = NULL;
    free_list->free = 1;

    return 0;  // Success
}

// Allocate memory from the pool
void *mem_alloc(size_t size) {
    if (size == 0 || free_list == NULL) {
        return NULL;  // Invalid request
    }

    Block *current = free_list;
    Block *prev = NULL;

    // Traverse the free list to find a suitable block
    while (current != NULL) {
        if (current->free && current->size >= size) {
            // If we found a suitable free block, split it if needed
            if (current->size > size + sizeof(Block)) {
                // Create a new block after the allocated block
                Block *new_block = (Block *)((uintptr_t)current + sizeof(Block) + size);
                new_block->size = current->size - size - sizeof(Block);
                new_block->free = 1;
                new_block->next = current->next;

                // Update the current block to reflect the allocated size
                current->size = size;
                current->next = new_block;
            }

            // Mark the block as used
            current->free = 0;

            return (void *)((uintptr_t)current + sizeof(Block));  // Return pointer to memory after Block header
        }

        prev = current;
        current = current->next;
    }

    return NULL;  // No suitable block found
}

// Free memory previously allocated
void mem_free(void *ptr) {
    if (!ptr) {
        return;  // Invalid pointer
    }

    // Find the block header
    Block *block = (Block *)((uintptr_t)ptr - sizeof(Block));

    // Mark the block as free
    block->free = 1;

    // Merge adjacent free blocks
    Block *current = free_list;
    while (current != NULL) {
        if (current->free && current->next != NULL && (uintptr_t)current + sizeof(Block) + current->size == (uintptr_t)current->next) {
            current->size += sizeof(Block) + current->next->size;  // Merge the current and next block
            current->next = current->next->next;  // Skip the next block
        } else {
            current = current->next;
        }
    }
}

// Copy contents from one block to another
void *mem_copy(void *dest, const void *src, size_t size) {
    if (!dest || !src || size == 0) {
        return NULL;  // Invalid input
    }

    return memcpy(dest, src, size);
}

// Search for a specific byte in a block of memory
void *mem_search_byte(void *block_ptr, size_t block_size, unsigned char byte) {
    if (!block_ptr || block_size == 0) {
        return NULL;  // Invalid input
    }

    unsigned char *byte_ptr = (unsigned char *)block_ptr;

    for (size_t i = 0; i < block_size; i++) {
        if (byte_ptr[i] == byte) {
            return &byte_ptr[i];  // Return the pointer to the found byte
        }
    }

    return NULL;  // Byte not found
}

// Search for a pattern (sequence of bytes) in a block of memory
void *mem_search_pattern(void *block_ptr, size_t block_size, const unsigned char *pattern, size_t pattern_size) {
    if (!block_ptr || block_size == 0 || !pattern || pattern_size == 0) {
        return NULL;  // Invalid input
    }

    unsigned char *byte_ptr = (unsigned char *)block_ptr;

    for (size_t i = 0; i <= block_size - pattern_size; i++) {
        if (memcmp(&byte_ptr[i], pattern, pattern_size) == 0) {
            return &byte_ptr[i];  // Return the pointer to the found pattern
        }
    }

    return NULL;  // Pattern not found
}

// Defragment memory by merging free blocks and shifting used blocks
void mem_defrag(void) {
    Block *current = free_list;
    Block *prev = NULL;

    // Merge free blocks
    while (current != NULL) {
        if (current->free && current->next != NULL && (uintptr_t)current + sizeof(Block) + current->size == (uintptr_t)current->next) {
            // Merge current block with the next one
            current->size += sizeof(Block) + current->next->size;
            current->next = current->next->next;  // Skip the next block
        } else {
            prev = current;
            current = current->next;
        }
    }

    // Move used blocks towards the beginning of the memory pool
    unsigned char *pool_start = (unsigned char *)memory_pool;
    unsigned char *write_ptr = pool_start;
    Block *block = (Block *)pool_start;

    while (block != NULL) {
        if (block->free == 0) {
            // Move the block contents
            if ((unsigned char *)block != write_ptr) {
                memmove(write_ptr, (unsigned char *)block + sizeof(Block), block->size);
            }

            // Update the block pointer to the new location
            block->next = (Block *)(write_ptr + block->size);
            write_ptr += block->size;
        }
        block = block->next;
    }

    // Update the free list after defrag
    free_list = (Block *)pool_start;
}
