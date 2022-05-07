#ifndef HG_COLTI_CHUNK
#define HG_COLTI_CHUNK

#include "common.h"

/// @brief Represents an instruction to be executed by the VM
typedef enum {
	OP_RETURN,
} OperationCode;

/// @brief Represents a stream of instructions
typedef struct {
	/// @brief Number of items pointed to
	uint32_t count;
	/// @brief Capacity of the current allocated code_buffer
	uint32_t capacity;

	/// @brief Pointer to the beginning of the byte-code
	uint8_t* code;
	/// @brief Small-code_buffer optimization for code
	uint8_t code_buffer[4];
} Chunk;

/// @brief Prints the byte content of a Chunk
/// @param chunk The chunk whose content to print
void chunkPrintBytes(Chunk* chunk);

/// @brief Prints a human readable description of the code contained in a chunk
/// @param chunk The chunk whose content to print
void chunkDisassemble(Chunk* chunk, const char* name);

/// @brief Zero-initializes a chunk
/// @param chunk The chunk to initialize
void chunkInit(Chunk* chunk);

/// @brief Appends a byte to the end of the chunk
/// @param chunk The chunk to append to
/// @param byte The byte to append
void chunkWriteByte(Chunk* chunk, uint8_t byte);

/// @brief Frees memory used by a chunk
/// @param chunk The chunk to free
void chunkFree(Chunk* chunk);

/**********************************
IMPLEMENTATION HELPERS
**********************************/

/// @brief Doubles the capacity of a chunk
/// @param chunk The chunk to modify
void impl_chunk_grow(Chunk* chunk);

int impl_chunk_print_code(Chunk* chunk, int offset);

int impl_print_simple_instruction(const char* name, int offset);

#endif //HG_COLTI_CHUNK