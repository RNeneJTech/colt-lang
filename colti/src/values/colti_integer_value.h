/** @file colti_integer_value.h
* Contains typedefs representing integers and bool values in Colt.
*/

#ifndef HG_COLTI_INTEGER_VALUE
#define HG_COLTI_INTEGER_VALUE

#include <stdint.h>
#include <stdbool.h>

/// @brief Byte representing a 2 state value 'true' (1) 'false' (0)
typedef bool ColtiBool;
#define OPERAND_COLTI_BOOL 0

/// @brief Unsigned 8 bit integer
typedef uint8_t ColtiUI8;
#define OPERAND_COLTI_UI8 1

/// @brief Unsigned 16 bit integer
typedef uint16_t ColtiUI16;
#define OPERAND_COLTI_UI16 2

/// @brief Unsigned 32 bit integer
typedef uint32_t ColtiUI32;
#define OPERAND_COLTI_UI32 3

/// @brief Unsigned 64 bit integer
typedef uint64_t ColtiUI64;
#define OPERAND_COLTI_UI64 4


/// @brief Signed 8 bit integer
typedef int8_t ColtiI8;
#define OPERAND_COLTI_I8 5

/// @brief Signed 16 bit integer
typedef int16_t ColtiI16;
#define OPERAND_COLTI_I16 6 

/// @brief Signed 32 bit integer
typedef int32_t ColtiI32;
#define OPERAND_COLTI_I32 7

/// @brief Signed 64 bit integer
typedef int64_t ColtiI64;
#define OPERAND_COLTI_I64 8


#endif //HG_COLTI_INTEGER_VALUE