// Defines the internal structure of the CPU
// and its model of computation.

#include "cpu.h"
#include "log.h"
#include "opcodes.h"

#include <stdlib.h>
#include <string.h>

// Special registers defining CPU behaviour.

// Processor status word
// 0  -> master exception enable
// 2  -> user mode previous
// 3  -> user mode enable
// 4  -> floating points enabled
// 8  -> illegal instruction trap
// 9  -> illegal memory address trap
// 10 -> memory protection trap
// 12 -> floating point trap
// 15 -> user initiated trap enable
// 16 -> interrupt 0 enabled.
// ...
// 23 -> interrupt 7 enabled
#define PSW 0x01

// Value of PC at time of interrupt/exception
#define XAR 0x02

// Address of 15 word table of exception handlers
#define XBR 0x03

// Added to all addresses while in user-mode
#define MBR 0x04

// Number of bytes accessible in user mode, relative to mbr.
#define MLR 0x05

// Timer status word
// 0  -> start timer
// 6  -> interrupt enable
// 7  -> ready
// 16 -> interrupt 0
// ...
// 23 -> interrupt 7
#define TSR 0x06

// Timer; decremented by one each cycle.
#define TCR 0x07

struct _cpu {
  // Current program count.
  word_t pc;

  // Current instruction
  word_t ir;

  // 32 general purpose registers. r0 is always 0.
  word_t r[32];

  // 7 special purpose registers, detailed above.
  word_t s[8];


  // Actual location of byte '0'.
  byte_t *mem;

  // Has the processor halted?
  int halted;

};

// CPU internal function to fetch next instruction
void _fetch(cpu c) {

  if (c->pc % sizeof(word_t)) {
    // TODO(au.zachary.forman) Trigger interrupt instead
    FATAL("Unaligned access");
  }

  c->ir = *((word_t*)(c->mem) + c->pc);
  c->pc += 4;
}

// CPU internal function to execute the instruction in IR.
void _execute_current_instruction(cpu c) {
  word_t op = TYPE(c->ir);
  switch (TYPE(op)) {
// Contains the R type operations
#include "r_ops.c"

// Contains the I type operations
#include "i_ops.c"

// Contains the L type operations
#include "l_ops.c"
  }
}

// Construct a new CPU with provided base
// memory pointer and a standard initialization.
// Caller must free the returned cpu pointer and
// the provided mem pointer.
cpu new_cpu(byte_t *mem) {
  cpu c = (cpu) malloc(sizeof(struct _cpu));

  c->pc  = 0x00000000;
  memset(c->r, 0, 32 * sizeof(word_t));

  c->s[PSW] = 0x00000000;
  c->s[XAR] = 0x00000000;
  c->s[XBR] = 0x00000000;
  c->s[MBR] = 0x00000000;
  c->s[MLR] = 0x00000000;
  // Set timer to ready
  c->s[TSR] = 0x00000080;
  c->s[TCR] = 0x00000000;

  c->mem = mem;

  c->halted = 0;

  return c;
}

// Perform a cycle on the CPU
// This involves fetching the instruction at PC
// and then executing it.
void cycle(cpu c) {
  _fetch(c);
  _execute_current_instruction(c);
}

// Utility functions

// Returns the value of register reg.
word_t read_register(cpu c, int reg) {
  if (reg < 0 || reg > 31) {
    // TODO(au.zachary.forman) Reconsider.
    FATAL("Register %d out of bounds", reg);
  }
  return c->r[reg];
}

// Returns the value of pc.
word_t read_pc(cpu c) {
  return c->pc;
}

// Returns the value of ir.
word_t read_ir(cpu c) {
  return c->ir;
}

// Returns the value of psw.
word_t read_psw(cpu c) {
  return c->s[PSW];
}

// Returns the value of xar
word_t read_xar(cpu c) {
  return c->s[XAR];
}

// Returns the value of xbr
word_t read_xbr(cpu c) {
  return c->s[XBR];
}

// Returns the value of mbr
word_t read_mbr(cpu c) {
  return c->s[MBR];
}

// Returns the value of mlr
word_t read_mlr(cpu c) {
  return c->s[MLR];
}

// Returns the value of tsr
word_t read_tsr(cpu c) {
  return c->s[TSR];
}

// Returns the value of tmr
word_t read_tcr(cpu c) {
  return c->s[TCR];
}
