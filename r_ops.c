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
      return;
    }
    case HALT: {
      c->halted = 1;
      return;
    }

    //Shifts
    case SLL: {
      // Note: DLX does only use the first 5 bits of src2.
      c->r[dst] = c->r[src1] << (c->r[src2] & 0x1F);
      return;
    }
    case SRL: {
      // Note: DLX does only use the first 5 bits of src2.
      c->r[dst] = c->r[src1] >> (c->r[src2] & 0x1F);
      return;
    }
    case SRA: {
      // Note: DLX does only use the first 5 bits of src2.
      c->r[dst] = (sword_t)(c->r[src1]) >> (c->r[src2] & 0x1F);
      return;
    }

    // Logical
    case SEQU: {
      c->r[dst] = c->r[src1] == c->r[src2];
      return;
    }
    case SNEU: {
      c->r[dst] = c->r[src1] != c->r[src2];
      return;
    }
    case SLTU: {
      c->r[dst] = c->r[src1] < c->r[src2];
      return;
    }
    case SGTU: {
      c->r[dst] = c->r[src1] > c->r[src2];
      return;
    }
    case SLEU: {
      c->r[dst] = c->r[src1] <= c->r[src2];
      return;
    }
    case SGEU: {
      c->r[dst] = c->r[src1] <= c->r[src2];
      return;
    }

    // Arithmetic:
    case ADD: {
      //TODO(au.zachary.forman) trap on overflow
      c->r[dst] = (sword_t)(c->r[src1]) + c->r[src2];
      return;
    }
    case ADDU: {
      c->r[dst] = c->r[src1] + c->r[src2];
      return;
    }
    case SUB: {
      //TODO(au.zachary.forman) trap on overflow
      c->r[dst] = (sword_t)(c->r[src1]) - c->r[src2];
      return;
    }
    case SUBU: {
      c->r[dst] = c->r[src1] - c->r[src2];
      return;
    }

    //Logical
    case AND: {
      c->r[dst] = c->r[src1] & c->r[src2];
      return;
    }
    case OR: {
      c->r[dst] = c->r[src1] | c->r[src2];
      return;
    }
    case XOR: {
      c->r[dst] = c->r[src1] ^ c->r[src2];
      return;
    }

    // Logical
    case SEQ: {
      c->r[dst] = (sword_t)(c->r[src1]) == c->r[src2];
      return;
    }
    case SNE: {
      c->r[dst] = (sword_t)(c->r[src1]) != c->r[src2];
      return;
    }
    case SLT: {
      c->r[dst] = (sword_t)(c->r[src1]) < c->r[src2];
      return;
    }
    case SGT: {
      c->r[dst] = (sword_t)(c->r[src1]) > c->r[src2];
      return;
    }
    case SLE: {
      c->r[dst] = (sword_t)(c->r[src1]) <= c->r[src2];
      return;
    }
    case SGE: {
      c->r[dst] = (sword_t)(c->r[src1]) <= c->r[src2];
      return;
    }

    // Special
    case MOVI2S: {
      //TODO(au.zachary.forman) implement.
      FATAL("Unimplemented");
      return;
    }
    case MOVS2I: {
      //TODO(au.zachary.forman) implement.
      FATAL("Unimplemented");
      return;
    }

    default: {
      //TODO(au.zachary.forman) illegal instruction interrupt
      FATAL("No such opcode %X", op);
      return;
    }
  }
}