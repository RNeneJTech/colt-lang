#ifndef HG_COLTI_SCANNER
#define HG_COLTI_SCANNER

#include "common.h"
#include "structs/struct_string.h"
#include "token.h"

typedef struct
{
	StringView view;
	uint64_t current_line;

	String parsed_identifier;
	double parsed_double;
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

#endif //HG_COLTI_SCANNER