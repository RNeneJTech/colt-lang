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
	if (chunk->count == chunk->capacity) //Grow if needed
		impl_chunk_grow_double(chunk);
	chunk->code[chunk->count] = (uint8_t)code;
	++chunk->count;
}

void ChunkWriteByte(Chunk* chunk, BYTE byte)
{
	if (chunk->count == chunk->capacity) //Grow if needed
		impl_chunk_grow_double(chunk);
	chunk->code[chunk->count] = byte.ui8;
	++chunk->count;
}

void ChunkWriteBytes(Chunk* chunk, const uint8_t* const bytes, uint32_t size)
{
	if (!(chunk->count + size < chunk->capacity)) //Grow if needed
		impl_chunk_grow_size(chunk, size);
	memcpy(chunk->code, bytes, size);
	chunk->count += size;
}

void ChunkWriteWord(Chunk* chunk, WORD value)
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

void ChunkWriteDWord(Chunk* chunk, DWORD value)
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

void ChunkWriteQWord(Chunk* chunk, QWORD value)
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

BYTE ChunkGetByte(const Chunk* chunk, int* offset)
{
	colti_assert(chunk->code[*offset] == OP_IMMEDIATE_BYTE, "'offset' should point to an OP_IMMEDIATE_BYTE!");
	*offset += 1;
	BYTE byte = { .ui8 = chunk->code[(*offset)++] };
	return byte;
}

WORD ChunkGetWord(const Chunk* chunk, int* offset)
{
	colti_assert(chunk->code[*offset] == OP_IMMEDIATE_WORD, "'offset' should point to an OP_IMMEDIATE_WORD!");

	//Local variable which will be used to store a copy of the offset, than write only one time to *offset
	//As the offset points to OP_IMMEDIATE_WORD, we also need to add 1
	int local_offset = *offset + 1;
	//We add the padding to the offset, which means we are now pointing to the int16
	local_offset += ((uint64_t)(chunk->code + local_offset) & 1);

	//Extract the int16 from the bytes
	WORD return_val = { .ui16 = *(int16_t*)(chunk->code + local_offset) };
	//Update the value of the offset
	*offset = local_offset + sizeof(int16_t);
	return return_val;
}

DWORD ChunkGetDWord(const Chunk* chunk, int* offset)
{
	colti_assert(chunk->code[*offset] == OP_IMMEDIATE_DWORD, "'offset' should point to an OP_IMMEDIATE_DWORD!");

	//Local variable which will be used to store a copy of the offset, than write only one time to *offset
	//As the offset points to OP_IMMEDIATE_DWORD, we also need to add 1
	int local_offset = *offset + 1;
	//We add the padding to the offset, which means we are now pointing to the int32
	local_offset += (uint64_t)(chunk->code + local_offset) % 4;


	DWORD return_val;
	return_val.ui32 = *(int32_t*)(chunk->code + local_offset);
	//Update the value of the offset
	*offset = local_offset + sizeof(int32_t);
	return return_val;
}

