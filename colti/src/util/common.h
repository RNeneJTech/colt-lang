/** @file common.h
* Contains common macros and types used throughout the interpreter.
*/

#ifndef HG_COLTI_COMMON
#define HG_COLTI_COMMON

#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>

#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include "colti_config.h"
#include "memory.h"
#include "console_colors.h"
#include "values/colti_floating_value.h"
#include "values/colti_integer_value.h"

/// @brief Represents the result of an interpretation of a program
typedef enum
{
	INTERPRET_OK,
	INTERPRET_COMPILE_ERROR,
	INTERPRET_RUNTIME_ERROR
} InterpretResult;

/// @brief Represents a Byte, which can be type-punned to a more useful type
typedef union
{
	ColtiBool b;
	ColtiI8 i8;
	ColtiUI8 ui8;
} BYTE;

/// @brief Represents a Word (2 bytes), which can be type-punned to a more useful type
typedef union
{
	ColtiBool b;
	ColtiI8 i8;
	ColtiUI8 ui8;
	BYTE byte;

	ColtiI16 i16;
	ColtiUI16 ui16;
} WORD;

/// @brief Represents a Double Word (4 bytes), which can be type-punned to a more useful type
typedef union
{
	ColtiBool b;
	ColtiI8 i8;
	ColtiUI8 ui8;
	BYTE byte;

	ColtiI16 i16;
	ColtiUI16 ui16;
	WORD word;

	ColtiFloat f;
	ColtiI32 i32;
	ColtiUI32 ui32;
} DWORD;

/// @brief Represents a Quad Word (8 bytes), which can be type-punned to a more useful type
typedef union
{
	ColtiBool b;
	ColtiI8 i8;
	ColtiUI8 ui8;
	BYTE byte;

	ColtiI16 i16;
	ColtiUI16 ui16;
	WORD word;
	
	ColtiFloat f;
	ColtiI32 i32;
	ColtiUI32 ui32;
	DWORD dword;

	ColtiDouble d;
	ColtiI64 i64;
	ColtiUI64 ui64;
} QWORD;

#ifdef COLTI_WINDOWS
	#define COLTI_CURRENT_FILENAME (strrchr("\\" __FILE__, '\\') + 1)
#else
	#define COLTI_CURRENT_FILENAME (strrchr("/" __FILE__, '/') + 1)
#endif

#ifdef COLTI_DEBUG_BUILD
	//Terminates the program if 'cond' is not true, and in that case prints error with useful debug info
	#define colti_assert(cond, error) do { \
	if (!(cond)) { \
		printf(CONSOLE_FOREGROUND_BRIGHT_RED "\nAssertion failed from file " CONSOLE_FOREGROUND_BRIGHT_WHITE "\"%s\"" \
			CONSOLE_FOREGROUND_BRIGHT_RED ", at line " CONSOLE_FOREGROUND_BRIGHT_MAGENTA "%d" \
			CONSOLE_FOREGROUND_BRIGHT_RED " in function " CONSOLE_FOREGROUND_BRIGHT_WHITE "\"%s\"" \
			CONSOLE_FOREGROUND_BRIGHT_RED ":\nError: " CONSOLE_FOREGROUND_BRIGHT_CYAN "%s\n" CONSOLE_COLOR_RESET, \
			COLTI_CURRENT_FILENAME, __LINE__, __FUNCTION__, (error)); \
		(void)getc(stdin); \
		exit(1); \
	} } while (0)
	
	//On debug builds we want to check for memory leaks and where they are coming from
	#define safe_malloc(size)		checked_malloc(size)
	#define safe_free(ptr)			checked_free(ptr)
	
	//Rather than having to type #ifdef COLTI_DEBUG_BUILD
	#define DO_IF_DEBUG_BUILD(what) do { what; } while(0)
#else
	#define colti_assert(cond, error)
	
	//On release builds we don't want overhead for allocating
	#define safe_malloc(size)		checked_malloc(size)
	#define safe_free(ptr)			checked_free(ptr)

	//Doesn't do anything
	#define DO_IF_DEBUG_BUILD(what) do { (what); } while(0)
#endif

#endif //HG_COLTI_COMMON