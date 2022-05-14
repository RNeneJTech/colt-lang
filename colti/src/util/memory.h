/** @file memory.h
* Contains helper for managing memory allocations.
*/

#ifndef HG_COLTI_MEMORY
#define HG_COLTI_MEMORY

#include "console_colors.h"
#include <stdio.h>

#ifdef COLTI_WINDOWS
	//The current filename (strips path using Windows separator)
	#define COLTI_CURRENT_FILENAME (strrchr("\\" __FILE__, '\\') + 1)
#else
	//The current filename (strips path using Unix separator)
	#define COLTI_CURRENT_FILENAME (strrchr("/" __FILE__, '/') + 1)
#endif

/// @brief Allocates a block of size 'size' from the heap, but terminates if the pointer is NULL
/// @param size The size of the block to allocate
/// @return a non-NULL pointer
void* checked_malloc(size_t size);

/// @brief Frees the pointer if it isn't NULL, else terminates.
/// While this might seem not useful, any pointer obtained by checked_malloc
/// is not NULL. This means that if a pointer is NULL, a programming bug happened.
/// @param ptr The pointer to free
void checked_free(void* ptr);

#ifdef COLTI_DEBUG_BUILD

void* debug_checked_malloc(size_t size, const char* from, size_t str_size);

void debug_checked_free(void* ptr);

#endif //COLTI_DEBUG_BUILD

#endif //HG_COLTI_MEMORY