#include "precomph.h"

int main(int argc, const char** argv)
{
	ParseResult result = ParseArguments(argc, argv);
	if (result.file_path_in == NULL)
	{
		printf("> ");
		String line = StringGetLine();
		Scanner scan;
		ScannerInit(&scan, StringToStringView(&line));
		Token tkn = ScannerGetNextToken(&scan);
		do
		{
			printf("%s\n", TokenToString(tkn));
			tkn = ScannerGetNextToken(&scan);
		} while (tkn != TKN_EOF);

		StringFree(&line);
	}
	else
	{
		String file_content = StringGetFileContent(argv[1]);
		StringFree(&file_content);
	}
	DUMP_MEMORY_LEAKS();
}