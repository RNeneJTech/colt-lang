#include "precomph.h"
#include "vm/stack_based_vm.h"

int chunk_and_vm(int argc, char** argv)
{
	printf(CONSOLE_BACKGROUND_BRIGHT_MAGENTA CONSOLE_FOREGROUND_BLACK
		"COLTI v%s on %s" CONSOLE_COLOR_RESET "\n", COLTI_VERSION_STRING, COLTI_OS_STRING);
	printf("Test: "CONSOLE_COLOR_BRIGHT_CYAN"%s\n"CONSOLE_COLOR_RESET, COLTI_CURRENT_FILENAME);

	Chunk chunk;
	ChunkInit(&chunk);
	ChunkWriteOpCode(&chunk, OP_IMMEDIATE_QWORD);
	QWORD qword = { .i64 = -15 };
	ChunkWriteQWORD(&chunk, qword);

	ChunkWriteOpCode(&chunk, OP_NEGATE);
	ChunkWriteOperand(&chunk, COLTI_INT64);

	ChunkWriteOpCode(&chunk, OP_PRINT);
	ChunkWriteOperand(&chunk, COLTI_INT64);


	ChunkWriteOpCode(&chunk, OP_IMMEDIATE_QWORD);
	QWORD qword2 = { .i64 = 15 };
	ChunkWriteQWORD(&chunk, qword2);

	ChunkWriteOpCode(&chunk, OP_PRINT);
	ChunkWriteOperand(&chunk, COLTI_INT64);

	ChunkWriteOpCode(&chunk, OP_DIVIDE);
	ChunkWriteOperand(&chunk, COLTI_INT64);

	ChunkWriteOpCode(&chunk, OP_PRINT);
	ChunkWriteOperand(&chunk, COLTI_INT64);

	ChunkWriteOpCode(&chunk, OP_RETURN);
	ChunkDisassemble(&chunk, "Test1");

	if (ChunkIsStackAllocated(&chunk))
		printf("Chunk is stack-allocated!\n");
	else
		printf("Chunk is heap-allocated!\n");

	StackVM vm;
	StackVMInit(&vm);
	StackVMRun(&vm, &chunk);

	StackVMFree(&vm);
	ChunkFree(&chunk);
}