#ifndef HG_COLTI_CHUNK
#define HG_COLTI_CHUNK

#include "common.h"
#include "byte_code.h" //Contains the byte-code enum

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
	uint8_t code_buffer[8];
} Chunk;

/// @brief Prints the byte content of a Chunk
/// @param chunk The chunk whose content to print
void ChunkPrintBytes(const Chunk* chunk);

/// @brief Zero-initializes a chunk
/// @param chunk The chunk to initialize
void ChunkInit(Chunk* chunk);

/// @brief Appends an OpCode to the end of the chunk
/// @param chunk The chunk to append to
/// @param byte The byte to append
void ChunkWriteOpCode(Chunk* chunk, OpCode code);

/// @brief Appends a byte to the end of the chunk
/// @param chunk The chunk to append to
/// @param byte The byte to append
void ChunkWriteByte(Chunk* chunk, BYTE byte);

/// @brief Appends multiple bytes to the end of a chunk
/// @param chunk The chunk to append to
/// @param bytes The array of bytes from which to copy the bytes
/// @param size The number of bytes to copy from 'bytes'
void ChunkWriteBytes(Chunk* chunk, const uint8_t* const bytes, uint32_t size);

/// @brief Writes an int16 to the end of a chunk, padding if necessary
/// @param chunk The chunk to append to
/// @param value The value to write
void ChunkWriteWord(Chunk* chunk, WORD value);

/// @brief Writes an int32 to the end of a chunk, padding if necessary
/// @param chunk The chunk to append to
/// @param value The value to write
void ChunkWriteDWord(Chunk* chunk, DWORD value);

/// @brief Writes an int64 to the end of a chunk, padding if necessary
/// @param chunk The chunk to append to
/// @param value The value to write
void ChunkWriteQWord(Chunk* chunk, QWORD value);

/// @brief Gets a byte from the offset specified
/// @param chunk The chunk to get the value from
/// @param offset The offset should point to the OP_IMMEDIATE_BYTE, is modified by this function
/// @return The byte at that offset
BYTE ChunkGetByte(const Chunk* chunk, int* offset);

/// @brief Gets a word from the offset specified, aligning the access
/// @param chunk The chunk to get the value from
/// @param offset The offset should point to the OP_IMMEDIATE_WORD, is modified by this function
/// @return The word at that offset
WORD ChunkGetWord(const Chunk* chunk, int* offset);

/// @brief Gets a double word from the offset specified, aligning the access
/// @param chunk The chunk to get the value from
/// @param offset The offset should point to the OP_IMMEDIATE_DWORD
/// @return The double word at that offset
DWORD ChunkGetDWord(const Chunk* chunk, int* offset);

/// @brief Gets a quad word from the offset specified, aligning the access
/// @param chunk The chunk to get the value from
/// @param offset The offset should point to the OP_IMMEDIATE_QWORD
/// @return The quad word at that offset
QWORD ChunkGetQWord(const Chunk* chunk, int* offset);

/// @brief Frees memory used by a chunk
/// @param chunk The chunk to free
void ChunkFree(Chunk* chunk);

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

#endif //HG_COLTI_CHUNK