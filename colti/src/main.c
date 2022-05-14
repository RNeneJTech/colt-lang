#include "precomph.h"
#include "vm/stack_based_vm.h"

int main(int argc, const char** argv)
{
	printf(CONSOLE_BACKGROUND_BRIGHT_MAGENTA CONSOLE_FOREGROUND_BLACK
		"COLTI v%s on %s" CONSOLE_COLOR_RESET "\n", COLTI_VERSION_STRING, COLTI_OS_STRING);
	
	if (argc == 1)
	{
		printf("> ");
		String line = StringGetLine();		
		StringFree(&line);
	}
	else if (argc == 2)
	{
		String file_content = StringGetFileContent(argv[1]);
		StringFree(&file_content);
	}
	else
	{
		printf("Usage: "CONSOLE_FOREGROUND_BRIGHT_BLUE"colti"CONSOLE_FOREGROUND_BRIGHT_MAGENTA" [path]"CONSOLE_COLOR_RESET);
		exit(3);
	}	
}