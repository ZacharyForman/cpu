// Tests for opcodes.h
#include "../opcodes.h"
#include "test.h"

// Tests for R type instructions.
void test_r_opcodes() {
  int i;
  struct {
    unsigned op;
    unsigned r_type;
    unsigned r_dst;
    unsigned r_src1;
    unsigned r_src2;
  } t[] = {
    {0x00000000, NOP   ,  0,  0,  0},
    {0x00430820, ADD   ,  1,  2,  3},
    {0x0361F823, SUBU  , 31, 27,  1},
    {0x00000020, ADD   ,  0,  0,  0},
    {0x00000001, HALT  ,  0,  0,  0},
    {0x0041202D, SGE   ,  4,  2,  1},
    {0x00210804, SLA   ,  1,  1,  1},
    {0x03FFF82D, SGE   , 31, 31, 31},
    {0x00000020, ADD   ,  0,  0,  0},
    {0x00000004, SLL   ,  0,  0,  0},
  };

  for (i = 0; i < sizeof(t)/sizeof(t[0]); i++) {
    REQUIRE(4*i, R_TYPE(t[i].op), t[i].r_type);
    REQUIRE(4*i+1, R_DST(t[i].op), t[i].r_dst);
    REQUIRE(4*i+2, R_SRC1(t[i].op), t[i].r_src1);
    REQUIRE(4*i+3, R_SRC2(t[i].op), t[i].r_src2);
  }
}

// Tests for I type instructions.
void test_i_opcodes() {
  int i;
  struct {
    unsigned op;
    unsigned type;
    unsigned i_imm;
    unsigned i_dst;
    unsigned i_src;
  } t[0];

  for (i = 0; i < sizeof(t)/sizeof(t[0]); i++) {
    REQUIRE(4*i, TYPE(t[i].op), t[i].type);
    REQUIRE(4*i+1, I_IMM(t[i].op), t[i].i_imm);
    REQUIRE(4*i+2, I_DST(t[i].op), t[i].i_dst);
    REQUIRE(4*i+3, I_SRC(t[i].op), t[i].i_src);
  }
}

// Tests for signed I type instructions.
void test_i_s_opcodes() {
  int i;
  struct {
    unsigned op;
    unsigned type;
    signed i_imm;
    unsigned i_dst;
    unsigned i_src;
  } t[0];

  for (i = 0; i < sizeof(t)/sizeof(t[0]); i++) {
    REQUIRE(4*i, TYPE(t[i].op), t[i].type);
    REQUIRE(4*i+1, I_SGN(I_IMM(t[i].op)), t[i].i_imm);
    REQUIRE(4*i+2, I_DST(t[i].op), t[i].i_dst);
    REQUIRE(4*i+3, I_SRC(t[i].op), t[i].i_src);
  }
}

// Tests for L type instructions.
void test_l_opcodes() {
  int i;
  struct {
    unsigned op;
    unsigned type;
    signed l_imm;
  } t[0];

  for (i = 0; i < sizeof(t)/sizeof(t[0]); i++) {
    REQUIRE(4*i, TYPE(t[i].op), t[i].type);
    REQUIRE(4*i+1, L_SGN(L_IMM(t[i].op)), t[i].l_imm);
  }

}

int main() {
  test_r_opcodes();
  test_i_opcodes();
  test_i_s_opcodes();
  test_l_opcodes();

  return 0;
}