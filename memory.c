// Defines an implementation of memory

#include "memory.h"

#include <stdlib.h>

// Internal representation of memory
struct _memory {
  // Page table to allow for simulating 4GB of memory
  // on a budget.
  // An address is split into 10 bits, giving the
  // page of memory the actual memory is, and 22 bits,
  // giving the offset of memory into the page.
  word_t **pt;
};

memory new_memory()
{
  int i;
  memory mem = (memory) malloc(sizeof(memory));
  mem->pt = (word_t**) malloc(1024 * sizeof(word_t*));
  for (i = 0; i < 1024; i++) {
    mem->pt[i] = 0;
  }
  return mem;
}

void free_memory(memory mem)
{
  int i;
  for (i = 0; i < 1024; i++) {
    free(mem->pt[i]);
  }
  free(mem->pt);
}

// Return a pointer to the actual address
word_t *_address(memory mem, word_t address)
{
  word_t page = (address & 0xFFC00000) >> 22;
  word_t *base = mem->pt[page];
  if (base == NULL) {
    mem->pt[page] = (word_t*) malloc(1 << 22);
    base = mem->pt[page];
  }
  return base + (address & 0x003FFFF);
}

// Returns a pointer to the byte at address.
byte_t *membyte(memory mem, word_t address)
{
  return ((byte_t*) _address(mem, address));
}

// Returns a pointer to the halfword at address.
halfword_t *memhalfword(memory mem, word_t address)
{
  return ((halfword_t*) _address(mem, address));
}

// Returns a pointer to the word at address.
word_t *memword(memory mem, word_t address)
{
  return ((word_t*) _address(mem, address));
}

// Prints the range of memory from 
void print_words(memory mem, word_t start, unsigned range)
{
  word_t i;
  fprintf(stdout, "Address:\tValue:\n");
  for (i = 0; i < range ; i++) {
    fprintf(stdout, "%.8X:\t%.8X\n", 
      start + i*sizeof(word_t), 
      *memword(mem, start + i*sizeof(word_t)));
  }
}

// Loads count words from arr into mem, starting at start.
void load_memory(memory mem, word_t start, word_t *arr, unsigned count)
{
  int i;
  for (i = 0; i < sz; i++) {
    *memword(mem, start + i * sizeof(word_t)) = arr[i];
  }
}
