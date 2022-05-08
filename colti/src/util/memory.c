#include "memory.h"

void* checked_malloc(size_t size)
{
    void* ptr = malloc(size);
    if (ptr) return ptr;

    printf("Error: Couldn't allocate memory!");
    exit(2);
}

void checked_free(void* ptr)
{
    if (ptr)
    {
        free(ptr); return;
    }
    printf("Error: Pointer was NULL when it shouldn't be!");
    exit(2);
}