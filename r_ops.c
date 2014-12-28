// Fragment used to define all the R type ops.
// c is the cpu.
// op is the operation.
case 0: {
  int src1 = R_SRC1(op);
  int src2 = R_SRC2(op);
  int dst  = R_DST(op);
  switch (R_TYPE(op)) {
    // Control
    case NOP: {
      return -1;
    }
    case HALT: {
      c->halted = 1;
      return -1;
    }
    case WAIT: {
      c->waiting = 1;
      return -1;
    }

    // Shifts
    case SLL: {
      // Note: DLX does only use the first 5 bits of src2.
      c->r[dst] = c->r[src1] << (c->r[src2] & 0x1F);
      return -1;
    }
    case SRL: {
      // Note: DLX does only use the first 5 bits of src2.
      c->r[dst] = c->r[src1] >> (c->r[src2] & 0x1F);
      return -1;
    }
    case SRA: {
      // Note: DLX does only use the first 5 bits of src2.
      c->r[dst] = ((sword_t)c->r[src1]) >> (c->r[src2] & 0x1F);
      return -1;
    }

    // Logical
    case SEQU: {
      c->r[dst] = c->r[src1] == c->r[src2];
      return -1;
    }
    case SNEU: {
      c->r[dst] = c->r[src1] != c->r[src2];
      return -1;
    }
    case SLTU: {
      c->r[dst] = c->r[src1] < c->r[src2];
      return -1;
    }
    case SGTU: {
      c->r[dst] = c->r[src1] > c->r[src2];
      return -1;
    }
    case SLEU: {
      c->r[dst] = c->r[src1] <= c->r[src2];
      return -1;
    }
    case SGEU: {
      c->r[dst] = c->r[src1] <= c->r[src2];
      return -1;
    }

    // Arithmetic:
    case ADD: {
      sword_t a1, a2;
      int ret = -1;
      a1 = (sword_t)c->r[R_SRC1(op)];
      a2 = (sword_t)c->r[R_SRC2(op)];
      // Integer overflow
      if (a1 > 0 && a2 > INT_MAX - a1) {
        ret = 3;
      }
      if (a1 < 0 && a2 < INT_MIN - a1) {
        ret = 3;
      }
      c->r[R_DST(op)] = a1 + a2;
      return ret;
    }
    case ADDU: {
      c->r[dst] = c->r[src1] + c->r[src2];
      return -1;
    }
    case SUB: {
      sword_t a1, a2;
      int ret = -1;
      a1 = (sword_t)c->r[R_SRC1(op)];
      a2 = (sword_t)c->r[R_SRC2(op)];
      // Integer overflow
      if (a1 > 0 && (-a2) > INT_MAX - a1) {
        ret = 3;
      }
      if (a1 < 0 && (-a2) < INT_MIN - a1) {
        ret = 3;
      }
      c->r[R_DST(op)] = a1 - a2;
      return ret;
    }
    case SUBU: {
      c->r[dst] = c->r[src1] - c->r[src2];
      return -1;
    }

    // Logical
    case AND: {
      c->r[dst] = c->r[src1] & c->r[src2];
      return -1;
    }
    case OR: {
      c->r[dst] = c->r[src1] | c->r[src2];
      return -1;
    }
    case XOR: {
      c->r[dst] = c->r[src1] ^ c->r[src2];
      return -1;
    }

    // Logical
    case SEQ: {
      c->r[dst] = ((sword_t)c->r[src1]) == c->r[src2];
      return -1;
    }
    case SNE: {
      c->r[dst] = ((sword_t)c->r[src1]) != c->r[src2];
      return -1;
    }
    case SLT: {
      c->r[dst] = ((sword_t)c->r[src1]) < c->r[src2];
      return -1;
    }
    case SGT: {
      c->r[dst] = ((sword_t)c->r[src1]) > c->r[src2];
      return -1;
    }
    case SLE: {
      c->r[dst] = ((sword_t)c->r[src1]) <= c->r[src2];
      return -1;
    }
    case SGE: {
      c->r[dst] = ((sword_t)c->r[src1]) <= c->r[src2];
      return -1;
    }

    // Special
    case MOVI2S: {
      if (dst >= 1 && dst <= 7) {
        c->s[dst] = c->r[src1];
      } else {
        ERROR("Special register %d is out of bounds", dst);
      }
      return -1;
    }
    case MOVS2I: {
      if (src1 >= 1 && src1 <= 7) {
        c->r[dst] = c->s[src1];
      } else {
        ERROR("Special register %d is out of bounds", src1);
      }
      return -1;
    }
  }
}
