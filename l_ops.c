// Fragment used to define all the L type ops.
// c is the cpu.
// op is the operation.

// Control instructions
case J: {
  c->pc += L_SGN(L_IMM(op));
  return;
}
case JAL: {
  c->r[31] = c->pc;
  c->pc += L_SGN(L_IMM(op));
  return;
}
case RFE: {
  c->pc = c->s[XAR];
  word_t psw = c->s[PSW];
  // set psw's usermode status to previous
  psw = (psw & (~0x2)) | ((psw & 0x4) >> 1);
  // set exceptions to on
  psw |= 0x1;
  c->s[PSW] = psw;
  return;
}
case TRAP: {
  // TODO(au.zachary.forman): Enable exceptions
  FATAL("Exceptions are not yet enabled");
  return;
}
