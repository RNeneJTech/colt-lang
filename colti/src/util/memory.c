#include "memory.h"

void* checked_malloc(size_t size)
{
    void* ptr = malloc(size);
    if (ptr) return ptr;

    printf("Error: Couldn't allocate memory!");
    exit(2);
}