/** @file chunk.h
* Contains the Chunk struct, which is used to provide simpler operations for storing and writing byte-code.
* A Chunk can be seen as a contiguous dynamic array which handles resizes automatically
* when writing byte-code to it.
* While some functions might seem redundant (ChunkWriteOpCode, ChunkWriteOperand),
* they provide a bit of type safety (as C's type system can be summarized with warnings)
* over just appending a byte at the end of the Chunk's code.
* The ChunkWrite(BYTE|[DQ]?WORD) aligns the writing of the value to its required alignment,
* which is also the reason for which to use ChunkGet(BYTE|[DQ]?WORD).
* These take a pointer to an int representing the offset, as it's the functions responsibility
* to update that offset.
* The unsafe_get_... are used for when a pointer is used rather than an offset.
*/

#ifndef HG_COLTI_CHUNK
#define HG_COLTI_CHUNK

#include "common.h"
#include "byte_code.h" //Contains the byte-code enum

#define CHUNK_SMALL_BUFFER_OPTIMIZATION 64

/// @brief Represents a stream of instructions
typedef struct
{
	/// @brief Number of items pointed to
	uint64_t count;
	/// @brief Capacity of the current allocated code_buffer
	uint64_t capacity;

	/// @brief Pointer to the beginning of the byte-code
	uint8_t* code;
	/// @brief Small-code_buffer optimization for code
	uint8_t code_buffer[CHUNK_SMALL_BUFFER_OPTIMIZATION];
} Chunk;

/// @brief Prints the byte content of a Chunk
/// @param chunk The chunk whose content to print
void ChunkPrintBytes(const Chunk* chunk);

/// @brief Zero-initializes a chunk
/// @param chunk The chunk to initialize
void ChunkInit(Chunk* chunk);

/// @brief Appends an OpCode to the end of the chunk
/// @param chunk The chunk to append to
/// @param code The byte to append
void ChunkWriteOpCode(Chunk* chunk, OpCode code);

/// @brief Appends an OperandType to the end of the chunk
/// @param chunk The chunk to append to
/// @param type The type to append
void ChunkWriteOperand(Chunk* chunk, OperandType type);

/// @brief Appends a byte to the end of the chunk
/// @param chunk The chunk to append to
/// @param byte The byte to append
void ChunkWriteBYTE(Chunk* chunk, BYTE byte);

/// @brief Appends multiple bytes to the end of a chunk
/// @param chunk The chunk to append to
/// @param bytes The array of bytes from which to copy the bytes
/// @param size The number of bytes to copy from 'bytes'
void ChunkWriteBytes(Chunk* chunk, const uint8_t* const bytes, uint32_t size);

/// @brief Writes an int16 to the end of a chunk, padding if necessary
/// @param chunk The chunk to append to
/// @param value The value to write
void ChunkWriteWORD(Chunk* chunk, WORD value);

/// @brief Writes an int32 to the end of a chunk, padding if necessary
/// @param chunk The chunk to append to
/// @param value The value to write
void ChunkWriteDWORD(Chunk* chunk, DWORD value);

/// @brief Writes an int64 to the end of a chunk, padding if necessary
/// @param chunk The chunk to append to
/// @param value The value to write
void ChunkWriteQWORD(Chunk* chunk, QWORD value);

/// @brief Gets a byte from the offset specified
/// @param chunk The chunk to get the value from
/// @param offset The offset should point to the OP_IMMEDIATE_BYTE, is modified by this function
/// @return The byte at that offset
BYTE ChunkGetBYTE(const Chunk* chunk, int* offset);

/// @brief Gets a word from the offset specified, aligning the access
/// @param chunk The chunk to get the value from
/// @param offset The offset should point to the OP_IMMEDIATE_WORD, is modified by this function
/// @return The word at that offset
WORD ChunkGetWORD(const Chunk* chunk, int* offset);

/// @brief Gets a double word from the offset specified, aligning the access
/// @param chunk The chunk to get the value from
/// @param offset The offset should point to the OP_IMMEDIATE_DWORD
/// @return The double word at that offset
DWORD ChunkGetDWORD(const Chunk* chunk, int* offset);

/// @brief Gets a quad word from the offset specified, aligning the access
/// @param chunk The chunk to get the value from
/// @param offset The offset should point to the OP_IMMEDIATE_QWORD
/// @return The quad word at that offset
QWORD ChunkGetQWORD(const Chunk* chunk, int* offset);

/// @brief Frees memory used by a chunk
/// @param chunk The chunk to free
void ChunkFree(Chunk* chunk);

/// @brief Checks if a chunk's buffer is stack-allocated (due to small buffer optimization)
/// @param chunk The chunk for which to check
/// @return True if the chunk is using its internal stack buffer
bool ChunkIsStackAllocated(const Chunk* chunk);

/**********************************
IMPLEMENTATION HELPERS
**********************************/

/// @brief Extracts a BYTE from a pointer and updates the location pointed by that pointer
/// @param ptr Pointer to the pointer pointing to the byte following OP_IMMEDIATE_BYTE (not checked)
/// @return BYTE union representing the read byte
BYTE unsafe_get_byte(uint8_t** ptr);

/// @brief Extracts a WORD from a pointer and updates the location pointed by that pointer
/// @param ptr Pointer to the pointer pointing to the byte following OP_IMMEDIATE_WORD (not checked)
/// @return WORD union representing the read word
WORD unsafe_get_word(uint8_t** ptr);

/// @brief Extracts a DWORD from a pointer and updates the location pointed by that pointer
/// @param ptr Pointer to the pointer pointing to the byte following OP_IMMEDIATE_DWORD (not checked)
/// @return DWORD union representing the read word
DWORD unsafe_get_dword(uint8_t** ptr);

/// @brief Extracts a QWORD from a pointer and updates the location pointed by that pointer
/// @param ptr Pointer to the pointer pointing to the byte following OP_IMMEDIATE_QWORD(not checked)
/// @return QWORD union representing the read word
QWORD unsafe_get_qword(uint8_t** ptr);

/// @brief Doubles the capacity of a chunk
/// @param chunk The chunk to modify
void impl_chunk_grow_double(Chunk* chunk);

/// @brief Augments the capacity of a chunk by 'size'
/// @param chunk The chunk to modify
/// @param size The capacity to add
void impl_chunk_grow_size(Chunk* chunk, uint32_t size);

/// @brief Appends a byte at the end of the chunk
/// @param chunk The chunk to modify
/// @param byte The byte to append
void impl_chunk_write_byte(Chunk* chunk, uint8_t byte);

#endif //HG_COLTI_CHUNK