#ifndef _CPU_DISASSEMBLER_H
#define _CPU_DISASSEMBLER_H

// Interface of simple disassembler.

#include "types.h"

char **disassemble(word_t *instr, int len);

#endif // _CPU_DISASSEMBLER_H