#include "precomph.h"
#include "vm/stack_based_vm.h"

int main(int argc, const char** argv)
{
	printf(CONSOLE_BACKGROUND_BRIGHT_MAGENTA CONSOLE_FOREGROUND_BLACK
		"COLTI v%s on %s" CONSOLE_COLOR_RESET "\n", COLTI_VERSION_STRING, COLTI_OS_STRING);
	Chunk chunk;
	ChunkInit(&chunk);
	ChunkWriteOpCode(&chunk, OP_IMMEDIATE_DWORD);
	DWORD qword = { .f = 1.5 };
	ChunkWriteDWORD(&chunk, qword);

	ChunkWriteOpCode(&chunk, OP_NEGATE);
	ChunkWriteOperand(&chunk, COLTI_FLOAT);

	ChunkWriteOpCode(&chunk, OP_PRINT);
	ChunkWriteOperand(&chunk, COLTI_FLOAT);


	ChunkWriteOpCode(&chunk, OP_IMMEDIATE_DWORD);
	DWORD qword2 = { .f = 1.5 };
	ChunkWriteDWORD(&chunk, qword2);

	ChunkWriteOpCode(&chunk, OP_PRINT);
	ChunkWriteOperand(&chunk, COLTI_FLOAT);

	ChunkWriteOpCode(&chunk, OP_DIVIDE);
	ChunkWriteOperand(&chunk, COLTI_FLOAT);

	ChunkWriteOpCode(&chunk, OP_PRINT);
	ChunkWriteOperand(&chunk, COLTI_FLOAT);

	ChunkWriteOpCode(&chunk, OP_RETURN);
	ChunkDisassemble(&chunk, "Test1");

	StackVM vm;
	StackVMInit(&vm);
	StackVMRun(&vm, &chunk);

	StackVMFree(&vm);
	ChunkFree(&chunk);
}