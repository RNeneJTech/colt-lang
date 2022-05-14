#ifndef HG_COLTI_STRUCT_STRING
#define HG_COLTI_STRUCT_STRING

#include "common.h"

#define STRING_SMALL_BUFFER_OPTIMIZATION 32

typedef struct
{
	uint64_t capacity;
	uint64_t size;
	char* ptr;
	char buffer[STRING_SMALL_BUFFER_OPTIMIZATION];
} String;


/// @brief Initializes a string
/// @param str The string to initialize
void StringInit(String* str);

/// @brief Frees the resources used by a string
/// @param str The string to modify
void StringFree(String* str);

/// @brief A non-owning view over an array of characters
typedef struct
{
	const char* start;
	const char* end;
} StringView;

#endif //HG_COLTI_STRUCT_STRING