#ifndef _CPU_MEMORY_H
#define _CPU_MEMORY_H

// Define the interface for interacting with memory

#include "types.h"

typedef struct _memory memory;

// Returns a new memory system.
memory *new_memory();

//Frees the provided memory system.
void free_memory(memory *mem);

// Returns a pointer to the byte at address.
byte_t *membyte(memory *mem, word_t address);

// Returns a pointer to the halfword at address.
halfword_t *memhalfword(memory *mem, word_t address);

// Returns a pointer to the word at address.
word_t *memword(memory *mem, word_t address);

#endif // _CPU_MEMORY_H