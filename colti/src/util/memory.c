/** @file memory.c
* Contains the definitions of the function declared in 'memory.h'
*/

#include "memory.h"

void* checked_malloc(size_t size)
{
	void* ptr = malloc(size);
	if (ptr) return ptr;

	printf(CONSOLE_FOREGROUND_BRIGHT_RED "Error: Couldn't allocate memory!");
	(void)getc(stdin);
	exit(2);
}

void checked_free(void* ptr)
{
	if (ptr)
	{
		free(ptr); return;
	}
	printf(CONSOLE_FOREGROUND_BRIGHT_RED "Error: Pointer passed 'checked_free' was NULL!");
	(void)getc(stdin);
	exit(2);
}