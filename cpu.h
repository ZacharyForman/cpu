#ifndef _CPU_CPU_H
#define _CPU_CPU_H

// Defines the behaviours of a CPU.
// Additionaly exposes an API for inspecting
// the CPU.

#include "memory.h"
#include "types.h"

#include <stdio.h>

typedef struct _cpu *cpu;

// Construct a new CPU with provided base
// memory pointer and a standard initialization.
cpu new_cpu(memory mem);

// Frees the cpu provided.
void free_cpu(cpu c);

// Perform a cycle on the CPU
void cycle(cpu c);

// Utility functions

// Returns the value of pc.
word_t read_pc(cpu c);

// Returns the value of ir.
word_t read_ir(cpu c);

// Returns the value of register reg.
word_t read_register(cpu c, int reg);

// Returns the value of special register special
word_t read_special(cpu c, int reg);

// Prints an array of details about the current state of c.
void print_cpu_details(FILE *f, cpu c);

#endif // _CPU_CPU_H
