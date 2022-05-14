#include "struct_string.h"

void StringInit(String* str)
{
	str->capacity = STRING_SMALL_BUFFER_OPTIMIZATION;
	str->size = 0;
	str->ptr = str->buffer;
}

void StringFree(String* str)
{
	if (str->capacity == STRING_SMALL_BUFFER_OPTIMIZATION)
		return;
	safe_free(str->ptr);

	DO_IF_DEBUG_BUILD(str->size = 0);
	DO_IF_DEBUG_BUILD(str->capacity = 0);
}
