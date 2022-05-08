#include "chunk.h"

void ChunkPrintBytes(const Chunk* chunk)
{
	//TODO: maybe optimize?
	for (uint32_t i = 0; i < chunk->count; i++)
	{
		if (i > 0) printf(" ");
		printf("%02X", chunk->code[i]);
	}
	printf("\n");
}

void ChunkDisassemble(const Chunk* chunk, const char* name)
{
	printf("============ %s ============\n", name);

	if (chunk->count == 0)
	{
		printf("!EMPTY CHUNK!");
		return;
	}
	for (int offset = 0; (uint64_t)offset < chunk->count;)
	{
		offset = impl_chunk_print_code(chunk, offset);
	}
}

void ChunkInit(Chunk* chunk)
{
	chunk->capacity = 8;
	chunk->count = 0;
	chunk->code = chunk->code_buffer;
}

void ChunkWriteOpCode(Chunk* chunk, OpCode code)
{
	ChunkWriteByte(chunk, code);
}

void ChunkWriteByte(Chunk* chunk, uint8_t byte)
{
	if (chunk->count == chunk->capacity) //Grow if needed
		impl_chunk_grow_double(chunk);
	chunk->code[chunk->count] = byte;
	++chunk->count;
}

void ChunkWriteBytes(Chunk* chunk, const uint8_t* const bytes, uint32_t size)
{
	if (!(chunk->count + size < chunk->capacity)) //Grow if needed
		impl_chunk_grow_size(chunk, size);
	memcpy(chunk->code, bytes, size);
	chunk->count += size;
}

void ChunkWriteWord(Chunk* chunk, uint16_t value)
{
	//We need to pad if needed
	uint64_t offset = (uint64_t)(chunk->code + chunk->count) & 1; //same as % 2
	if (!(chunk->count + offset + sizeof(uint16_t) < chunk->capacity)) //Grow if needed
		impl_chunk_grow_double(chunk);

	//Set the padding byte to CD on Debug build
	DO_IF_DEBUG_BUILD(if (offset != 0) chunk->code[chunk->count] = 205;);

	//Copy the bytes of the integer to the aligned memory
	memcpy(chunk->code + (chunk->count += offset), &value, sizeof(uint16_t));
	//We already added offset to 'count' ^
	chunk->count += sizeof(uint16_t);
}

void ChunkWriteDWord(Chunk* chunk, uint32_t value)
{
	uint64_t offset = (uint64_t)(chunk->code + chunk->count) % 4;
	if (!(chunk->count + offset + sizeof(uint32_t) < chunk->capacity)) //Grow if needed
		impl_chunk_grow_double(chunk);

	//Set the padding bytes to CD on Debug build
	DO_IF_DEBUG_BUILD(if (offset != 0) memset(chunk->code + chunk->count, 205, offset););

	//Copy the bytes of the integer to the aligned memory
	memcpy(chunk->code + (chunk->count += offset), &value, sizeof(uint32_t));
	//We already added offset to 'count' ^
	chunk->count += sizeof(uint32_t);
}

void ChunkWriteQWord(Chunk* chunk, uint64_t value)
{	
	uint64_t offset = (uint64_t)(chunk->code + chunk->count) % 8;
	if (!(chunk->count + offset + sizeof(uint64_t) < chunk->capacity)) //Grow if needed
		impl_chunk_grow_double(chunk);

	//Set the padding bytes to CD on Debug build
	DO_IF_DEBUG_BUILD(if (offset != 0) memset(chunk->code + chunk->count, 205, offset););

	//Copy the bytes of the integer to the aligned memory
	memcpy(chunk->code + (chunk->count += offset), &value, sizeof(uint64_t));
	//We already added offset to 'count' ^
	chunk->count += sizeof(uint64_t);
}

uint16_t ChunkGetByte(const Chunk* chunk, int* offset)
{
	colti_assert(chunk->code[*offset] == OP_IMMEDIATE_BYTE, "'offset' should point to an OP_IMMEDIATE_BYTE!");
	*offset += 1;
	return chunk->code[(*offset)++];
}

uint16_t ChunkGetWord(const Chunk* chunk, int* offset)
{
	colti_assert(chunk->code[*offset] == OP_IMMEDIATE_WORD, "'offset' should point to an OP_IMMEDIATE_WORD!");

	//Local variable which will be used to store a copy of the offset, than write only one time to *offset
	//As the offset points to OP_IMMEDIATE_WORD, we also need to add 1
	int local_offset = *offset + 1;
	//We add the padding to the offset, which means we are now pointing to the int16
	local_offset += ((uint64_t)(chunk->code + local_offset) & 1);

	//Extract the int16 from the bytes
	uint16_t return_val = *(int16_t*)(chunk->code + local_offset);
	//Update the value of the offset
	*offset = local_offset + sizeof(int16_t);
	return return_val;
}

uint32_t ChunkGetDWord(const Chunk* chunk, int* offset)
{
	colti_assert(chunk->code[*offset] == OP_IMMEDIATE_DWORD, "'offset' should point to an OP_IMMEDIATE_DWORD!");

	//Local variable which will be used to store a copy of the offset, than write only one time to *offset
	//As the offset points to OP_IMMEDIATE_DWORD, we also need to add 1
	int local_offset = *offset + 1;
	//We add the padding to the offset, which means we are now pointing to the int32
	local_offset += (uint64_t)(chunk->code + local_offset) % 4;

	//Extract the int16 from the bytes
	uint32_t return_val = *(int32_t*)(chunk->code + local_offset);
	//Update the value of the offset
	*offset = local_offset + sizeof(int32_t);
	return return_val;
}

