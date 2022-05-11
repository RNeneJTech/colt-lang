/** @file byte_code.h
* Contains the byte-code enums, and helper functions to execute the byte-codes.
* The OpCode enum represents an Operation Code, which describes an operation
* to be done by the interpreter.
* These byte-codes are usually followed by operands giving more informations about
* what to do. For example, OP_NEGATE should be followed by an OperandType,
* which describes the actual type of the QWORD to negate.
* To abstract away the OpCode, how they and their operands are stored,
* helper function of the form OpCode_{OP_CODE_NAME} are written, which expects
* the operands on which to perform the operation, and the operands describing that operation.
*/

#ifndef HG_COLTI_BYTE_CODE
#define HG_COLTI_BYTE_CODE

#include "common.h"

/// @brief Represents an instruction to be executed by the VM
typedef enum
{
	//IMMEDIATE VALUES LOADING
	OP_IMMEDIATE_BYTE,
	OP_IMMEDIATE_WORD,
	OP_IMMEDIATE_DWORD,
	OP_IMMEDIATE_QWORD,

	//followed by an operand,
	//pops an 'operand' from the stack
	//converts it to 'operand', negates it,
	//then pushes it back
	OP_NEGATE,

	//followed by two operand,
	//pops an 'first operand' from the stack,
	//converts it to 'second operand',
	//then pushes it back
	OP_CONVERT,

	//followed by an operand,
	//pops the first and second item from the stack,
	//converts them to the operand,
	//then pushes the result to the stack
	OP_ADD,
	OP_SUBTRACT,
	OP_MULTIPLY,
	OP_DIVIDE,

	//DON'T KNOW
	OP_MODULO,

	//DEBUG PURPOSES
	OP_PRINT, // followed by an operand

	//MISCALLENEOUS
	OP_RETURN,
} OpCode;


/// @brief Represents an operand type
typedef enum
{
	COLTI_BOOL		= OPERAND_COLTI_BOOL,
	COLTI_DOUBLE	= OPERAND_COLTI_DOUBLE,
	COLTI_FLOAT		= OPERAND_COLTI_FLOAT,

	COLTI_INT8		= OPERAND_COLTI_I8,
	COLTI_INT16		= OPERAND_COLTI_I16,
	COLTI_INT32		= OPERAND_COLTI_I32,
	COLTI_INT64		= OPERAND_COLTI_I64,

	COLTI_UINT8		= OPERAND_COLTI_UI8,
	COLTI_UINT16	= OPERAND_COLTI_UI16,
	COLTI_UINT32	= OPERAND_COLTI_UI32,
	COLTI_UINT64	= OPERAND_COLTI_UI64,
} OperandType;

/**********************************
BYTE-CODE RUNNING
**********************************/

/// @brief Casts 'value' to 'type', negates the result and return it
/// @param value The QWORD on which to operate
/// @param type The type to which to cast the QWORD
/// @return The modified QWORD
QWORD OpCode_Negate(QWORD value, OperandType type);

/// @brief Casts 'value' from 'from' to 'to'
/// @param value The QWORD on which to operate
/// @param from The type of the QWORD
/// @param to The type to which to cast the QWORD
/// @return The modified QWORD
QWORD OpCode_Convert(QWORD value, OperandType from, OperandType to);

/// @brief Casts 2 QWORD and return their sums
/// @param left The left hand side
/// @param right The right hand side
/// @param type The type of the QWORDs
/// @return The sum of the QWORDs
QWORD OpCode_Sum(QWORD left, QWORD right, OperandType type);

/// @brief Casts 2 QWORD and return their difference
/// @param left The left hand side
/// @param right The right hand side
/// @param type The type of the QWORDs
/// @return The difference of the QWORDs
QWORD OpCode_Difference(QWORD left, QWORD right, OperandType type);

/// @brief Casts 2 QWORD and return their multiplication
/// @param left The left hand side
/// @param right The right hand side
/// @param type The type of the QWORDs
/// @return The multiplication of the QWORDs
QWORD OpCode_Multiply(QWORD left, QWORD right, OperandType type);

/// @brief Casts 2 QWORD and return their division
/// @param left The left hand side
/// @param right The right hand side
/// @param type The type of the QWORDs
/// @return The division of the QWORDs
QWORD OpCode_Divide(QWORD left, QWORD right, OperandType type);

/// @brief Casts 'value' to 'type' then prints its value, for DEBUG purposes
/// @param value The QWORD to print
/// @param type The type of the QWORD
void OpCode_Print(QWORD value, OperandType type);

#endif //HG_COLTI_BYTE_CODE