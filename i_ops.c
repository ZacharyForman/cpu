// Fragment used to define all the I type ops.
// c is the cpu.
// op is the operation.

// Arithmetic
case ADDI: {
  // TODO(au.zachary.forman) trap on overflow
  c->r[I_DST(op)] = c->r[I_SRC(op)] + I_SGN(I_IMM(op));
  return;
}
case ADDUI: {
  c->r[I_DST(op)] = c->r[I_SRC(op)] + I_IMM(op);
  return;
}
case SUBI: {
  // TODO(au.zachary.forman) trap on overflow
  c->r[I_DST(op)] = c->r[I_SRC(op)] - I_SGN(I_IMM(op));
  return;
}
case SUBUI: {
  c->r[I_DST(op)] = c->r[I_SRC(op)] - I_IMM(op);
  return;
}

// Logical
case ANDI: {
  c->r[I_DST(op)] = c->r[I_SRC(op)] & I_IMM(op);
  return;
}
case ORI: {
  c->r[I_DST(op)] = c->r[I_SRC(op)] | I_IMM(op);
  return;
}
case XORI: {
  c->r[I_DST(op)] = c->r[I_SRC(op)] ^ I_IMM(op);
  return;
}

// Register control
case LHI: {
  c->r[I_DST(op)] = (c->r[I_SRC(op)]&0x0000FFFF) | (I_IMM(op) << 16);
  return;
}

// Control
case JR: {
  c->pc = c->r[I_SRC(op)];
  return;
}
case JALR: {
  c->r[31] = c->pc;
  c->pc = c->r[I_SRC(op)];
  return;
}

// Shifts
case SLAI: {
  // Note: DLX does only use the first 5 bits of imm.
  c->r[I_DST(op)] = c->r[I_SRC(op)] << (I_IMM(op) & 0x1F);
  return;
}
case SRLI: {
  // Note: DLX does only use the first 5 bits of imm.
  c->r[I_DST(op)] = c->r[I_SRC(op)] >> (I_IMM(op) & 0x1F);
  return;
}
case SRAI: {
  // Note: DLX does only use the first 5 bits of imm.
  c->r[I_DST(op)] = ((sword_t)c->r[I_SRC(op)]) >> (I_IMM(op) & 0x1F);
  return;
}

// Logical
case SEQI: {
  c->r[I_DST(op)] = c->r[I_SRC(op)] == I_SGN(I_IMM(op));
  return;
}
case SNEI: {
  c->r[I_DST(op)] = c->r[I_SRC(op)] != I_SGN(I_IMM(op));
  return;
}
case SLTI: {
  c->r[I_DST(op)] = ((sword_t)c->r[I_SRC(op)]) < I_SGN(I_IMM(op));
  return;
}
case SGTI: {
  c->r[I_DST(op)] = ((sword_t)c->r[I_SRC(op)]) > I_SGN(I_IMM(op));
  return;
}
case SLEI: {
  c->r[I_DST(op)] = ((sword_t)c->r[I_SRC(op)]) <= I_SGN(I_IMM(op));
  return;
}
case SGEI: {
  c->r[I_DST(op)] = ((sword_t)c->r[I_SRC(op)]) >= I_SGN(I_IMM(op));
  return;
}

// Memory control
case LB: {
  sword_t val = *(membyte(c->mem, c->r[I_SRC(op)] + I_SGN(I_IMM(op))));
  c->r[I_DST(op)] = (val & 0x00000080) ? (0xFFFFFF00 | val) : val;
  return;
}
case LH: {
  sword_t val = *(membyte(c->mem, c->r[I_SRC(op)] + I_SGN(I_IMM(op))));
  c->r[I_DST(op)] = (val & 0x00008000) ? (0xFFFF0000 | val) : val;
  return;
}
case LW: {
  c->r[I_DST(op)] = *(memword(c->mem, c->r[I_SRC(op)] + I_SGN(I_IMM(op))));
  return;
}
case LBU: {
  c->r[I_DST(op)] = *(membyte(c->mem, c->r[I_SRC(op)] + I_SGN(I_IMM(op))));
  return;
}
case LHU: {
  c->r[I_DST(op)] = *(memhalfword(c->mem, c->r[I_SRC(op)] + I_SGN(I_IMM(op))));
  return;
}
case SB: {
  // TODO(au.zachary.forman) Implement memory protection
  *(membyte(c->mem, c->r[I_DST(op)] + I_SGN(I_IMM(op))))
    = (byte_t)(c->r[I_SRC(op)] & 0x000000FF);
  return;
}
case SH: {
  // TODO(au.zachary.forman) Implement memory protection
  *(memhalfword(c->mem, c->r[I_DST(op)] + I_SGN(I_IMM(op))))
    = (halfword_t)(c->r[I_SRC(op)] & 0x0000FFFF);
  return;
}
case SW: {
  // TODO(au.zachary.forman) Implement memory protection
  *(memword(c->mem, c->r[I_DST(op)] + I_SGN(I_IMM(op))))
    = (word_t)(c->r[I_SRC(op)]);
  return;
}

// Logical
case SEQUI: {
  c->r[I_DST(op)] = c->r[I_SRC(op)] == I_IMM(op);
  return;
}
case SNEUI: {
  c->r[I_DST(op)] = c->r[I_SRC(op)] != I_IMM(op);
  return;
}
case SLTUI: {
  c->r[I_DST(op)] = ((sword_t)c->r[I_SRC(op)]) < I_IMM(op);
  return;
}
case SGTUI: {
  c->r[I_DST(op)] = ((sword_t)c->r[I_SRC(op)]) > I_IMM(op);
  return;
}
case SLEUI: {
  c->r[I_DST(op)] = ((sword_t)c->r[I_SRC(op)]) <= I_IMM(op);
  return;
}
case SGEUI: {
  c->r[I_DST(op)] = ((sword_t)c->r[I_SRC(op)]) >= I_IMM(op);
  return;
}
