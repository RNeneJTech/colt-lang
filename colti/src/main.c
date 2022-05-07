#include "precomph.h"

int main(int argc, const char** argv)
{
	printf("%s colti!%c", "Hello", '\n');
	Chunk chunk;
	chunkInit(&chunk);
	chunkWriteByte(&chunk, OP_RETURN);
	chunkWriteByte(&chunk, 1);
	chunkWriteByte(&chunk, 2);
	chunkWriteByte(&chunk, 3);
	chunkWriteByte(&chunk, 5);
	chunkPrintBytes(&chunk);
	chunkDisassemble(&chunk, "Test1");
	chunkFree(&chunk);
}