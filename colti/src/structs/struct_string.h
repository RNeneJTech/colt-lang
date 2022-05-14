#ifndef HG_COLTI_STRUCT_STRING
#define HG_COLTI_STRUCT_STRING

#include "common.h"

#define STRING_SMALL_BUFFER_OPTIMIZATION

typedef struct
{
	uint64_t capacity;
	uint64_t size;
	char* ptr;
	char buffer[32];
} String;

#endif //HG_COLTI_STRUCT_STRING