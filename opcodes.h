#ifndef _CPU_OPCODES_H
#define _CPU_OPCODES_H

// List of opcode constants for DLX and macros
// for extracting information from these opcodes.

// The DLX instructionset has 3 kinds of instructions:
// r:
//   000000 rrrrr rrrrr rrrrr _____ xxxxxx
// i:
//   xxxxxx rrrrr rrrrr ################
// l:
//   xxxxxx ##########################

// Extracts the type from an operation.
#define TYPE(op)   (((op) & 0xFC000000) >> 26)


// Extracts subtype of an R operation.
#define R_TYPE(op) (((op) & 0x0000003F) >> 0)

// Extracts destination register of an R operation
#define R_DST(op)  (((op) & 0x0000F800) >> 11)

// Extracts the first source register of an R operation.
#define R_SRC1(op) (((op) & 0x03E00000) >> 21)

// Extracts the second source register of an R operation.
#define R_SRC2(op) (((op) & 0x001F0000) >> 16)


// Extracts the immediate value of an I operation.
#define I_IMM(op)  (((op) & 0x0000FFFF) >> 0)

// Extracts the destination register of an I operation.
#define I_DST(op)  (((op) & 0x001F0000) >> 16)

// Extracts the source register of an I operation.
#define I_SRC(op)  (((op) & 0x03E00000) >> 21)


// Extracts the immediate value of a J operation.
#define L_IMM(op)  (((op) & 0x03FFFFFF) >> 0)


// Some operations take unsigned immediates.
// These are simply used as extracted from
// the word.
// However, some operations take signed
// immediates, and then must sign extend.
// The behaviour of SIGN-EXTEND(k) is to
// propagate the most significant bit of k
// forward.

// Sign extend a 16 bit immediate
#define I_SGN(k) ( ((k) & 0x00008000) ? (0xFFFF0000 | (k)) : (k) )

//Sign extend a 26 bit immediate.
#define L_SGN(k) ( ((k) & 0x02000000) ? (0xFC000000 | (k)) : (k) )


// r type instructions

// Control
#define NOP    0x00
#define HALT   0x01

// Shifts
#define SLA    0x04
#define SLL    0x04
#define SRL    0x06
#define SRA    0x07

// Logical
#define SEQU   0x10
#define SNEU   0x11
#define SLTU   0x12
#define SGTU   0x13
#define SLEU   0x14
#define SGEU   0x15

// Arithmetic
#define ADD    0x20
#define ADDU   0x21
#define SUB    0x22
#define SUBU   0x23

// Logical
#define AND    0x24
#define OR     0x25
#define XOR    0x26

// Logical
#define SEQ    0x28
#define SNE    0x29
#define SLT    0x2A
#define SGT    0x2B
#define SLE    0x2C
#define SGE    0x2D

// Special
#define MOVI2S 0x30
#define MOVS2I 0x31

// i type instructions

// Arithmetic
#define ADDI   0x08
#define ADDUI  0x09
#define SUBI   0x0A
#define SUBUI  0x0B

// Logical
#define ANDI   0x0C
#define ORI    0x0D
#define XORI   0x0E

// Register control
#define LHI    0x0F

// Control
#define JR     0x12
#define JALR   0x13

// Shifts
#define SLAI   0x14
#define SLLI   0x14
#define SRLI   0x16
#define SRAI   0x17

// Logical
#define SEQI   0x18
#define SLTI   0x1A
#define SGTI   0x1B
#define SLEI   0x1C
#define SGEI   0x1D

// Memory control
#define LB     0x20
#define LH     0x21
#define LW     0x23
#define LBU    0x24
#define LHU    0x25
#define SB     0x28
#define SH     0x29
#define SW     0x2B

//Logical
#define SEQUI  0x30
#define SNEUI  0x31
#define SLTUI  0x32
#define SGTUI  0x33
#define SLEUI  0x34
#define SGEUI  0x35

// l type instructions

//Control
#define J      0x02
#define JAL    0x03
#define RFE    0x10
#define TRAP   0x11

#endif // _CPU_OPCODES_H
