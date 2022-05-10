#ifndef HG_COLTI_BYTE_CODE
#define HG_COLTI_BYTE_CODE

#include "values/colti_floating_value.h"
#include "values/colti_integer_value.h"

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
	OP_SUM,
	OP_DIFFERENCE,
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



#endif //HG_COLTI_BYTE_CODE