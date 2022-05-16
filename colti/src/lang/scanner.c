/** @file scanner.c
* Contains the definitions of the functions declared in 'scanner.h'
*/

#include "scanner.h"

void ScannerInit(Scanner* scan, StringView to_scan)
{
	colti_assert(scan != NULL, "Pointer was NULL!");
	memset(scan, 0, sizeof(Scanner));
	scan->view = to_scan;
	StringInit(&scan->parsed_identifier);
}

void ScannerFree(Scanner* scan)
{
	StringFree(&scan->parsed_identifier);
}

StringView ScannerGetIdentifier(const Scanner* scan)
{
	return StringToStringView(&scan->parsed_identifier);
}

double ScannerGetDouble(const Scanner* scan)
{
	return scan->parsed_double;
}

uint64_t ScannerGetInt(const Scanner* scan)
{
	return scan->parsed_integer;
}

Token ScannerGetNextToken(Scanner* scan)
{
	char next_char = impl_get_next_char(scan);
	while (isspace(next_char))
	{
		if (next_char == '\n')
			scan->current_line += 1;
		next_char = impl_get_next_char(scan);
	}

	if (isalpha(next_char))
		return impl_scanner_handle_identifier(scan, &next_char);
	//else if (isdigit(next_char))
		//return impl_scanner_handle_digit(scan, &next_char);
	return TKN_EOF;
}

/**********************************
IMPLEMENTATION HELPERS
**********************************/

char impl_get_next_char(Scanner* scan)
{
	if (scan->offset < (uint64_t)(scan->view.end - scan->view.start))
		return scan->view.start[scan->offset++];
	return EOF;
}

char impl_peek_next_char(Scanner* scan, uint64_t offset)
{

}

Token impl_scanner_handle_identifier(Scanner* scan, char* current_char)
{
	//Clear the string
	scan->parsed_identifier.size = 1;
	scan->parsed_identifier.ptr[0] = '\0';

	StringAppendChar(&scan->parsed_identifier, *current_char);
	
	char next_char = impl_get_next_char(scan);
	while (isalnum(next_char))
	{
		StringAppendChar(&scan->parsed_identifier, next_char);
		next_char = impl_get_next_char(scan);
	}

	//We update the current character
	*current_char = next_char;
	return impl_token_identifier_or_keyword(&scan->parsed_identifier);
}

Token impl_token_identifier_or_keyword(const String* string)
{
	const char* str = string->ptr;
	//size - 1 as we don't care about comparing the NUL terminator
	size_t len = string->size - 1;
	
	//Table of keywords
	//We optimize comparisons by comparing the first character
	switch (str[0])
	{
	break; case 'a':
		if (strncmp(str, "and", len) == 0)
			return TKN_OPERATOR_AND_AND;
	break; case 'b':
		if (strncmp(str, "break", len) == 0)
			return TKN_KEYWORD_BREAK;
	break; case 'c':
		if (strncmp(str, "case", len) == 0)
			return TKN_KEYWORD_CASE;
		else if (strncmp(str, "continue", len) == 0)
			return TKN_KEYWORD_CONTINUE;
	break; case 'd':
		if (strncmp(str, "default", len) == 0)
			return TKN_KEYWORD_DEFAULT;
	break; case 'e':
		if (strncmp(str, "elif", len) == 0)
			return TKN_KEYWORD_ELIF;
		else if (strncmp(str, "else", len) == 0)
			return TKN_KEYWORD_ELSE;
	break; case 'f':
		if (strncmp(str, "for", len) == 0)
			return TKN_KEYWORD_FOR;
	break; case 'g':
		if (strncmp(str, "goto", len) == 0)
			return TKN_KEYWORD_GOTO;
	break; case 'i':
		if (strncmp(str, "if", len) == 0)
			return TKN_KEYWORD_IF;
	break; case 'o':
		if (strncmp(str, "or", len) == 0)
			return TKN_OPERATOR_OR_OR;
	break; case 's':
		if (strncmp(str, "switch", len) == 0)
			return TKN_KEYWORD_SWITCH;
	break; case 'w':
		if (strncmp(str, "while", len) == 0)
			return TKN_KEYWORD_WHILE;
	break; default:
		return TKN_IDENTIFIER;
	}
	return TKN_IDENTIFIER;
}
