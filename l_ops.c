// Fragment used to define all the L type ops.
// c is the cpu.
// op is the operation.

// Control instructions
case J: {
  c->pc += L_SGN(L_IMM(op));
  return -1;
}
case JAL: {
  c->r[31] = c->pc;
  c->pc += L_SGN(L_IMM(op));
  return -1;
}
case RFE: {
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
  // If trap is enabled, trap, otherwise illegal instruction.
  if (c->s[PSW] & (1<<15)) {
    return 7;
  } else {
    return 0;
  }
  return -1;
}
