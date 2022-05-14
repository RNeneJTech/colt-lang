#include "precomph.h"
#include "vm/stack_based_vm.h"

int deserialize()
{
	printf(CONSOLE_BACKGROUND_BRIGHT_MAGENTA CONSOLE_FOREGROUND_BLACK
		"COLTI v%s on %s" CONSOLE_COLOR_RESET "\n", COLTI_VERSION_STRING, COLTI_OS_STRING);
	printf("Test: "CONSOLE_COLOR_BRIGHT_CYAN"%s\n"CONSOLE_COLOR_RESET, COLTI_CURRENT_FILENAME);

	Chunk to_serialize;
	ChunkInit(&to_serialize);
	ChunkWriteOpCode(&to_serialize, OP_IMMEDIATE_QWORD);
	QWORD qword = { .i64 = -15 };
	ChunkWriteQWORD(&to_serialize, qword);

	ChunkWriteOpCode(&to_serialize, OP_NEGATE);
	ChunkWriteOperand(&to_serialize, COLTI_INT64);

	ChunkWriteOpCode(&to_serialize, OP_PRINT);
	ChunkWriteOperand(&to_serialize, COLTI_INT64);


	ChunkWriteOpCode(&to_serialize, OP_IMMEDIATE_QWORD);
	QWORD qword2 = { .i64 = 15 };
	ChunkWriteQWORD(&to_serialize, qword2);

	ChunkWriteOpCode(&to_serialize, OP_PRINT);
	ChunkWriteOperand(&to_serialize, COLTI_INT64);

	ChunkWriteOpCode(&to_serialize, OP_DIVIDE);
	ChunkWriteOperand(&to_serialize, COLTI_INT64);

	ChunkWriteOpCode(&to_serialize, OP_PRINT);
	ChunkWriteOperand(&to_serialize, COLTI_INT64);

	ChunkWriteOpCode(&to_serialize, OP_RETURN);
	ChunkDisassemble(&to_serialize, "Test1");

	ChunkSerialize(&to_serialize, "chunk.bin");
	Chunk deserialized = ChunkDeserialize("chunk.bin");

	StackVM vm;
	StackVMInit(&vm);
	StackVMRun(&vm, &to_serialize);
	StackVMRun(&vm, &deserialized);

	StackVMFree(&vm);
	ChunkFree(&to_serialize);
	ChunkFree(&deserialized);

	return 0;
}