uint64_t ChunkGetQWord(const Chunk* chunk, int* offset)
{
	colti_assert(chunk->code[*offset] == OP_IMMEDIATE_QWORD, "'offset' should point to an OP_IMMEDIATE_QWORD!");

	//Local variable which will be used to store a copy of the offset, than write only one time to *offset
	//As the offset points to OP_IMMEDIATE_QWORD, we also need to add 1
	int local_offset = *offset + 1;
	//We add the padding to the offset, which means we are now pointing to the int64
	local_offset += (uint64_t)(chunk->code + local_offset) % 8;

	//Extract the int16 from the bytes
	uint64_t return_val = *(int64_t*)(chunk->code + local_offset);
	//Update the value of the offset
	*offset = local_offset + sizeof(int64_t);
	return return_val;
}

void ChunkFree(Chunk* chunk)
{
	//As chunks use a small-code_buffer optimization, we make sure
	//not to free the stack code_buffer they contain
	if (chunk->capacity != 8)
		safe_free(chunk->code);

	//Most functions that take a Chunk* check for if the capacity is 0,
	//which should never be.
	//By setting it to 0, we ensure that calling a function on the freed chunk
	//will cause an assertion on debug builds.
	DO_IF_DEBUG_BUILD(chunk->capacity = 0);
}

void impl_chunk_grow_double(Chunk* chunk)
{
	colti_assert(chunk->capacity != 0, "Chunk capacity was 0! Be sure to call ChunkInit for any Chunk you create!");

	//Allocate double the capacity
	uint8_t* ptr = (uint8_t*)safe_malloc(chunk->capacity *= 2);
	//Copy byte-code to new location
	memcpy(ptr, chunk->code, chunk->count);

	//As chunks use a small-code_buffer optimization, we make sure
	//not to free the stack code_buffer they contain
	if (chunk->capacity != 16) //as we * 2 the capacity, and the stack code_buffer is 8
		safe_free(chunk->code);
	chunk->code = ptr;
}

void impl_chunk_grow_size(Chunk* chunk, uint32_t size)
{
	colti_assert(size != 0, "Tried to augment the capacity of a Chunk by 0!");
	colti_assert(chunk->capacity != 0, "Chunk capacity was 0! Be sure to call ChunkInit for any Chunk you create!");

	//Allocate the new capacity
	uint8_t* ptr = (uint8_t*)safe_malloc(chunk->capacity += size);
	//Copy byte-code to new location
	memcpy(ptr, chunk->code, chunk->count);

	//As chunks use a small-code_buffer optimization, we make sure
	//not to free the stack code_buffer they contain
	if (chunk->capacity != 8 + size) //as we added 'size' the capacity, and the stack code_buffer is 8
		safe_free(chunk->code);
	chunk->code = ptr;
}

int impl_chunk_print_code(const Chunk* chunk, int offset)
{
	printf("%04d ", offset);

	uint8_t instruction = chunk->code[offset];
	switch (instruction)
	{
	case OP_RETURN:
		return impl_print_simple_instruction("OP_RETURN", offset);

	case OP_IMMEDIATE_BYTE:
		colti_assert(offset + 1 <= chunk->count, "Missing byte after OP_IMMEDIATE_BYTE!");
		impl_print_hex_instruction("OP_IMMEDIATE_BYTE", ChunkGetByte(chunk, &offset));
		return offset;

	case OP_IMMEDIATE_WORD:
		colti_assert(offset + ((uint64_t)(chunk->code + offset) & 1) + sizeof(int16_t) <= chunk->count, "Missing int16 after OP_IMMEDIATE_WORD");
		impl_print_hex_instruction("OP_IMMEDIATE_WORD", ChunkGetWord(chunk, &offset));
		return offset;

	case OP_IMMEDIATE_DWORD:
		colti_assert(offset + ((uint64_t)(chunk->code + offset) % 4) + sizeof(int32_t) <= chunk->count, "Missing int32 after OP_IMMEDIATE_DWORD");
		impl_print_hex_instruction("OP_IMMEDIATE_DWORD", ChunkGetDWord(chunk, &offset));
		return offset;

	case OP_IMMEDIATE_QWORD:
		colti_assert(offset + ((uint64_t)(chunk->code + offset) % 8) + sizeof(int64_t) <= chunk->count, "Missing int64 after OP_IMMEDIATE_QWORD");
		impl_print_hex_instruction("OP_IMMEDIATE_QWORD", ChunkGetQWord(chunk, &offset));
		return offset;

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

int impl_print_byte_instruction(const char* name, uint8_t byte, int offset)
{
	printf("%s '%d'\n", name, byte);
	return offset + 2;
}

void impl_print_int_instruction(const char* name, int64_t value)
{
	printf("%s '%"PRId64"'", name, value);
}

void impl_print_hex_instruction(const char* name, uint64_t value)
{
	printf("%s '0x%"PRIX64"'\n", name, value);
}