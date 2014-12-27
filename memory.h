#ifndef _CPU_MEMORY_H
#define _CPU_MEMORY_H

// Define the interface for interacting with memory

#include "types.h"

typedef struct _memory memory;

// Returns a new memory system.
memory *new_memory();

//Frees the provided memory system
void free_memory(memory *mem);

// Returns a pointer to the byte at offset.
byte_t *membyte(memory *mem, int offset);

// Returns a pointer to the halfword at offset
halfword_t *memhalfword(memory *mem, int offset);

// Returns a pointer to the 
word_t *memword(memory *mem, int offset);

#endif // _CPU_MEMORY_H