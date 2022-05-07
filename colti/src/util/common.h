#ifndef HG_COLTI_COMMON
#define HG_COLTI_COMMON

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include "memory.h"

#ifdef COLTI_DEBUG_BUILD
	//Terminates 
	#define colti_assert(cond, error) do { \
	if (!(cond)) { \
		printf("\nError from file %s, at line %d:\nFunction: %s\nError: %s\n", __FILE__, __LINE__, __FUNCTION__, (error)); \
		getc(stdin); \
		exit(1); \
	} } while (0)
	
	//On debug builds we want to check for memory leaks and where they are coming from
	#define safe_malloc(size)		checked_malloc(size)
	#define safe_free(ptr)			free(ptr)
	
	//Rather than having to type #ifdef COLTI_DEBUG_BUILD
	#define DO_IF_DEBUG_BUILD(what) do { (what); } while(0)
#else
	#define colti_assert(cond, error)
	
	//On release builds we don't want overhead for allocating
	#define safe_malloc(size)		checked_malloc(size)
	#define safe_free(ptr)			free(ptr)

	//Doesn't do anything
	#define DO_IF_DEBUG_BUILD(what) do { (what); } while(0)
#endif

#endif //HG_COLTI_COMMON