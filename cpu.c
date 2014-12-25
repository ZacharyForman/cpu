// Defines the internal structure of the CPU
// and its model of computation.

#include "cpu.h"
#include "log.h"
#include "opcodes.h"

#include <stdlib.h>
#include <string.h>

struct _cpu {
  // Current program count.
  word_t pc;

  // Current instruction
  word_t ir;

  // 32 general purpose registers. r0 is always 0.
  word_t r[32];

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
  word_t psw;

  // Value of PC at time of interrupt/exception
  word_t xar;

  // Address of 15 word table of exception handlers
  word_t xbr;

  // Added to all addresses while in user-mode
  word_t mbr;

  // Number of bytes accessible in user mode, relative to mbr.
  word_t mlr;

  // Timer status word
  // 0  -> start timer
  // 6  -> interrupt enable
  // 7  -> ready
  // 16 -> interrupt 0
  // ...
  // 23 -> interrupt 7
  word_t tsr;

  // Timer; decremented by one each cycle.
  word_t tcr;

  // Internals used for simulation

  // Actual location of byte '0'.
  byte_t *mem;

  // Has the processor halted?
  int halted;

};

// CPU internal function to fetch next instruction
void _fetch(cpu c) {
  if (c->pc % sizeof(word_t))
    FATAL("TODO: Trigger illegal access interrupt");

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

  c->psw = 0x00000000;
  c->xar = 0x00000000;
  c->xbr = 0x00000000;
  c->mbr = 0x00000000;
  c->mlr = 0x00000000;
  // Set timer to ready
  c->tsr = 0x00000080;
  c->tcr = 0x00000000;

  c->mem = mem;

  c->halted = 0;

  return c;
}

// Perform a cycle on the CPU
void cycle(cpu c) {
  _fetch(c);
  _execute_current_instruction(c);
}

// Utility functions

// Returns the value of register reg.
word_t read_register(cpu c, int reg) {
  if (reg < 0 || reg > 31)
    FATAL("Register %d out of bounds", reg);
  return c->r[reg];
}

// Returns the value of pc.
word_t read_pc(cpu c) {
  return c->pc;
}

// Returns the value of ir.
word_t read_psw(cpu c) {
  return c->psw;
}

// Returns the value of psw.
word_t read_ir(cpu c) {
  return c->ir;
}

// Returns the value of xar
word_t read_xar(cpu c) {
  return c->xar;
}

// Returns the value of xbr
word_t read_xbr(cpu c) {
  return c->xbr;
}

// Returns the value of mbr
word_t read_mbr(cpu c) {
  return c->mbr;
}

// Returns the value of mlr
word_t read_mlr(cpu c) {
  return c->mlr;
}

// Returns the value of tsr
word_t read_tsr(cpu c) {
  return c->tsr;
}

// Returns the value of tmr
word_t read_tcr(cpu c) {
  return c->tcr;
}
