/** @file scanner.h
* Contains the Scanner struct, which breaks a string into lexemes.
* As the Scanner does not need to modify the input string, it takes in
* a StringView.
* The Scanner also handles error printing through `impl_scanner_print_error`.
* The Scanner's string to integer can handles binary integers `0b`, decimal integers `0x`, octal integers `0o`.
*/

#ifndef HG_COLTI_SCANNER
#define HG_COLTI_SCANNER

#include "common.h"
#include "structs/struct_string.h"
#include "token.h"

/// @brief Struct responsible of breaking a string into lexemes
typedef struct
{
	/// @brief The string to scan
	StringView view;
	/// @brief The offset to the current character from the string
	uint64_t offset;
	/// @brief The offset to the beginning of the current lexeme
	uint64_t lexeme_begin;
	/// @brief The current line number
	uint64_t current_line;

	/// @brief The last parsed identifier
	String parsed_identifier;
	/// @brief The last parsed double literal
	double parsed_double;
	/// @brief The last parsed unsigned integer literal
	uint64_t parsed_uinteger;
	/// @brief The last parsed signed integer literal
	int64_t parsed_integer;
} Scanner;

/// @brief Initializes a Scanner
/// @param to_scan The scanner to initialize
void ScannerInit(Scanner* scan, StringView to_scan);

/// @brief Frees any resources used by a Scanner
/// @param scan The scanner to modify
void ScannerFree(Scanner* scan);

/// @brief Returns the parsed identifier
/// @param scan The scanner from which to get the value
/// @return A StringView of the 
StringView ScannerGetIdentifier(const Scanner* scan);

/// @brief Returns the parsed double/float
/// @param scan The scanner from which to get the value
/// @return The value stored in parsed_double
double ScannerGetDouble(const Scanner* scan);

/// @brief Returns the parsed integer/char
/// @param scan The scanner from which to get the value
/// @return The value stored in parsed_uinteger
uint64_t ScannerGetInt(const Scanner* scan);

/// @brief Get the next token from a scanner.
/// @param scan The scanner from which to get the value
/// @return A Token representing the parsed lexeme, or TKN_EOF if there are no more lexemes
Token ScannerGetNextToken(Scanner* scan);

/**********************************
IMPLEMENTATION HELPERS
**********************************/

/// @brief Prints formatted 'error' and highlights the current lexeme.
/// Prints the line number, the line, highlights the lexeme.
/// This function is as optimized as it can be, but it should still only
/// be called for when error are found to avoid slowdown.
/// @param scan The scanner from which to get the lexeme and line
/// @param error The error, which is a `printf` style-format string
/// @param  Variadic number of arguments to format to 'error'
void impl_scanner_print_error(const Scanner* scan, const char* error, ...);

/// @brief Returns the next character in the stream, and updates the offset
/// @param scan The scanner from which to get the character
/// @return The next character or EOF (-1) if no more characters are available
char impl_get_next_char(Scanner* scan);

/// @brief Returns the next character + offset in the stream, without updating the offset.
/// To peek to the next character, let offset be equal to 0.
/// @param scan The scanner from which to get the character
/// @param offset The next character + offset
/// @return The character or EOF if no more characters are available
char impl_peek_next_char(const Scanner* scan, uint64_t offset);

/// @brief Handles an identifier case, searching for if it's a keyword or not
/// @param scan The scanner from which to get the identifier
/// @param current_char The pointer to the current char (which should be an alpha), which will be modified
/// @return The Token representing the identifier
Token impl_scanner_handle_identifier(Scanner* scan, char current_char);

/// @brief Handles a digit case, searching for if it's a float or an integer
/// @param scan The scanner from which to get the value
/// @param current_char The pointer to the current char (which should be a digit)
/// @return The Token representing the identifier
Token impl_scanner_handle_digit(Scanner* scan, char current_char);

Token impl_scanner_handle_plus(Scanner* scan);

Token impl_scanner_handle_minus(Scanner* scan);

Token impl_scanner_handle_star(Scanner* scan);

Token impl_scanner_handle_slash(Scanner* scan);

Token impl_scanner_handle_dot(Scanner* scan);

Token impl_scanner_handle_less(Scanner* scan);

Token impl_scanner_handle_greater(Scanner* scan);

/// @brief Handles comparisons for determining if an identifier is a keyword
/// @param str The string to compare
/// @return A Token representing a keyword, or TKN_IDENTIFIER
Token impl_token_identifier_or_keyword(const String* string);

/// @brief Converts a scanner's identifier string to a double.
/// This function also stores the result in the scanner's 'parsed_double',
/// and handles any error.
/// @param scan The scanner to modify
/// @return TKN_DOUBLE or TKN_ERROR if an error is encountered
Token impl_token_str_to_double(Scanner* scan);

/// @brief Converts a scanner's identifier string to an int of base 'base'.
/// This function also stores the result of the conversion in the scanner's
/// 'parsed_uinteger' and handles any error.
/// @param scan The scanner to modify
/// @param base The base of the int to parse
/// @return TKN_INTEGER or TKN_ERROR if an error is encountered
Token impl_token_str_to_uinteger(Scanner* scan, int base);

/// @brief Adds characters to the scanner's identifier string while they are alpha or digits
/// @param scan The scanner to modify
char impl_parse_alnum(Scanner* scan);

/// @brief Adds characters to the scanner's identifier string while they are digits
/// @param scan The scanner to modify
char impl_parse_digits(Scanner* scan);

#endif //HG_COLTI_SCANNER