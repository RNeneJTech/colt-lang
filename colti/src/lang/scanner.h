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
	/// @brief The last parsed integer literal
	uint64_t parsed_integer;
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
/// @return The value stored in parsed_integer
uint64_t ScannerGetInt(const Scanner* scan);

/// @brief Get the next token from a scanner.
/// @param scan The scanner from which to get the value
/// @return A Token representing the parsed lexeme, or TKN_EOF if there are no more lexemes
Token ScannerGetNextToken(Scanner* scan);

/**********************************
IMPLEMENTATION HELPERS
**********************************/

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
Token impl_scanner_handle_identifier(Scanner* scan, char* current_char);

/// @brief Handles a digit case, searching for if it's a float or an integer
/// @param scan The scanner from which to get the value
/// @param current_char The pointer to the current char (which should be a digit)
/// @return The Token representing the identifier
Token impl_scanner_handle_digit(Scanner* scan, char* current_char);

/// @brief Handles comparisons for determining if an identifier is a keyword
/// @param str The string to compare
/// @return A Token representing a keyword, or TKN_IDENTIFIER
Token impl_token_identifier_or_keyword(const String* string);

Token impl_token_str_to_double(Scanner* scan);

Token impl_token_str_to_integer(Scanner* scan, int base);

#endif //HG_COLTI_SCANNER