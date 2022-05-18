#include "precomph.h"

int main(int argc, const char** argv)
{
	ParseResult result = ParseArguments(argc, argv);
	if (result.file_path_in == NULL)
	{
		while (!feof(stdin))
		{
			printf("> ");
			String line = StringGetLine();
			Scanner scan;
			ScannerInit(&scan, StringToStringView(&line));
			Token tkn = ScannerGetNextToken(&scan);
			do
			{

				switch (tkn)
				{
				break; case TKN_DOUBLE:
					printf("%s: %g\n", TokenToString(tkn), scan.parsed_double);
				break; case TKN_INTEGER:
					printf("%s: %"PRIu64"\n", TokenToString(tkn), scan.parsed_uinteger);
				break; case TKN_IDENTIFIER:
					printf("%s: %s\n", TokenToString(tkn), scan.parsed_identifier.ptr);
				break; default:
					printf("%s\n", TokenToString(tkn));
				}
				tkn = ScannerGetNextToken(&scan);
			} while (tkn != TKN_EOF);
			StringFree(&line);
		}
	}
	else
	{
		String file_content = StringGetFileContent(argv[1]);
		StringFree(&file_content);
	}
	DUMP_MEMORY_LEAKS();
}