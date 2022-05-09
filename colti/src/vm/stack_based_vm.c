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

		break; case OP_NEGATE_DOUBLE:
		{
			QWORD val = { .d= -StackVMPop(vm).d };
			StackVMPush(vm, val);
		}

		/******************************************************/

		break; case OP_PRINT_DOUBLE:
		{
			colti_assert(!StackVMIsEmpty(vm), "Stack was empty!");
			ColtiDouble val = StackVMPop(vm).d;
			printf("%f\n", val);
		}
		break; case OP_RETURN:
			return INTERPRET_OK;

		break; default:
			break;
		}
		ip += 1;
	}
}