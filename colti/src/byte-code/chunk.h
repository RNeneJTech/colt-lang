#ifndef HG_COLTI_CHUNK
#define HG_COLTI_CHUNK

#include "common.h"

/// @brief Represents an instruction to be executed by the VM
typedef enum {
	OP_IMMEDIATE_BYTE,
	OP_IMMEDIATE_INT16,
	OP_IMMEDIATE_INT32,
	OP_IMMEDIATE_INT64,
	OP_RETURN,
} OperationCode;

/// @brief Represents a stream of instructions
typedef struct {
	/// @brief Number of items pointed to
	uint64_t count;
	/// @brief Capacity of the current allocated code_buffer
	uint64_t capacity;

	/// @brief Pointer to the beginning of the byte-code
	uint8_t* code;
	/// @brief Small-code_buffer optimization for code
	uint8_t code_buffer[8];
} Chunk;

/// @brief Prints the byte content of a Chunk
/// @param chunk The chunk whose content to print
void ChunkPrintBytes(const Chunk* chunk);

/// @brief Prints a human readable description of the code contained in a chunk
/// @param chunk The chunk whose content to print
void ChunkDisassemble(const Chunk* chunk, const char* name);

/// @brief Zero-initializes a chunk
/// @param chunk The chunk to initialize
void ChunkInit(Chunk* chunk);

/// @brief Appends a byte to the end of the chunk
/// @param chunk The chunk to append to
/// @param byte The byte to append
void ChunkWriteByte(Chunk* chunk, uint8_t byte);

/// @brief Appends multiple bytes to the end of a chunk
/// @param chunk The chunk to append to
/// @param bytes The array of bytes from which to copy the bytes
/// @param size The number of bytes to copy from 'bytes'
void ChunkWriteBytes(Chunk* chunk, const uint8_t* const bytes, uint32_t size);

/// @brief Writes an int16 to the end of a chunk, padding if necessary
/// @param chunk The chunk to append to
/// @param value The value to write
void ChunkWriteInt16(Chunk* chunk, int16_t value);

/// @brief Writes an int32 to the end of a chunk, padding if necessary
/// @param chunk The chunk to append to
/// @param value The value to write
void ChunkWriteInt32(Chunk* chunk, int32_t value);

/// @brief Writes an int64 to the end of a chunk, padding if necessary
/// @param chunk The chunk to append to
/// @param value The value to write
void ChunkWriteInt64(Chunk* chunk, int64_t value);

/// @brief Gets an int16 from the offset specified, aligning the access
/// @param chunk The chunk to get the value from
/// @param offset The offset should point to the OP_IMMEDIATE_INT16, is modified by this function
/// @return The int at that offset
int16_t ChunkGetInt16(const Chunk* chunk, int* offset);

/// @brief Gets an int32 from the offset specified, aligning the access
/// @param chunk The chunk to get the value from
/// @param offset The offset should point to the OP_IMMEDIATE_INT32
/// @return The int at that offset
int32_t ChunkGetInt32(const Chunk* chunk, int* offset);

/// @brief Gets an int64 from the offset specified, aligning the access
/// @param chunk The chunk to get the value from
/// @param offset The offset should point to the OP_IMMEDIATE_INT64
/// @return The int at that offset
int64_t ChunkGetInt64(const Chunk* chunk, int* offset);

/// @brief Frees memory used by a chunk
/// @param chunk The chunk to free
void ChunkFree(Chunk* chunk);

/**********************************
IMPLEMENTATION HELPERS
**********************************/

/// @brief Doubles the capacity of a chunk
/// @param chunk The chunk to modify
void impl_chunk_grow_double(Chunk* chunk);

/// @brief Augments the capacity of a chunk by 'size'
/// @param chunk The chunk to modify
/// @param size The capacity to add
void impl_chunk_grow_size(Chunk* chunk, uint32_t size);

/// @brief Dispatches a code to the correct printing function
/// @param chunk The chunk from which to extract the code
/// @param offset The offset to the code
/// @return Modified offset
int impl_chunk_print_code(const Chunk* chunk, int offset);

/// @brief Prints a one byte instruction
/// @param name The name of the instruction
/// @param offset The current byte offset
/// @return The current byte offset + 1
int impl_print_simple_instruction(const char* name, int offset);

int impl_print_byte_instruction(const char* name, uint8_t byte, int offset);

void impl_print_int_instruction(const char* name, int64_t value);

#endif //HG_COLTI_CHUNK