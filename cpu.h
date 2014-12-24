#ifndef _CPU_CPU_H
#define _CPU_CPU_H

// Defines the behaviours of a CPU.
// Additionaly exposes an API for inspecting
// the CPU.

#include <stdint.h>


typedef int8_t   sbyte_t;
typedef int16_t  shalfword_t;
typedef int32_t  sword_t;

typedef uint8_t  byte_t;
typedef uint16_t halfword_t;
typedef uint32_t word_t;

typedef struct _cpu *cpu;

// Construct a new CPU with provided base
// memory pointer and a standard initialization.
cpu new_cpu(byte_t *mem);

// Perform a cycle on the CPU
void cycle(cpu c);

// Utility functions

// Returns the value of register reg.
word_t read_register(cpu c, int reg);

// Returns the value of psw.
word_t read_psw(cpu c);

// Returns the value of xar
word_t read_xar(cpu c);

// Returns the value of xbr
word_t read_xbr(cpu c);

// Returns the value of mbr
word_t read_mbr(cpu c);

// Returns the value of mlr
word_t read_mlr(cpu c);

// Returns the value of tsr
word_t read_tsr(cpu c);

// Returns the value of tmr
word_t read_tmr(cpu c);

#endif // _CPU_CPU_H