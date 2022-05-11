/** @file byte_code.c
* Contains the definitions of the functions declared in 'byte_code.h'
*/

#include "byte_code.h"

QWORD OpCode_Negate(QWORD value, OperandType type)
{
	QWORD result;
	switch (type)
	{
	break; case OPERAND_COLTI_I8:		result.i8 = -value.i8;
	break; case OPERAND_COLTI_I16:		result.i16 = -value.i16;
	break; case OPERAND_COLTI_I32:		result.i32 = -value.i32;
	break; case OPERAND_COLTI_I64:		result.i64 = -value.i64;
	break; case OPERAND_COLTI_FLOAT:	result.f = -value.f;
	break; case OPERAND_COLTI_DOUBLE:	result.d = -value.d;
	break; default: colti_assert(false, "Invalid operand for OP_ADD!");
	}
	return result;
}

QWORD OpCode_Convert(QWORD value, OperandType from, OperandType to)
{
	colti_assert(false, "Still not implemented!");
}

QWORD OpCode_Sum(QWORD left, QWORD right, OperandType type)
{
	QWORD result;
	switch (type)
	{
	break; case OPERAND_COLTI_I8:		result.i8 = left.i8 + right.i8;
	break; case OPERAND_COLTI_I16:		result.i16 = left.i16 + right.i16;
	break; case OPERAND_COLTI_I32:		result.i32 = left.i32 + right.i32;
	break; case OPERAND_COLTI_I64:		result.i64 = left.i64 + right.i64;
	break; case OPERAND_COLTI_UI8:		result.ui8 = left.ui8 + right.ui8;
	break; case OPERAND_COLTI_UI16:		result.ui16 = left.ui16 + right.ui16;
	break; case OPERAND_COLTI_UI32:		result.ui32 = left.ui32 + right.ui32;
	break; case OPERAND_COLTI_UI64:		result.ui64 = left.ui64 + right.ui64;
	break; case OPERAND_COLTI_FLOAT:	result.f = left.f + right.f;
	break; case OPERAND_COLTI_DOUBLE:	result.d = left.d + right.d;
	break; default: colti_assert(false, "Invalid operand for OP_ADD!");
	}
	return result;
}

QWORD OpCode_Difference(QWORD left, QWORD right, OperandType type)
{
	QWORD result;
	switch (type)
	{
	break; case OPERAND_COLTI_I8:		result.i8 = left.i8 - right.i8;
	break; case OPERAND_COLTI_I16:		result.i16 = left.i16 - right.i16;
	break; case OPERAND_COLTI_I32:		result.i32 = left.i32 - right.i32;
	break; case OPERAND_COLTI_I64:		result.i64 = left.i64 - right.i64;
	break; case OPERAND_COLTI_UI8:		result.ui8 = left.ui8 - right.ui8;
	break; case OPERAND_COLTI_UI16:		result.ui16 = left.ui16 - right.ui16;
	break; case OPERAND_COLTI_UI32:		result.ui32 = left.ui32 - right.ui32;
	break; case OPERAND_COLTI_UI64:		result.ui64 = left.ui64 - right.ui64;
	break; case OPERAND_COLTI_FLOAT:	result.f = left.f - right.f;
	break; case OPERAND_COLTI_DOUBLE:	result.d = left.d - right.d;
	break; default: colti_assert(false, "Invalid operand for OP_ADD!");
	}
	return result;
}

QWORD OpCode_Multiply(QWORD left, QWORD right, OperandType type)
{
	QWORD result;
	switch (type)
	{
	break; case OPERAND_COLTI_I8:		result.i8 = left.i8 * right.i8;
	break; case OPERAND_COLTI_I16:		result.i16 = left.i16 * right.i16;
	break; case OPERAND_COLTI_I32:		result.i32 = left.i32 * right.i32;
	break; case OPERAND_COLTI_I64:		result.i64 = left.i64 * right.i64;
	break; case OPERAND_COLTI_UI8:		result.ui8 = left.ui8 * right.ui8;
	break; case OPERAND_COLTI_UI16:		result.ui16 = left.ui16 * right.ui16;
	break; case OPERAND_COLTI_UI32:		result.ui32 = left.ui32 * right.ui32;
	break; case OPERAND_COLTI_UI64:		result.ui64 = left.ui64 * right.ui64;
	break; case OPERAND_COLTI_FLOAT:	result.f = left.f * right.f;
	break; case OPERAND_COLTI_DOUBLE:	result.d = left.d * right.d;
	break; default: colti_assert(false, "Invalid operand for OP_ADD!");
	}
	return result;
}

QWORD OpCode_Divide(QWORD left, QWORD right, OperandType type)
{
	QWORD result;
	switch (type)
	{
	break; case OPERAND_COLTI_I8:		result.i8 = left.i8 / right.i8;
	break; case OPERAND_COLTI_I16:		result.i16 = left.i16 / right.i16;
	break; case OPERAND_COLTI_I32:		result.i32 = left.i32 / right.i32;
	break; case OPERAND_COLTI_I64:		result.i64 = left.i64 / right.i64;
	break; case OPERAND_COLTI_UI8:		result.ui8 = left.ui8 / right.ui8;
	break; case OPERAND_COLTI_UI16:		result.ui16 = left.ui16 / right.ui16;
	break; case OPERAND_COLTI_UI32:		result.ui32 = left.ui32 / right.ui32;
	break; case OPERAND_COLTI_UI64:		result.ui64 = left.ui64 / right.ui64;
	break; case OPERAND_COLTI_FLOAT:	result.f = left.f / right.f;
	break; case OPERAND_COLTI_DOUBLE:	result.d = left.d / right.d;
	break; default: colti_assert(false, "Invalid operand for OP_ADD!");
	}
	return result;
}

void OpCode_Print(QWORD value, OperandType type)
{
	switch (type)
	{
	break; case OPERAND_COLTI_BOOL:		printf("%s", value.b ? "true\n" : "false\n");
	break; case OPERAND_COLTI_I8:		printf("%"PRId8"\n", value.ui8);
	break; case OPERAND_COLTI_I16:		printf("%"PRId16"\n", value.ui16);
	break; case OPERAND_COLTI_I32:		printf("%"PRId32"\n", value.ui32);
	break; case OPERAND_COLTI_I64:		printf("%"PRId64"\n", value.ui64);
	break; case OPERAND_COLTI_UI8:		printf("%"PRIu8"\n", value.ui8);
	break; case OPERAND_COLTI_UI16:		printf("%"PRIu16"\n", value.ui16);
	break; case OPERAND_COLTI_UI32:		printf("%"PRIu32"\n", value.ui32);
	break; case OPERAND_COLTI_UI64:		printf("%"PRIu64"\n", value.ui64);
	break; case OPERAND_COLTI_FLOAT:	printf("%g\n", value.f);
	break; case OPERAND_COLTI_DOUBLE:	printf("%g\n", value.d);
	break; default: colti_assert(false, "Invalid operand for OP_PRINT!");
	}
}
