// Implementation of simple disassembler.

#include "disassembler.h"
#include "opcodes.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// 24 maximum length of a command.
// SNEUI\tr31, r31, -32768\0
#define COMMAND_LEN 24

// Parses a single hex instruction into its string representation.
void parse_instr(char **lookup, char **r_lookup, 
  char **special_reg_lookup, word_t instr, char *res)
{
  switch(TYPE(instr)) {
    case 0: {
      if (r_lookup[R_TYPE(instr)] == 0) {
        sprintf(res, "%s", "UNKNOWN");
      } else if (R_TYPE(instr) == MOVS2I) {
        sprintf(res, "%s\t%d, %s",
          lookup[R_TYPE(instr)],
          R_DST(instr),
          special_reg_lookup[R_SRC1(instr)]);
      } else if (R_TYPE(instr) == MOVI2S) {
        sprintf(res, "%s\t%s, %d",
          lookup[R_TYPE(instr)],
          special_reg_lookup[R_DST(instr)],
          R_SRC1(instr));
      } else if (instr == NOP ||
          instr == HALT ||
          instr == WAIT) {
        sprintf(res, "%s", 
          r_lookup[R_TYPE(instr)]);
      } else {
        sprintf(res, "%s\tr%d, r%d, r%d",
        r_lookup[R_TYPE(instr)],
        R_DST(instr),
        R_SRC1(instr),
        R_SRC2(instr));
      }
      return;
    }
    case RFE: {
      sprintf(res, "%s", 
        lookup[TYPE(instr)]);
      return;
    }
    case J:
    case JAL: {
      sprintf(res, "%s\t%d", 
        lookup[TYPE(instr)],
        L_SGN(L_IMM(instr)));
      return;
    }
    case TRAP: {
      sprintf(res, "%s\t%u", 
        lookup[TYPE(instr)],
        L_IMM(instr));
      return;
    }
    case LB:
    case LBU:
    case LH:
    case LHU:
    case LW: {
      sprintf(res, "%s\tr%d, %d(r%d)",
        lookup[TYPE(instr)],
        I_DST(instr),
        I_SGN(I_IMM(instr)),
        I_SRC(instr));
      return;
    }
    case SB:
    case SH:
    case SW: {
      sprintf(res, "%s\t%d(r%d), r%d",
        lookup[TYPE(instr)],
        I_SGN(I_IMM(instr)),
        I_DST(instr),
        I_SRC(instr));
      return;
    }
    case LHI:
    case BEQZ:
    case BNEZ: {
      sprintf(res, "%s\tr%d, %.4x",
        lookup[TYPE(instr)],
        I_DST(instr),
        I_IMM(instr));
      return;
    }
    case JR:
    case JALR: {
      sprintf(res, "%s\tr%d",
        lookup[TYPE(instr)],
        I_SRC(instr));
      return;
    }
    default: {
      char *s = lookup[TYPE(instr)];
      if (s == 0) {
        sprintf(res, "%s", "UNKNOWN");
      } else if (s[strlen(s)-2] == 'U') {
        sprintf(res, "%s\tr%d, r%d, %u",
          s, I_DST(instr),
          I_SRC(instr),
          I_IMM(instr));
      } else {
        sprintf(res, "%s\tr%d, r%d, %d",
          s, I_DST(instr),
          I_SRC(instr),
          I_SGN(I_IMM(instr)));
      }
      return;
    }
  }
}

#define I(tab, instr) \
do { \
  (tab[instr]) = #instr; \
} while(0);
// Create a table for looking up R-type instructions.
void create_r_table(char **r_lookup)
{
  // R instructions
  I(r_lookup, NOP);
  I(r_lookup, HALT);
  I(r_lookup, WAIT);
  I(r_lookup, SLA);
  I(r_lookup, SLL);
  I(r_lookup, SRL);
  I(r_lookup, SRA);
  I(r_lookup, SEQU);
  I(r_lookup, SNEU);
  I(r_lookup, SLTU);
  I(r_lookup, SGTU);
  I(r_lookup, SLEU);
  I(r_lookup, SGEU);
  I(r_lookup, ADD);
  I(r_lookup, ADDU);
  I(r_lookup, SUB);
  I(r_lookup, SUBU);
  I(r_lookup, AND);
  I(r_lookup, OR);
  I(r_lookup, XOR);
  I(r_lookup, SEQ);
  I(r_lookup, SNE);
  I(r_lookup, SLT);
  I(r_lookup, SGT);
  I(r_lookup, SLE);
  I(r_lookup, SGE);
  I(r_lookup, MOVI2S);
  I(r_lookup, MOVS2I);
}

// Create a table for looking up other instructions.
void create_table(char **lookup)
{
  // I instructions
  I(lookup, ADDI);
  I(lookup, ADDUI);
  I(lookup, SUBI);
  I(lookup, SUBUI);
  I(lookup, ANDI);
  I(lookup, ORI);
  I(lookup, XORI);
  I(lookup, LHI);
  I(lookup, BEQZ);
  I(lookup, BNEZ);
  I(lookup, JR);
  I(lookup, JALR);
  I(lookup, SLAI);
  I(lookup, SLLI);
  I(lookup, SRLI);
  I(lookup, SRAI);
  I(lookup, SEQI);
  I(lookup, SNEI);
  I(lookup, SLTI);
  I(lookup, SGTI);
  I(lookup, SLEI);
  I(lookup, SGEI);
  I(lookup, LB);
  I(lookup, LH);
  I(lookup, LW);
  I(lookup, LBU);
  I(lookup, LHU);
  I(lookup, SB);
  I(lookup, SH);
  I(lookup, SW);
  I(lookup, SEQUI);
  I(lookup, SNEUI);
  I(lookup, SLTUI);
  I(lookup, SGTUI);
  I(lookup, SLEUI);
  I(lookup, SGEUI);

  // L instructions
  I(lookup, J);
  I(lookup, JAL);
  I(lookup, RFE);
  I(lookup, TRAP);
}

#undef I

// Disassembles the array of sz instructions at instr.
// Caller must free the returned array.
char **disassemble(word_t *instr, int sz)
{
  int i;
  char *lookup[1<<6];
  char *r_lookup[1<<6];
  for (i = 0; i < (1<<6); i++) {
    lookup[i] = 0;
    r_lookup[i] = 0;
  }
  char *special_reg_lookup[32] = {
    "???", "PSW", "XAR", "XBR",
    "MBR", "MLR", "TSR", "TCR",
    "???", "???", "???", "???",
    "???", "???", "???", "???",
    "???", "???", "???", "???",
    "???", "???", "???", "???",
    "???", "???", "???", "???",
    "???", "???", "???", "???"
  };
  create_r_table(r_lookup);
  create_table(lookup);

  char **res = (char**) malloc(sizeof(char*) * sz);
  for (i = 0; i < sz; i++) {
    res[i] = (char*) malloc(COMMAND_LEN);
    parse_instr(lookup, r_lookup, special_reg_lookup, instr[i], res[i]);
  }
  return res;
}