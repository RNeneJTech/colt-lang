/** @file struct_string.c
* Contains the definitions of the functions declared in 'struct_string.h'
*/

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

void StringPrint(const String* str)
{
	printf("%.*s", (int)str->size, str->ptr);
}

uint64_t StringCapacity(const String* str)
{
	return str->capacity;
}

uint64_t StringSize(const String* str)
{
	return str->size;
}

bool StringIsEmpty(const String* str)
{
	return str->size == 0;
}

bool StringIsStackAllocated(const String* str)
{
	return str->capacity == STRING_SMALL_BUFFER_OPTIMIZATION;
}

void StringReplaceAllChar(String* str, char character, char with)
{
	colti_assert(str->ptr != NULL, "Huge bug: a string's buffer was NULL!");
	for (size_t i = 0; i < str->size; i++)
	{
		if (str->ptr[i] == character)
			str->ptr[i] = with;
	}
}

void StringReplaceAllString(String* str, const char* what, const char* with)
{
	colti_assert(str->ptr != NULL, "Huge bug: a string's buffer was NULL!");
	colti_assert(false, "NOT IMPLEMENTED!");
}

void StringFill(String* str, char character)
{
	colti_assert(str->ptr != NULL, "Huge bug: a string's buffer was NULL!");
	memset(str->ptr, character, str->size);
}

void StringReserve(String* str, size_t size)
{
	char* temp = (char*)safe_malloc(str->capacity += size);
	memcpy(temp, str->ptr, str->size);

	if (str->capacity != STRING_SMALL_BUFFER_OPTIMIZATION + size)
		safe_free(str->ptr);
	str->ptr = temp;
}

String StringGetLine()
{
	String str;
	str.capacity = 1024;
	str.size = 0;
	str.ptr = safe_malloc(1024);
	fgets(str.ptr, 1024, stdin);
	return str;
}

String StringGetFileContent(const char* path)
{
	FILE* file = fopen(path, "rb"); //Read-binary mode
	if (file == NULL)
	{
		fprintf(stderr, CONSOLE_FOREGROUND_BRIGHT_RED"Couldn't open the file '%s'!\n"CONSOLE_COLOR_RESET, path);
		exit(3);
	}
	fseek(file, 0L, SEEK_END);
	size_t file_size = ftell(file); //Get file size
	
	String str;
	str.ptr = safe_malloc(str.capacity = file_size + 1);
	str.size = str.capacity;
	
	rewind(file); //Go back to the beginning of the file
	size_t bytes_read = fread(str.ptr, sizeof(char), file_size, file);
	fclose(file);
	if (bytes_read != file_size)
	{
		fprintf(stderr, CONSOLE_FOREGROUND_BRIGHT_RED"Couldn't read all the content of the file '%s'!\n"CONSOLE_COLOR_RESET, path);
		exit(2);
	}
	str.ptr[str.size - 1] = '\0';
	return str;
}

StringView StringToStringView(const String* str)
{
	StringView strv = { str->ptr, str->ptr + str->size };
	return strv;
}

void StringViewPrint(const StringView strv)
{
	printf("%.*s", (int)(strv.end - strv.start), strv.start);
}
