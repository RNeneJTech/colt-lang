#ifndef HG_COLTI_PARSE_ARGS
#define HG_COLTI_PARSE_ARGS

#include "common.h"
#include "chunk.h"
#include "disassemble.h"

/// @brief The result of parsing command line arguments
typedef struct
{
	/// @brief The path to the file to interpret/compile
	const char* file_path;
} ParseResult;

typedef enum
{
	/// @brief -h, or --help
	ARG_HELP,
	/// @brief -v, or --version
	ARG_VERSION,
	/// @brief -d, or --disassemble
	ARG_DISASSEMBLE,
	/// @brief Any invalid argument
	ARG_INVALID
} CommandLineArgument;

/// @brief Parses the command line arguments and returns a corresponding result
/// @param argc The argument count
/// @param argv The argument values
/// @return 
ParseResult ParseArguments(int argc, const char** argv);

/// @brief Check if a path is a valid file
/// @param path The path to check for
/// @return True if the path is valid and points to a file
bool checkIfValidFile(const char* path);

/*****************************************
IMPLEMENTATION HELPERS
*****************************************/

/// @brief Transform a string to an enum value, which makes it easier to deal with
/// @param str The string to convert, which SHOULD start with '-'
/// @return The argument type, which can be ARG_INVALID when the 'str' is not recognized
CommandLineArgument impl_string_to_arg(const char* str);

/// @brief Handles the -v or --version logic, and exits
/// @param argc The argument count
/// @param argv The argument values
void impl_version(int argc, const char** argv);

/// @brief Handles the -d or --disassemble logic, and exits
/// @param argc The argument count
/// @param argv The argument values
void impl_disassemble(int argc, const char** argv);

/// @brief Handles the -h or --help logic, and exits
/// @param argc The argument count
/// @param argv The argument values
void impl_help(int argc, const char** argv);

/// @brief Prints an error that is caused by an invalid combination of arguments.
/// Expects at least that argc is 2.
/// @param argc The argument count
/// @param argv The argument values
void impl_print_invalid_combination(int argc, const char** argv);

/// @brief Prints the help of '-d' or '--disassemble'
void impl_help_disassemble();

/// @brief Prints the help of '-v' or '--version'
void impl_help_version();

/// @brief Prints the help of '-h' or '--help'
void impl_help_help();

#endif //HG_COLTI_PARSE_ARGS