QWORD ChunkGetQWord(const Chunk* chunk, int* offset)
{
	colti_assert(chunk->code[*offset] == OP_IMMEDIATE_QWORD, "'offset' should point to an OP_IMMEDIATE_QWORD!");

	//Local variable which will be used to store a copy of the offset, than write only one time to *offset
	//As the offset points to OP_IMMEDIATE_QWORD, we also need to add 1
	int local_offset = *offset + 1;
	//We add the padding to the offset, which means we are now pointing to the int64
	local_offset += (uint64_t)(chunk->code + local_offset) % 8;

	QWORD return_val;
	return_val.ui64 = *(int64_t*)(chunk->code + local_offset);
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

BYTE unsafe_get_byte(uint8_t** ptr)
{
	BYTE return_val;
	return_val.ui8 = *(*ptr);
	*ptr += sizeof(uint8_t);
	return return_val;
}

WORD unsafe_get_word(uint8_t** ptr)
{
	*ptr += (uint64_t)(*ptr) & 1; //read past padding
	WORD return_val;
	return_val.ui16 = *((uint16_t*)*ptr);
	*ptr += sizeof(int32_t);
	return return_val;
}

DWORD unsafe_get_dword(uint8_t** ptr)
{
	*ptr += (uint64_t)(*ptr) % 4; //read past padding
	DWORD return_val;
	return_val.ui32 = *((uint32_t*)*ptr);
	*ptr += sizeof(int32_t);
	return return_val;
}

QWORD unsafe_get_qword(uint8_t** ptr)
{
	*ptr += (uint64_t)(*ptr) % 8; //read past padding
	QWORD return_val;
	return_val.ui64 = *((uint64_t*)*ptr);
	*ptr += sizeof(int64_t);
	return return_val;
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
		/******************************************************/

	case OP_IMMEDIATE_BYTE:
		colti_assert(offset + 1 <= chunk->count, "Missing byte after OP_IMMEDIATE_BYTE!");
		impl_print_hex_instruction("OP_IMMEDIATE_BYTE", ChunkGetByte(chunk, &offset).ui8);
		return offset;

	case OP_IMMEDIATE_WORD:
		colti_assert(offset + ((uint64_t)(chunk->code + offset) & 1) + sizeof(int16_t) <= chunk->count, "Missing int16 after OP_IMMEDIATE_WORD");
		impl_print_hex_instruction("OP_IMMEDIATE_WORD", ChunkGetWord(chunk, &offset).ui16);
		return offset;

	case OP_IMMEDIATE_DWORD:
		colti_assert(offset + ((uint64_t)(chunk->code + offset) % 4) + sizeof(int32_t) <= chunk->count, "Missing int32 after OP_IMMEDIATE_DWORD");
		impl_print_hex_instruction("OP_IMMEDIATE_DWORD", ChunkGetDWord(chunk, &offset).ui32);
		return offset;

	case OP_IMMEDIATE_QWORD:
		colti_assert(offset + ((uint64_t)(chunk->code + offset) % 8) + sizeof(int64_t) <= chunk->count, "Missing int64 after OP_IMMEDIATE_QWORD");
		impl_print_hex_instruction("OP_IMMEDIATE_QWORD", ChunkGetQWord(chunk, &offset).ui64);
		return offset;

		/******************************************************/

	case OP_NEGATE:
		return impl_print_operand_instruction("OP_NEGATE", chunk->code[offset + 1], offset);

	case OP_SUM:
		return impl_print_operand_instruction("OP_SUM", chunk->code[offset + 1], offset);
	case OP_DIFFERENCE:
		return impl_print_operand_instruction("OP_DIFFERENCE", chunk->code[offset + 1], offset);
	case OP_MULTIPLY:
		return impl_print_operand_instruction("OP_MULTIPLY", chunk->code[offset + 1], offset);
	case OP_DIVIDE:
		return impl_print_operand_instruction("OP_DIVIDE", chunk->code[offset + 1], offset);

		/******************************************************/

	case OP_PRINT:
		return impl_print_operand_instruction("OP_PRINT", chunk->code[offset + 1], offset);

		/******************************************************/

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

int impl_print_byte_instruction(const char* name, uint8_t byte, int offset)
{
	printf("%s '%d'\n", name, byte);
	return offset + 2;
}

int impl_print_operand_instruction(const char* name, uint8_t byte, int offset)
{
	const char* operand;
	switch (byte)
	{
	break; case OPERAND_COLTI_BOOL:		operand = "BOOL";
	break; case OPERAND_COLTI_DOUBLE:	operand = "DOUBLE";
	break; case OPERAND_COLTI_FLOAT:	operand = "FLOAT";
	break; case OPERAND_COLTI_I8:		operand = "INT8";
	break; case OPERAND_COLTI_I16:		operand = "INT16";
	break; case OPERAND_COLTI_I32:		operand = "INT32";
	break; case OPERAND_COLTI_I64:		operand = "INT64";
	break; case OPERAND_COLTI_UI8:		operand = "UINT8";
	break; case OPERAND_COLTI_UI16:		operand = "UINT16";
	break; case OPERAND_COLTI_UI32:		operand = "UINT32";
	break; case OPERAND_COLTI_UI64:		operand = "UINT64";
	break; default:						operand = "UNKOWN";
	}
	printf("%s '%s'\n", name, operand);
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