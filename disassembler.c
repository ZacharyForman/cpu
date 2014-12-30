// Implementation of simple disassembler.

#include "disassembler.h"
#include "opcodes.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define COMMAND_LEN 64

#define I(tab, instr) \
  do { \
    (tab[instr]) = #instr; \
  } while(0);

char **disassemble(word_t *instr, int sz)
{
  int i;
  char **lookup = malloc(sizeof(char*) * (1 << 7));
  char **r_lookup = malloc(sizeof(char*) * (1 << 7));
  for (i = 0; i < (i<<7); i++) {
    lookup[i] = 0;
    r_lookup[i] = 0;
  }
  char *special_reg_lookup[] = {
    "UNKNOWN",
    "PSW",
    "XAR",
    "XBR",
    "MBR",
    "MLR",
    "TSR",
    "TCR"
  };

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

  char **res = (char**) malloc(sizeof(char*) * sz);
  for (i = 0; i < sz; i++) {
    res[i] = (char*) malloc(COMMAND_LEN);
    switch(TYPE(instr[i])) {
      case 0: {
        if (r_lookup[R_TYPE(instr[i])] == 0) {
          sprintf(res[i], "%s", "UNKNOWN");
        } else if (instr[i] == MOVS2I) {
          sprintf(res[i], "%s\t%d, %s",
            lookup[R_TYPE(instr[i])],
            R_DST(instr[i]),
            special_reg_lookup[R_SRC1(instr[i])]);
        } else if (instr[i] == MOVI2S) {
          sprintf(res[i], "%s\t%s, %d",
            lookup[R_TYPE(instr[i])],
            special_reg_lookup[R_DST(instr[i])],
            R_SRC1(instr[i]));
        } else if (instr[i] == NOP ||
            instr[i] == HALT ||
            instr[i] == WAIT) {
          sprintf(res[i], "%s", 
            r_lookup[R_TYPE(instr[i])]);
        } else {
          sprintf(res[i], "%s\tr%d, r%d, r%d",
          r_lookup[R_TYPE(instr[i])],
          R_DST(instr[i]),
          R_SRC1(instr[i]),
          R_SRC2(instr[i]));
        }
        break;
      }
      case RFE: {
        sprintf(res[i], "%s", 
          lookup[TYPE(instr[i])]);
        break;
      }
      case J:
      case JAL: {
        sprintf(res[i], "%s\t%d", 
          lookup[TYPE(instr[i])],
          L_SGN(L_IMM(instr[i])));
        break;
      }
      case TRAP: {
        sprintf(res[i], "%s\t%u", 
          lookup[TYPE(instr[i])],
          L_IMM(instr[i]));
        break;
      }
      case LB:
      case LBU:
      case LH:
      case LHU:
      case LW: {
        sprintf(res[i], "%s\tr%d, %d(r%d)",
          lookup[TYPE(instr[i])],
          I_DST(instr[i]),
          I_SGN(I_IMM(instr[i])),
          I_SRC(instr[i]));
        break;
      }
      case SB:
      case SH:
      case SW: {
        sprintf(res[i], "%s\t%d(r%d), r%d",
          lookup[TYPE(instr[i])],
          I_SGN(I_IMM(instr[i])),
          I_DST(instr[i]),
          I_SRC(instr[i]));
        break;
      }
      case LHI:
      case BEQZ:
      case BNEZ: {
        sprintf(res[i], "%s\tr%d, %.4x",
          lookup[TYPE(instr[i])],
          I_DST(instr[i]),
          I_IMM(instr[i]));
        break;
      }
      case JR:
      case JALR: {
        sprintf(res[i], "%s\tr%d",
          lookup[TYPE(instr[i])],
          I_SRC(instr[i]));
        break;
      }
      default: {
        char *s = lookup[TYPE(instr[i])];
        if (s == 0) {
          sprintf(res[i], "%s", "UNKNOWN");
        } else if (s[strlen(s)-1] == 'U') {
          sprintf(res[i], "%s\tr%d, r%d, %u",
            s, I_DST(instr[i]),
            I_SRC(instr[i]),
            I_IMM(instr[i]));
        } else {
          sprintf(res[i], "%s\tr%d, r%d, %d",
            s, I_DST(instr[i]),
            I_SRC(instr[i]),
            I_SGN(I_IMM(instr[i])));
        }
        break;
      }
    }
  }
  return res;
}