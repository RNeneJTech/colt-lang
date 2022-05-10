#ifndef HG_COLTI_BYTE_CODE
#define HG_COLTI_BYTE_CODE

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

/**********************************
BYTE-CODE RUNNING
**********************************/

#endif //HG_COLTI_BYTE_CODE