/** @file scanner.c
* Contains the definitions of the functions declared in 'scanner.h'
*/

#include "scanner.h"

void ScannerInit(Scanner* scan, StringView to_scan)
{
	colti_assert(scan != NULL, "Pointer was NULL!");
	memset(scan, 0, sizeof(Scanner));
	scan->view = to_scan;
	scan->current_line = 1;
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
	return scan->parsed_uinteger;
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
	//we store the current offset, which is the beginning of the current lexeme
	scan->lexeme_begin = scan->offset - 1;

	if (isalpha(next_char))
		return impl_scanner_handle_identifier(scan, next_char);
	else if (isdigit(next_char))
		return impl_scanner_handle_digit(scan, next_char);
	return TKN_EOF;
}

/**********************************
IMPLEMENTATION HELPERS
**********************************/

void impl_scanner_print_error(const Scanner* scan, const char* error, ...)
{
	fprintf(stderr, CONSOLE_FOREGROUND_BRIGHT_RED"Error: "CONSOLE_COLOR_RESET"On line %"PRIu64": ", scan->current_line);
	
	//prints the error
	va_list args;
	va_start(args, error);
	vfprintf(stderr, error, args);
	va_end(args);
	fputc('\n', stderr);

	size_t line_begin = 0;
	//set the searching pointer to before the lexeme, and start searching backwards
	//for the first '\n', which would be the beginning of the newline.
	const char* newline = scan->view.start + scan->lexeme_begin;
	while (newline != scan->view.start)
	{
		if (*newline == '\n')
		{
			line_begin = newline - scan->view.start;
			break;
		}
		newline--;
	}

	//starts with the size of the string from the beginning of the line till the end of the whole string.
	size_t line_end = scan->view.end - newline;
	//reset the newline pointer to after the lexeme, and start looking for a '\n'
	newline = scan->view.start + scan->offset;
	while (newline != scan->view.end)
	{
		if (*newline == '\n')
		{
			line_end = newline - scan->view.start;
			break;
		}
		newline++;
	}

	//This offset variable allows to fix highlighting issues:
	//When the lexeme is not the last one, we need to remove 1 from the size of the lexeme.
	//But if it's the last one, doing this results in a character getting chopped off.
	int offset = (impl_peek_next_char(scan, 0) == EOF ? 0 : 1);

	//To highlight the error lexeme, we need to break down the line in 3 parts:
	//
	fprintf(stderr, "%.*s"CONSOLE_BACKGROUND_BRIGHT_RED"%.*s"CONSOLE_COLOR_RESET"%.*s\n",
		(uint32_t)(scan->lexeme_begin - line_begin), scan->view.start + line_begin,
		(uint32_t)(scan->offset - scan->lexeme_begin - offset), scan->view.start + scan->lexeme_begin,
		(uint32_t)(line_end - scan->offset), scan->view.end - (line_end - scan->offset) - offset
	);
}

char impl_get_next_char(Scanner* scan)
{
	if (scan->offset < (uint64_t)(scan->view.end - scan->view.start))
		return scan->view.start[scan->offset++];
	return EOF;
}

char impl_peek_next_char(const Scanner* scan, uint64_t offset)
{
	if (scan->offset + offset < (uint64_t)(scan->view.end - scan->view.start))
		return scan->view.start[scan->offset + offset];
	return EOF;
}

Token impl_scanner_handle_identifier(Scanner* scan, char current_char)
{
	//Clear the string
	scan->parsed_identifier.size = 1;
	scan->parsed_identifier.ptr[0] = '\0';

	StringAppendChar(&scan->parsed_identifier, current_char);
	
	char next_char = impl_get_next_char(scan);
	while (isalnum(next_char))
	{
		StringAppendChar(&scan->parsed_identifier, next_char);
		next_char = impl_get_next_char(scan);
	}

	return impl_token_identifier_or_keyword(&scan->parsed_identifier);
}

