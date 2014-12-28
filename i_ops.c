// Fragment used to define all the I type ops.
// c is the cpu.
// op is the operation.

// Arithmetic
case ADDI: {
  sword_t a1, a2;
  int ret = -1;
  a1 = (sword_t)c->r[I_SRC(op)];
  a2 = I_SGN(I_IMM(op));
  // Integer overflow
  if (a1 > 0 && a2 > INT_MAX - a1) {
    ret = 3;
  }
  if (a1 < 0 && a2 < INT_MIN - a1) {
    ret = 3;
  }
  c->r[I_DST(op)] = a1 + a2;
  return ret;
}
case ADDUI: {
  c->r[I_DST(op)] = c->r[I_SRC(op)] + I_IMM(op);
  return -1;
}
case SUBI: {
  sword_t a1, a2;
  int ret = -1;
  a1 = (sword_t)c->r[I_SRC(op)];
  a2 = I_SGN(I_IMM(op));
  // Integer overflow
  if (a1 > 0 && (-a2) > INT_MAX - a1) {
    ret = 3;
  }
  if (a1 < 0 && (-a2) < INT_MIN - a1) {
    ret = 3;
  }
  c->r[I_DST(op)] = a1 - a2;
  return ret;
}
case SUBUI: {
  c->r[I_DST(op)] = c->r[I_SRC(op)] - I_IMM(op);
  return -1;
}

// Logical
case ANDI: {
  c->r[I_DST(op)] = c->r[I_SRC(op)] & I_IMM(op);
  return -1;
}
case ORI: {
  c->r[I_DST(op)] = c->r[I_SRC(op)] | I_IMM(op);
  return -1;
}
case XORI: {
  c->r[I_DST(op)] = c->r[I_SRC(op)] ^ I_IMM(op);
  return -1;
}

// Register control
case LHI: {
  c->r[I_DST(op)] = (c->r[I_SRC(op)]&0x0000FFFF) | (I_IMM(op) << 16);
  return -1;
}

// Control
case JR: {
  c->pc = c->r[I_SRC(op)];
  return -1;
}
case JALR: {
  c->r[31] = c->pc;
  c->pc = c->r[I_SRC(op)];
  return -1;
}

// Shifts
case SLAI: {
  // Note: DLX does only use the first 5 bits of imm.
  c->r[I_DST(op)] = c->r[I_SRC(op)] << (I_IMM(op) & 0x1F);
  return -1;
}
case SRLI: {
  // Note: DLX does only use the first 5 bits of imm.
  c->r[I_DST(op)] = c->r[I_SRC(op)] >> (I_IMM(op) & 0x1F);
  return -1;
}
case SRAI: {
  // Note: DLX does only use the first 5 bits of imm.
  c->r[I_DST(op)] = ((sword_t)c->r[I_SRC(op)]) >> (I_IMM(op) & 0x1F);
  return -1;
}

// Logical
case SEQI: {
  c->r[I_DST(op)] = c->r[I_SRC(op)] == I_SGN(I_IMM(op));
  return -1;
}
case SNEI: {
  c->r[I_DST(op)] = c->r[I_SRC(op)] != I_SGN(I_IMM(op));
  return -1;
}
case SLTI: {
  c->r[I_DST(op)] = ((sword_t)c->r[I_SRC(op)]) < I_SGN(I_IMM(op));
  return -1;
}
case SGTI: {
  c->r[I_DST(op)] = ((sword_t)c->r[I_SRC(op)]) > I_SGN(I_IMM(op));
  return -1;
}
case SLEI: {
  c->r[I_DST(op)] = ((sword_t)c->r[I_SRC(op)]) <= I_SGN(I_IMM(op));
  return -1;
}
case SGEI: {
  c->r[I_DST(op)] = ((sword_t)c->r[I_SRC(op)]) >= I_SGN(I_IMM(op));
  return -1;
}

// Memory control
case LB: {
  word_t addr = c->r[I_SRC(op)] + I_SGN(I_IMM(op));
  sword_t val = *(membyte(c->mem, addr));
  c->r[I_DST(op)] = (val & 0x00000080) ? (0xFFFFFF00 | val) : val;
  return -1;
}
case LH: {
  word_t addr = c->r[I_SRC(op)] + I_SGN(I_IMM(op));
  // Check for unaligned access
  if (addr % sizeof(halfword_t)) {
    return 1;
  }
  sword_t val = *(membyte(c->mem, addr));
  c->r[I_DST(op)] = (val & 0x00008000) ? (0xFFFF0000 | val) : val;
  return -1;
}
case LW: {
  word_t addr = c->r[I_SRC(op)] + I_SGN(I_IMM(op));
  // Check for unaligned access
  if (addr % sizeof(word_t)) {
    return 1;
  }
  c->r[I_DST(op)] = *(memword(c->mem, addr));
  return -1;
}
case LBU: {
  word_t addr = c->r[I_SRC(op)] + I_SGN(I_IMM(op));
  c->r[I_DST(op)] = *(membyte(c->mem, addr));
  return -1;
}
case LHU: {
  word_t addr = c->r[I_SRC(op)] + I_SGN(I_IMM(op));
  // Check for unaligned access
  if (addr % sizeof(halfword_t)) {
    return 1;
  }
  c->r[I_DST(op)] = *(memhalfword(c->mem, addr));
  return -1;
}
case SB: {
  word_t addr = c->r[I_SRC(op)] + I_SGN(I_IMM(op));
  *(membyte(c->mem, addr)) = (byte_t)(c->r[I_SRC(op)] & 0x000000FF);
  return -1;
}
case SH: {
  word_t addr = c->r[I_SRC(op)] + I_SGN(I_IMM(op));
  // Check for unaligned access
  if (addr % sizeof(halfword_t)) {
    return 1;
  }
  *(memhalfword(c->mem, addr)) = (halfword_t)(c->r[I_SRC(op)] & 0x0000FFFF);
  return -1;
}
case SW: {
  word_t addr = c->r[I_SRC(op)] + I_SGN(I_IMM(op));
  // Check for unaligned access
  if (addr % sizeof(word_t)) {
    return 1;
  }
  *(memword(c->mem, addr)) = (word_t)(c->r[I_SRC(op)]);
  return -1;
}

// Logical
case SEQUI: {
  c->r[I_DST(op)] = c->r[I_SRC(op)] == I_IMM(op);
  return -1;
}
case SNEUI: {
  c->r[I_DST(op)] = c->r[I_SRC(op)] != I_IMM(op);
  return -1;
}
case SLTUI: {
  c->r[I_DST(op)] = ((sword_t)c->r[I_SRC(op)]) < I_IMM(op);
  return -1;
}
case SGTUI: {
  c->r[I_DST(op)] = ((sword_t)c->r[I_SRC(op)]) > I_IMM(op);
  return -1;
}
case SLEUI: {
  c->r[I_DST(op)] = ((sword_t)c->r[I_SRC(op)]) <= I_IMM(op);
  return -1;
}
case SGEUI: {
  c->r[I_DST(op)] = ((sword_t)c->r[I_SRC(op)]) >= I_IMM(op);
  return -1;
}
