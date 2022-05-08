#include "precomph.h"

int main(int argc, const char** argv)
{
	printf("%s colti!%c", "Hello", '\n');
	Chunk chunk;
	ChunkInit(&chunk);
	//ChunkWriteByte(&chunk, OP_RETURN);
	ChunkWriteByte(&chunk, OP_IMMEDIATE_BYTE);
	ChunkWriteByte(&chunk, 2);
	ChunkWriteByte(&chunk, OP_IMMEDIATE_INT16);
	ChunkWriteInt16(&chunk, 10);
	ChunkWriteByte(&chunk, OP_RETURN);
	ChunkWriteByte(&chunk, OP_RETURN);
	ChunkWriteByte(&chunk, OP_IMMEDIATE_INT16);
	ChunkPrintBytes(&chunk);
	ChunkDisassemble(&chunk, "Test1");
	ChunkFree(&chunk);
}