#include "disassemble.h"

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