Token impl_scanner_handle_digit(Scanner* scan, char current_char)
{
	//Clear the string
	scan->parsed_identifier.size = 1;
	scan->parsed_identifier.ptr[0] = '\0';

	StringAppendChar(&scan->parsed_identifier, current_char);

	if (current_char == '0') //Could be 0x, 0b, 0o
	{
		char after_0 = (char)tolower(impl_peek_next_char(scan, 0));
		int base = 10;
		switch (after_0)
		{
		break; case 'x': //HEXADECIMAL
			base = 16;			
		break; case 'b': //BINARY
			base = 2;
		break; case 'o': //OCTAL	
			base = 8;
		break; default:
			if (!isdigit(after_0))
			{
				scan->parsed_uinteger = 0;
				return TKN_INTEGER;
			}
			else //We recurse now that we have popped the leading 0
				return impl_scanner_handle_digit(scan, impl_get_next_char(scan));
		}
		//Handle the different bases: 0x, 0b, 0o
		after_0 = (char)tolower(impl_get_next_char(scan)); //consume the x|b|o
		impl_parse_alnum(scan);
		if (scan->parsed_identifier.size == 2) //Contains only the '0'
		{
			const char* range_str;
			switch (after_0)
			{
			break; case 'x':
				range_str = "[0-9a-f]";
			break; case 'b':
				range_str = "[0-1]";
			break; case 'o':
				range_str = "[0-7]";
			break; default: //should never happen
				colti_assert(false, "after_0 was an unexpected value!");
				range_str = "ERROR";
			}
			impl_scanner_print_error(scan, "'0%c' should be followed by characters in range %s!", after_0, range_str);
			return TKN_ERROR;
		}
		return impl_token_str_to_uinteger(scan, base);
	}

	//Parse as many digits as possible
	char next_char = impl_parse_digits(scan);	

	bool isfloat = false;
	// [0-9]+ followed by a .[0-9] is a float
	if (next_char == '.' && isdigit(impl_peek_next_char(scan, 0)))
	{
		isfloat = true;
		StringAppendChar(&scan->parsed_identifier, next_char);

		//Parse as many digits as possible
		next_char = impl_parse_digits(scan);
	}
	char after_e = impl_peek_next_char(scan, 0);
	// [0-9]+(.[0-9]+)?e[+-][0-9]+ is a float
	if (next_char == 'e' && (after_e == '+' || after_e == '-' || isdigit(after_e)))
	{
		isfloat = true;
		StringAppendChar(&scan->parsed_identifier, next_char);
		next_char = impl_get_next_char(scan);
		if (next_char == '+') //skip the + after the exponent
			next_char = impl_get_next_char(scan);

		StringAppendChar(&scan->parsed_identifier, next_char);		
		
		//Parse as many digits as possible
		(void)impl_parse_digits(scan);
	}

	if (isfloat)
		return impl_token_str_to_double(scan);
	else
		return impl_token_str_to_uinteger(scan, 10);
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

Token impl_token_str_to_double(Scanner* scan)
{
	char* end;
	double value = strtod(scan->parsed_identifier.ptr, &end);
	if (end != scan->parsed_identifier.ptr + scan->parsed_identifier.size - 1)
	{
		impl_scanner_print_error(scan, "Unexpected character '%c' while parsing floating point literal.", *end);
		return TKN_ERROR;
	}
	else if (value == HUGE_VAL && errno == ERANGE)
	{
		errno = 0;
		impl_scanner_print_error(scan, "Floating point literal is not representable.");
		return TKN_ERROR;
	}
	scan->parsed_double = value;
	return TKN_DOUBLE;
}

Token impl_token_str_to_uinteger(Scanner* scan, int base)
{
	char* end;
	uint64_t value = strtoull(scan->parsed_identifier.ptr, &end, base);
	
	if (end != scan->parsed_identifier.ptr + scan->parsed_identifier.size - 1)
	{
		impl_scanner_print_error(scan, "Unexpected character '%c' while parsing integer literal.", *end);
		return TKN_ERROR;
	}
	else if (value == ULLONG_MAX && errno == ERANGE)
	{
		errno = 0;
		impl_scanner_print_error(scan, "Integer literal is not representable.");
		return TKN_ERROR;
	}
	scan->parsed_uinteger = value;
	return TKN_INTEGER;
}

char impl_parse_alnum(Scanner* scan)
{
	char next_char = impl_get_next_char(scan);
	while (isalnum(next_char))
	{
		StringAppendChar(&scan->parsed_identifier, next_char);
		next_char = impl_get_next_char(scan);
	}
	return next_char;
}

char impl_parse_digits(Scanner* scan)
{
	char next_char = impl_get_next_char(scan);
	while (isdigit(next_char))
	{
		StringAppendChar(&scan->parsed_identifier, next_char);
		next_char = impl_get_next_char(scan);
	}
	return next_char;
}
