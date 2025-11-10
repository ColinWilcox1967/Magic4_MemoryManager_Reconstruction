#include <stdio.h>
#include "mem_manager.h"

int main() {
    size_t pool_size = 1024;  // 1 KB memory pool
    void *pool = malloc(pool_size);  // Allocate a large memory block for the pool

    if (mem_init(pool, pool_size) != 0) {
        printf("Memory initialization failed!\n");
        return -1;
    }

    // Allocate memory
    void *ptr1 = mem_alloc(100);
    void *ptr2 = mem_alloc(200);
    void *ptr3 = mem_alloc(50);
    void *ptr4 = mem_alloc(150);

    printf("Allocated blocks at %p, %p, %p, %p\n", ptr1, ptr2, ptr3, ptr4);

    // Free some blocks
    mem_free(ptr2);
    mem_free(ptr3);

    // Defrag the memory
    printf("Defragmenting memory...\n");
    mem_defrag();

    // Allocate more memory after defrag
    void *ptr5 = mem_alloc(100);

    // Free memory
    mem_free(ptr1);
    mem_free(ptr4);
    mem_free(ptr5);

    // Clean up
    free(pool);

    return 0;
}
