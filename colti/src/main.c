#include "precomph.h"

int main(int argc, const char** argv)
{
	ParseResult result = ParseArguments(argc, argv);
	if (result.file_path == NULL)
	{
		printf("> ");
		String line = StringGetLine();		
		StringFree(&line);
	}
	else
	{
		String file_content = StringGetFileContent(argv[1]);
		StringFree(&file_content);
	}	
}