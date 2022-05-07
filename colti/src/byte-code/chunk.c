#include "chunk.h"

void chunkPrintBytes(Chunk* chunk)
{
	//TODO: maybe optimize?
	for (uint32_t i = 0; i < chunk->count; i++)
	{
		if (i > 0) printf(":");
		printf("%02X", chunk->code[i]);
	}
	printf("\n");
}

void chunkDisassemble(Chunk* chunk, const char* name)
{
	printf("==== %s ====\n", name);

	if (chunk->count == 0)
	{
		printf("!EMPTY CHUNK!");
		return;
	}
	for (int offset = 0; offset < chunk->count;)
	{
		offset = impl_chunk_print_code(chunk, offset);
	}
}

void chunkInit(Chunk* chunk)
{
	chunk->capacity = 4;
	chunk->count = 0;
	chunk->code = chunk->buffer;
}

void chunkWriteByte(Chunk* chunk, uint8_t byte)
{
	if (chunk->count == chunk->capacity) //Grow if needed
		impl_chunk_grow(chunk);
	chunk->code[chunk->count] = byte;
	++chunk->count;
}

void chunkFree(Chunk* chunk)
{
	//As chunks use a small-code_buffer optimization, we make sure
	//not to free the stack code_buffer they contain
	if (chunk->capacity != 4)
		safe_free(chunk->code);

	//Most functions that take a Chunk* check for if the capacity is 0,
	//which should never be.
	//By setting it to 0, we ensure that calling a function on the freed chunk
	//will cause an assertion on debug builds.
	DO_IF_DEBUG_BUILD(chunk->capacity = 0);
}

void impl_chunk_grow(Chunk* chunk)
{
	colti_assert(chunk->capacity != 0, "Chunk capacity was 0! Be sure to call chunkInit for any Chunk you create!");

	//Allocate double the capacity
	uint8_t* ptr = (uint8_t*)safe_malloc(chunk->capacity *= 2);		
	//Copy byte-code to new location
	memcpy(ptr, chunk->code, chunk->count);

	//As chunks use a small-code_buffer optimization, we make sure
	//not to free the stack code_buffer they contain
	if (chunk->capacity != 8) //as we * 2 the capacity, and the stack code_buffer is 4
		safe_free(chunk->code);
	chunk->code = ptr;
}

int impl_chunk_print_code(Chunk* chunk, int offset)
{
	printf("%04d ", offset);

	uint8_t instruction = chunk->code[offset];
	switch (instruction) {
	case OP_RETURN:
		return impl_print_simple_instruction("OP_RETURN", offset);
	default:
		printf("UNKOWN OPCODE: '%d'\n", instruction);
		return offset + 1;
	}
}

int impl_print_simple_instruction(const char* name, int offset)
{
	printf("%s\n", name);
	return offset + 1;
}