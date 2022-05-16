/** @file memory.c
* Contains the definitions of the functions declared in 'memory.h'
*/

#include "memory.h"

void* checked_malloc(size_t size)
{
	void* ptr = malloc(size);
	if (ptr) return ptr;

	printf(CONSOLE_FOREGROUND_BRIGHT_RED "Error: "CONSOLE_COLOR_RESET"Could not allocate memory!\n");
	(void)getc(stdin);
	exit(2);
}

void checked_free(void* ptr)
{
	if (ptr)
	{
		free(ptr); return;
	}
	printf(CONSOLE_FOREGROUND_BRIGHT_RED "Error: "CONSOLE_COLOR_RESET"Pointer passed 'checked_free' was NULL!");
	(void)getc(stdin);
	exit(2);
}