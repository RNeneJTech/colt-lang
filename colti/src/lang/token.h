/** @file token.h
* Contains the tokens that can be returned from the scanner.
*/

#ifndef HG_COLTI_TOKEN
#define HG_COLTI_TOKEN

/// @brief Enum representing individual lexemes of the Colt language
typedef enum
{
	/// @brief +
	TKN_OPERATOR_PLUS,
	/// @brief ++
	TKN_OPERATOR_PLUS_PLUS,
	/// @brief +=
	TKN_OPERATOR_PLUS_EQUAL,

	/// @brief -
	TKN_OPERATOR_MINUS,
	/// @brief --
	TKN_OPERATOR_MINUS_PLUS,
	/// @brief -=
	TKN_OPERATOR_MINUS_EQUAL,

	/// @brief *
	TKN_OPERATOR_STAR,
	/// @brief *=
	TKN_OPERATOR_STAR_EQUAL,
	
	/// @brief /
	TKN_OPERATOR_SLASH,
	/// @brief /=
	TKN_OPERATOR_SLASH_EQUAL,

	/// @brief <
	TKN_OPERATOR_LESS,
	/// @brief <<
	TKN_OPERATOR_LESS_LESS,
	/// @brief <=
	TKN_OPERATOR_LESS_EQUAL,

	/// @brief >
	TKN_OPERATOR_GREATER,
	/// @brief >>
	TKN_OPERATOR_GREATER_GREATER,
	/// @brief >=
	TKN_OPERATOR_GREATER_EQUAL,

	/// @brief <:
	TKN_OPERATOR_LESS_COLON,
	/// @brief :>
	TKN_OPERATOR_COLON_GREATER,

	/// @brief =
	TKN_OPERATOR_EQUAL,
	/// @brief ==
	TKN_OPERATOR_EQUAL_EQUAL,
	
	/// @brief !
	TKN_OPERATOR_BANG,
	/// @brief !=
	TKN_OPERATOR_BANG_EQUAL,

	/// @brief &
	TKN_OPERATOR_AND,
	/// @brief &=
	TKN_OPERATOR_AND_EQUAL,
	/// @brief && or 'and'
	TKN_OPERATOR_AND_AND,

	/// @brief |
	TKN_OPERATOR_OR,
	/// @brief |=
	TKN_OPERATOR_OR_EQUAL,
	/// @brief || or 'or'
	TKN_OPERATOR_OR_OR,

	/// @brief ^
	TKN_OPERATOR_XOR,
	/// @brief ^=
	TKN_OPERATOR_XOR_EQUAL,

	/// @brief ~
	TKN_OPERATOR_TILDE

} Token;

/// @brief Converts a Token to a c-string
/// @param tkn The token to convert
/// @return A string representing the valid Token or UNKNOWN
const char* TokenToString(Token tkn);

#endif //HG_COLTI_TOKEN