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

/// @brief Represents the result of an interpretation of a program
typedef enum
{
	INTERPRET_OK,
	INTERPRET_COMPILE_ERROR,
	INTERPRET_RUNTIME_ERROR
} InterpretResult;

/// @brief Represents a Byte, which can be type-punned to [u]int8
typedef union
{
	uint8_t ui8;
	int8_t   i8;
} BYTE;

/// @brief Represents a Word, which can be type-punned to a [u]int16, or BYTE
typedef union
{
	BYTE byte;
	int16_t	 i16;
	uint16_t ui16;
} WORD;

/// @brief Represents a Double Word, which can be type-punned to a float 'f' or [u]int32 or WORD
typedef union
{
	BYTE byte;
	WORD word;
	ColtiFloat f;
	int32_t   i32;
	uint32_t ui32;
} DWORD;

/// @brief Represents a Quad Word, which can be type-punned to a double 'd' or [u]int64, DWORD
typedef union
{
	BYTE byte;
	WORD word;
	DWORD dword;
	ColtiDouble d;
	int64_t	  i64;
	uint64_t ui64;
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