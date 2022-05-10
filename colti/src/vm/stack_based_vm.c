#include "stack_based_vm.h"

void StackVMInit(StackVM* vm)
{
	//Point to index 0 of the stack (which means empty)
	vm->stack_top = vm->stack;
}

void StackVMFree(StackVM* vm)
{
	//Nothing to free
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

uint64_t StackVMSize(StackVM* vm)
{
	return vm->stack_top - vm->stack;
}

InterpretResult StackVMRun(StackVM* vm, Chunk* chunk)
{
	uint8_t* ip = chunk->code;
	for (;;)
	{
		switch (*(ip++)) //Dereferences then advances the pointer
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
			QWORD qword = { .dword = unsafe_get_dword(&ip) };
			StackVMPush(vm, qword);
		}
		break; case OP_IMMEDIATE_QWORD:
		{
			QWORD qword = unsafe_get_qword(&ip);
			StackVMPush(vm, qword);
		}

		/******************************************************/

		break; case OP_NEGATE:
			colti_assert(!StackVMIsEmpty(vm), "Stack should contain at least 1 items!");
			StackVMPush(vm, OpCode_Negate(StackVMPop(vm), *(ip++)));

		break; case OP_CONVERT:
		{
			colti_assert(!StackVMIsEmpty(vm), "Stack should contain at least 1 items!");
			OperandType from = *(ip++);
			OperandType to = *(ip++);
			StackVMPush(vm, OpCode_Convert(StackVMPop(vm), from, to));
		}

		/******************************************************/

		break; case OP_ADD:
		{
			colti_assert(StackVMSize(vm) >= 2, "Stack should contain at least 2 items!");
			QWORD val1 = StackVMPop(vm);
			QWORD val2 = StackVMPop(vm);			
			StackVMPush(vm, OpCode_Sum(val1, val2, *(ip++)));
		}
		break; case OP_SUBTRACT:
		{
			colti_assert(StackVMSize(vm) >= 2, "Stack should contain at least 2 items!");
			QWORD val1 = StackVMPop(vm);
			QWORD val2 = StackVMPop(vm);
			StackVMPush(vm, OpCode_Difference(val1, val2, *(ip++)));
		}
		break; case OP_MULTIPLY:
		{
			colti_assert(StackVMSize(vm) >= 2, "Stack should contain at least 2 items!");
			QWORD val1 = StackVMPop(vm);
			QWORD val2 = StackVMPop(vm);
			StackVMPush(vm, OpCode_Multiply(val1, val2, *(ip++)));
		}
		break; case OP_DIVIDE:
		{
			colti_assert(StackVMSize(vm) >= 2, "Stack should contain at least 2 items!");
			QWORD val1 = StackVMPop(vm);
			QWORD val2 = StackVMPop(vm);
			StackVMPush(vm, OpCode_Divide(val1, val2, *(ip++)));
		}
		/******************************************************/

		break; case OP_PRINT:
		{
			colti_assert(!StackVMIsEmpty(vm), "Stack was empty!");
			OpCode_Print(StackVMTop(vm), *(ip++));			
		}
		break; case OP_RETURN:
			return INTERPRET_OK;

		break; default:
			break;
		}
	}
}