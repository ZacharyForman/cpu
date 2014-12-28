// Fragment used to define all the L type ops.
// c is the cpu.
// op is the operation.

// Control instructions
case J: {
  int ret = -1;
  sword_t addr = c->pc + L_SGN(L_IMM(op));
  // Check for unaligned jump address.
  if (addr % 4) {
    ret = 1;
  }
  c->pc = addr;
  return ret;
}
case JAL: {
  int ret = -1;
  sword_t addr = c->pc + L_SGN(L_IMM(op));
  c->r[31] = c->pc;
  // Check for unaligned jump address.
  if (addr % 4) {
    ret = 1;
  }
  c->pc = addr;
  return ret;
}
case RFE: {
  // Privileged instruction.
  if ((c->s[PSW] & 0x4) == 0) {
    return 0;
  }
  c->pc = c->s[XAR];
  word_t psw = c->s[PSW];
  // set psw's usermode status to previous
  psw = (psw & (~0x2)) | ((psw & 0x4) >> 1);
  // set exceptions to on
  psw |= 0x1;
  c->s[PSW] = psw;
  return -1;
}
case TRAP: {
  // If trap is enabled, trap.
  if (c->s[PSW] & (1<<15)) {
    return 7;
  }
  // Illegal instruction.
  return 0;
}
