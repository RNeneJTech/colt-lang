#include "precomph.h"

int main(int argc, const char** argv)
{
	printf(CONSOLE_BACKGROUND_BRIGHT_MAGENTA CONSOLE_FOREGROUND_BLACK
		"COLTI v%s on %s" CONSOLE_COLOR_RESET "\n", COLTI_VERSION_STRING, COLTI_OS_STRING);
	Chunk chunk;
	ChunkInit(&chunk);
	ChunkWriteByte(&chunk, OP_RETURN);
	ChunkWriteByte(&chunk, OP_IMMEDIATE_BYTE);
	ChunkWriteByte(&chunk, 2);
	ChunkWriteByte(&chunk, OP_IMMEDIATE_WORD);
	ChunkWriteWord(&chunk, 10);
	ChunkWriteByte(&chunk, OP_RETURN);
	ChunkWriteByte(&chunk, OP_RETURN);
	ChunkWriteByte(&chunk, OP_IMMEDIATE_WORD);
	ChunkWriteWord(&chunk, -10);
	ChunkWriteByte(&chunk, OP_IMMEDIATE_DWORD);
	ChunkWriteDWord(&chunk, 256);
	ChunkWriteByte(&chunk, OP_RETURN);
	ChunkWriteByte(&chunk, OP_IMMEDIATE_QWORD);
	ChunkWriteQWord(&chunk, 512);
	ChunkPrintBytes(&chunk);
	ChunkDisassemble(&chunk, "Test1");
	ChunkFree(&chunk);
}