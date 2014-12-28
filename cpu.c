// Defines the internal structure of the CPU
// and its model of computation.

#include "cpu.h"
#include "log.h"
#include "opcodes.h"

#include <limits.h>
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

// Internal representation of the CPU.
struct _cpu {
  // Current program count.
  word_t pc;

  // Current instruction
  word_t ir;
  // Used for exceptions
  word_t iaddr;

  // 32 general purpose registers. r0 is always 0.
  word_t r[32];

  // 7 special purpose registers, detailed above.
  word_t s[8];


  // Memory structure
  memory mem;

  // Has the processor halted?
  int halted;

  // Are we waiting for an interrupt?
  int waiting;

};

// Construct a new CPU with provided memory 
// pointer and a standard initialization.
cpu new_cpu(memory mem)
{
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
  c->waiting = 0;

  return c;
}

// Frees the cpu provided.
void free_cpu(cpu c)
{
  free(c);
}

// CPU internal function to fetch next instruction
int _fetch(cpu c)
{
  if (c->pc % sizeof(word_t)) {
    // Memory access exception
    return 1;
  }

  c->ir = *memword(c->mem, c->pc);
  c->iaddr = c->pc;
  c->pc += 4;

  return -1;
}

// CPU internal function to execute the instruction in IR.
int _execute_current_instruction(cpu c)
{
  word_t op = c->ir;
  switch (TYPE(op)) {
// Contains the R type operations
#include "r_ops.c"

// Contains the I type operations
#include "i_ops.c"

// Contains the L type operations
#include "l_ops.c"
  }
  INFO("No such instruction %X", op);
  return 1;
}

// Perform a cycle on the CPU.
// This function simply runs _fetch and _execute,
// handling exceptions produced by them.
int _cycle(cpu c)
{
  int ex;

  ex = _fetch(c);
  if (ex != -1) {
    print_cpu_details(stderr, c);
    FATAL("Impossible.");
  }
  ex = _execute_current_instruction(c);
  c->r[0] = 0;

  return ex;
}

// Perform a cycle on the CPU
// This involves fetching the instruction at PC
// and then executing it.
void cycle(cpu c)
{
  int ex;
  int haltable = 1;

  // If halted, do nothing.
  if (c->halted) {
    return;
  }

  // TODO(au.zachary.forman) Handle the counter.

  // TODO(au.zachary.forman) Check for interrupts

  // If waiting and no interrupt, do nothing.
  if (c->waiting) {
    return;
  }

  ex = _cycle(c);

  // Handle exceptions
  switch (ex) {
    case -1: {
      // no exception
      return;
    }
    case 0:                 // Illegal instruction
    case 1:                 // Illegal memory access
    case 2:                 // Memory protection trap
    case 3: haltable = 0;   // Integer overflow
    case 4: haltable = 0;   // Floating point trap
    case 7: {               // User initiated trap
      if ((c->s[PSW] & (1 << (8+ex))) && ((c->s[PSW] & 1) == 0)) {
        // Set XAR
        c->s[XAR] = c->iaddr;
        // Set master exception bit to 0
        c->s[PSW] &= ~1;
        // Set Up = u
        c->s[PSW] = (c->s[PSW] & (~0x4)) | ((c->s[PSW] & 0x2) << 1);
        // Set u = 0.
        c->s[PSW] &= ~0x2;
        // Jump into exception table
        c->pc = c->s[XBR] + ex*sizeof(word_t);
      } else {
        c->halted = haltable;
      }
      return;
    }
    default: {
      // Programming error.
      ERROR("ERROR: Printing debug info");
      print_cpu_details(stderr, c);
      FATAL("Error: No such exception %d", ex);
    }
  }
}

// Utility functions

// Returns the value of register reg.
word_t read_register(cpu c, int reg)
{
  if (reg < 0 || reg > 31) {
    ERROR("Register %d out of bounds", reg);
    return 0;
  }
  return c->r[reg];
}

// Returns the value of pc.
word_t read_pc(cpu c)
{
  return c->pc;
}

// Returns the value of ir.
word_t read_ir(cpu c)
{
  return c->ir;
}

// Returns the value of psw.
word_t read_psw(cpu c)
{
  return c->s[PSW];
}

// Returns the value of xar
word_t read_xar(cpu c)
{
  return c->s[XAR];
}

// Returns the value of xbr
word_t read_xbr(cpu c)
{
  return c->s[XBR];
}

// Returns the value of mbr
word_t read_mbr(cpu c)
{
  return c->s[MBR];
}

// Returns the value of mlr
word_t read_mlr(cpu c)
{
  return c->s[MLR];
}

// Returns the value of tsr
word_t read_tsr(cpu c)
{
  return c->s[TSR];
}

// Returns the value of tmr
word_t read_tcr(cpu c)
{
  return c->s[TCR];
}

// Prints an array of details about the current state of c.
void print_cpu_details(FILE *f, cpu c)
{
  int i;
  
  fprintf(f, 
    "\nCPU:\n PC: %.8X\t IR: %.8X\nPSW: %.8X\tXAR: %.8X\n"
    "XBR: %.8X\tMBR: %.8X\nMLR: %.8X\tTSR: %.8X\nTCR: %.8X\n\nRegisters:\n",
    c->pc, c->ir, c->s[PSW], c->s[XAR], c->s[XBR], c->s[MBR],
    c->s[MLR], c->s[TSR], c->s[TCR]);

  for (i = 0; i < 16; i++) {
    fprintf(f, "r%.2d: %.8X\tr%.2d: %.8X\n", 
      2*i, c->r[2*i], 2*i+1, c->r[2*i+1]);
  }

  fprintf(f, "Current state: %s\n",
    c->halted ? "Halted" : "Executing");
}
