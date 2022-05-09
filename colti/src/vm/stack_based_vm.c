#include "stack_based_vm.h"

void StackVMInit(StackVM* vm)
{	
	//Point to index 0 of the stack (which means empty)
	vm->stack_top = vm->stack;
}

void StackVMFree(StackVM* vm)
{
	
}

void StackVMPush(StackVM* vm, QWORD value)
{
	*vm->stack_top = value;
	vm->stack_top++;
}

QWORD StackVMTop(StackVM* vm)
{
	colti_assert(!StackVMIsEmpty(vm), "Stack was empty!");
	return *(vm->stack_top - 1);
}

QWORD StackVMPop(StackVM* vm)
{
	return *(--vm->stack_top);
}

bool StackVMIsEmpty(StackVM* vm)
{
	return vm->stack_top == vm->stack;
}

InterpretResult StackVMRun(StackVM* vm, Chunk* chunk)
{
	uint8_t* ip = chunk->code;
	for (;;)
	{
		switch (*ip) //Dereferences then advances the pointer
		{
		
		/******************************************************/

		break; case OP_IMMEDIATE_BYTE:
		{
			QWORD qword = { .byte = unsafe_get_byte(&ip) };
			StackVMPush(vm, qword);
		}
		break; case OP_IMMEDIATE_WORD:
		{
			QWORD qword = { .word = unsafe_get_word(&ip) };
			StackVMPush(vm, qword);
		}
		break; case OP_IMMEDIATE_DWORD:
		{
			QWORD qword = { .word = unsafe_get_word(&ip) };
			StackVMPush(vm, qword);
		}
		break; case OP_IMMEDIATE_QWORD:
		{
			QWORD qword = unsafe_get_qword(&ip);
			StackVMPush(vm, qword);
		}
		
		/******************************************************/

		break; case OP_NEGATE:
		{
			QWORD val = StackVMPop(vm);
			switch (*(++ip))
			{
			break; case OPERAND_COLTI_DOUBLE:	val.d = -val.d;
			break; case OPERAND_COLTI_FLOAT:	val.f = -val.f;
			break; case OPERAND_COLTI_I8:		val.i8 = -val.i8;
			break; case OPERAND_COLTI_I16:		val.i16 = -val.i16;
			break; case OPERAND_COLTI_I32:		val.i32 = -val.i32;
			break; case OPERAND_COLTI_I64:		val.i64 = -val.i64;
			break; default: colti_assert(false, "Invalid operand for OP_NEGATE!");
			}
			StackVMPush(vm, val);
		}		

		/******************************************************/
		
		break; case OP_PRINT:
		{
			colti_assert(!StackVMIsEmpty(vm), "Stack was empty!");
			QWORD val = StackVMTop(vm);
			switch (*(++ip))
			{
			break; case OPERAND_COLTI_BOOL:		printf(val.b ? "true\n" : "false\n");
			break; case OPERAND_COLTI_I8:		printf("%"PRId8"\n", val.ui8);
			break; case OPERAND_COLTI_I16:		printf("%"PRId16"\n", val.ui16);
			break; case OPERAND_COLTI_I32:		printf("%"PRId32"\n", val.ui32);
			break; case OPERAND_COLTI_I64:		printf("%"PRId64"\n", val.ui64);
			break; case OPERAND_COLTI_UI8:		printf("%"PRIu8"\n", val.ui8);
			break; case OPERAND_COLTI_UI16:		printf("%"PRIu16"\n", val.ui16);
			break; case OPERAND_COLTI_UI32:		printf("%"PRIu32"\n", val.ui32);
			break; case OPERAND_COLTI_UI64:		printf("%"PRIu64"\n", val.ui64);
			break; case OPERAND_COLTI_FLOAT:	printf("%f\n", val.f);
			break; case OPERAND_COLTI_DOUBLE:	printf("%f\n", val.d);
			break; default: colti_assert(false, "Invalid operand for OP_PRINT!");
			}
		}
		break; case OP_RETURN:
			return INTERPRET_OK;

		break; default:
			break;
		}
		ip += 1;
	}
}