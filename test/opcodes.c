// Tests for opcodes.h
#include "../opcodes.h"
#include "../types.h"
#include "test.h"

// Tests for R type instructions.
void test_r_opcodes()
{
  int i;
  struct {
    word_t op;
    unsigned r_type;
    unsigned r_dst;
    unsigned r_src1;
    unsigned r_src2;
  } t[] = {
    {0x00000000,  NOP,  0,  0,  0},
    {0x00430820,  ADD,  1,  2,  3},
    {0x0361F823, SUBU, 31, 27,  1},
    {0x00000020,  ADD,  0,  0,  0},
    {0x00000001, HALT,  0,  0,  0},
    {0x0041202D,  SGE,  4,  2,  1},
    {0x00210804,  SLA,  1,  1,  1},
    {0x03FFF82D,  SGE, 31, 31, 31},
    {0x00000020,  ADD,  0,  0,  0},
    {0x00000004,  SLL,  0,  0,  0},
  };

  for (i = 0; i < sizeof(t)/sizeof(t[0]); i++) {
    REQUIRE(4*i, R_TYPE(t[i].op), t[i].r_type);
    REQUIRE(4*i+1, R_DST(t[i].op), t[i].r_dst);
    REQUIRE(4*i+2, R_SRC1(t[i].op), t[i].r_src1);
    REQUIRE(4*i+3, R_SRC2(t[i].op), t[i].r_src2);
  }
}

// Tests for I type instructions.
void test_i_opcodes()
{
  int i;
  struct {
    word_t op;
    unsigned type;
    word_t i_imm;
    unsigned i_dst;
    unsigned i_src;
  } t[] = {
    {0x27E12710, ADDUI, 10000, 1, 31},
    {0x24010000, ADDUI,     0, 1,  0},
    {0x24210001, ADDUI,     1, 1,  1},
    {0x3041000E,  ANDI,    14, 1,  2},
    {0x3C01FFFF,   LHI, 65535, 1,  0},
    {0x3C010000,   LHI,     0, 1,  0},
    {0x53E10025,  SLAI,    37, 1, 31},
    {0x50010005,  SLAI,     5, 1,  0}
  };

  for (i = 0; i < sizeof(t)/sizeof(t[0]); i++) {
    REQUIRE(4*i, TYPE(t[i].op), t[i].type);
    REQUIRE(4*i+1, I_IMM(t[i].op), t[i].i_imm);
    REQUIRE(4*i+2, I_DST(t[i].op), t[i].i_dst);
    REQUIRE(4*i+3, I_SRC(t[i].op), t[i].i_src);
  }
}

// Tests for signed I type instructions.
void test_i_s_opcodes()
{
  int i;
  struct {
    word_t op;
    unsigned type;
    sword_t i_imm;
    unsigned i_dst;
    unsigned i_src;
  } t[] = {
    {0x8C210004,   LW,     4,  1,  1},
    {0x8FFF7FFF,   LW, 32767, 31, 31},
    {0x93FFFFFF,  LBU,    -1, 31, 31},
    {0x60210190, SEQI,   400,  1,  1}
  };

  for (i = 0; i < sizeof(t)/sizeof(t[0]); i++) {
    REQUIRE(4*i, TYPE(t[i].op), t[i].type);
    REQUIRE(4*i+1, I_SGN(I_IMM(t[i].op)), t[i].i_imm);
    REQUIRE(4*i+2, I_DST(t[i].op), t[i].i_dst);
    REQUIRE(4*i+3, I_SRC(t[i].op), t[i].i_src);
  }
}

// Tests for L type instructions.
void test_l_opcodes()
{
  int i;
  struct {
    word_t op;
    unsigned type;
    word_t l_imm;
  } t[] = {
    {0x40000000,  RFE, 0},
    {0x44000002, TRAP, 2},
    {0x44000001, TRAP, 1},
    {0x47FFFFFF, TRAP, 67108863}
  };

  for (i = 0; i < sizeof(t)/sizeof(t[0]); i++) {
    REQUIRE(4*i, TYPE(t[i].op), t[i].type);
    REQUIRE(4*i+1, L_IMM(t[i].op), t[i].l_imm);
  }
}

// Tests for L type signed instructions.
void test_l_s_opcodes()
{
  int i;
  struct {
    word_t op;
    unsigned type;
    sword_t l_imm;
  } t[] = {
    {0x0800000C,   J, 12},
    {0x0BFFFFF8,   J, -8},
    {0x0BFFFFFF,   J, -1},
    {0x09FFFFFF,   J, 33554431},
    {0x0C00000E, JAL, 14},
    {0x0C0000FC, JAL, 252},
    {0x0C000084, JAL, 132}
  };

  for (i = 0; i < sizeof(t)/sizeof(t[0]); i++) {
    REQUIRE(4*i, TYPE(t[i].op), t[i].type);
    REQUIRE(4*i+1, L_SGN(L_IMM(t[i].op)), t[i].l_imm);
  }
}

// Driver
int main()
{
  test_r_opcodes();
  test_i_opcodes();
  test_i_s_opcodes();
  test_l_opcodes();
  test_l_s_opcodes();
  return 0;
}
