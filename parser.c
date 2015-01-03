#include "types.h"
typedef struct _map *map;

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

// Parser that currently only compiles absolute segments.

// Whether to tokenize at this point
#define START 1
#define STOP -1

// Struct that represents the current parsing
// state of the entire input.
typedef struct _parse_state {
  // Symbol table of string : value
  map symbol_table;
  // Array representing the in memory output of the assembler.
  int len;
  word_t *memory;
  // Current pass of the parser.
  int pass;
  // The start of the current token.
  // -1 if not currently tokenizing.
  int token_start;
} parse_state;

// Struct representing the parse state
// of the current line.
typedef struct _state state;
struct _state {
  // The condition that must be true to advance
  // to this state. If st->condition(c) == 1,
  // then advancing to st is possible, else
  // it is not.
  int (*condition)(char c);
  // The series of next states available.
  // The pseudo state 'failed' is also available.
  int nextc;
  state **next;
  // The function to call from this state
  // on termination at this state.
  // If null, state is not terminal.
  // Modifies the parse state given to it.
  // Uses parse number pn to do work.
  void (*operate)(parse_state *ps, int pn);
  //  1 => start a token
  // -1 => stop a token
  //  0 => do nothing.
  int token;
};

// Always available to traverse
int (*epsilon)(char) = NULL;

// Macro to trivialize function declaration
#define COND_IN_STR(name, str) \
int name (char c) \
{ \
  int i; \
  int len = strlen(str); \
  for (i = 0; i < len; i++) { \
    if (c == str[i]) { \
      return 1; \
    } \
  } \
  return 0; \
}

// Useful condition functions
COND_IN_STR(lowercase, "abcdefghijklmnopqrstuvwxyz");
COND_IN_STR(uppercase, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
COND_IN_STR(ascii, "abcdefghijklmnopqrstuvwxyz" \
                   "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
COND_IN_STR(identifier, "abcdefghijklmnopqrstuvwxyz" \
                        "ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
                        "1234567890_");
COND_IN_STR(comma, ",");
COND_IN_STR(dot, ".");
COND_IN_STR(semicolon, ";");
COND_IN_STR(dollar, "$");
COND_IN_STR(whitespace, " \t");
COND_IN_STR(eol, "\n");
COND_IN_STR(number, "1234567890");
COND_IN_STR(register0, "r");
COND_IN_STR(register1a, "0123456789");
COND_IN_STR(register1b, "12");
COND_IN_STR(register1c, "3");
COND_IN_STR(register2b, "0123456789");
COND_IN_STR(register2c, "01");
COND_IN_STR(equ1, "e");
COND_IN_STR(equ2, "q");
COND_IN_STR(equ3, "u");
COND_IN_STR(space1, "s");
COND_IN_STR(space2, "p");
COND_IN_STR(space3, "a");
COND_IN_STR(space4, "c");
COND_IN_STR(space5, "e");
COND_IN_STR(word1, "w");
COND_IN_STR(word2, "o");
COND_IN_STR(word3, "r");
COND_IN_STR(word4, "d");
COND_IN_STR(half1, "h");
COND_IN_STR(half2, "a");
COND_IN_STR(half3, "l");
COND_IN_STR(half4, "f");
COND_IN_STR(byte1, "b");
COND_IN_STR(byte2, "y");
COND_IN_STR(byte3, "t");
COND_IN_STR(byte4, "e");
COND_IN_STR(uns, "u");


// Terminal functions

void apply_equ_num(parse_state *ps, int pn)
{

}

void apply_equ_ident(parse_state *ps, int pn)
{

}

void apply_space_num(parse_state *ps, int pn)
{

}

void apply_space_ident(parse_state *ps, int pn)
{

}

// Signed data

void apply_word_num(parse_state *ps, int pn)
{

}

void apply_word_ident(parse_state *ps, int pn)
{

}

void apply_half_num(parse_state *ps, int pn)
{

}

void apply_half_ident(parse_state *ps, int pn)
{

}

void apply_byte_num(parse_state *ps, int pn)
{

}

void apply_byte_ident(parse_state *ps, int pn)
{

}

// Unsigned data

void apply_wordu_num(parse_state *ps, int pn)
{

}

void apply_wordu_ident(parse_state *ps, int pn)
{

}

void apply_halfu_num(parse_state *ps, int pn)
{

}

void apply_halfu_ident(parse_state *ps, int pn)
{

}

void apply_byteu_num(parse_state *ps, int pn)
{

}

void apply_byteu_ident(parse_state *ps, int pn)
{

}


void write_r_instruction(parse_state *ps, int pn)
{

}

void write_i_instruction(parse_state *ps, int pn)
{

}

void write_l_instruction(parse_state *ps, int pn)
{

}

state *construct_state(int (*cond)(char), void (*op)(parse_state*, int), 
  int tok, int self, int len, ...)
{
  int i = 0;
  va_list states;

  state *ret = (state *) malloc(sizeof(state));
  ret->condition = cond;
  ret->operate = op;
  ret->nextc = len + self;
  // If we don't have any next states
  if (ret->nextc == 0) {
    ret->next = NULL;
    return;
  }

  // Construct the next states set.
  ret->next = (state **) malloc(sizeof(state) * ret->nextc);
  if (self) {
    ret->next[i++] = ret;
  }
  va_start(states, len);
  for (i = 0; i < ret->nextc; i++) {
    ret->next[i++] = va_arg(states, state*);
  }
  va_end(states);
  return ret;
}

state *make_instruction_state()
{


  state *base_state = construct_state(epsilon, NULL, 0, 0, 26,
    nop_state,
    halt_state,
    wait_state,
    add_state,
    sub_state,
    seq_state,
    sne_state,
    slt_state,
    sgt_state,
    sle_state,
    sge_state,
    sl_state,
    sr_state,
    and_state,
    or_state,
    xor_state,
    jr_state,
    jalr_state,
    lhi_state,
    load_state,
    j_state,
    jal_state,
    rfe_state,
    trap_state,
    movi2s_state,
    movs2i_state);
}

state *dlx_parsing_state()
{
  // TODO(zforman) Let you have arbitrary cool $ + 3, $ + 16#FF, etc. stuff
  state *equ_state 
    = construct_state(dot, NULL, 0, 0, 1,
        construct_state(equ1, NULL, 0, 0, 1,
          construct_state(equ2, NULL, 0, 0, 1,
            construct_state(equ3, NULL, 0, 0, 1,
              construct_state(whitespace, NULL, 0, 1, 2,
                construct_state(number, NULL, START, 1, 2,
                  construct_state(eol, apply_equ_num, STOP, 0, 0),
                  construct_state(whitespace, NULL, STOP, 1, 1,
                    construct_state(eol, apply_equ_num, 0, 0))),
                construct_state(ascii, NULL, START, 0, 1,
                  construct_state(identifier, NULL, 0, 1, 2,
                    construct_state(eol, apply_equ_ident, STOP, 0, 0),
                    construct_state(whitespace, NULL, STOP, 1, 1,
                      construct_state(eol, apply_equ_ident, 0, 0, 0))
      )))))));


  // TODO(zforman) Let you have arbitrary cool $ + 3, $ + 16#FF, etc. stuff
  state *space_state 
    = construct_state(dot, NULL, 0, 0, 1,
        construct_state(space1, NULL, 0, 0, 1,
          construct_state(space2, NULL, 0, 0, 1,
            construct_state(space3, NULL, 0, 0, 1,
              construct_state(space4, NULL, 0, 0, 1,
                construct_state(space5, NULL, 0, 0, 1,
                  construct_state(whitespace, NULL, 0, 1, 2,
                    construct_state(number, NULL, START, 1, 2,
                      construct_state(eol, apply_space_num, STOP, 0, 0),
                      construct_state(whitespace, NULL, STOP, 1, 1,
                        construct_state(eol, apply_space_num, 0, 0, 0))),
                    construct_state(ascii, NULL, START, 0, 3,
                      construct_state(identifier, NULL, 0, 1, 2,
                        construct_state(eol, apply_space_ident, STOP, 0, 0),
                        construct_state(whitespace, NULL, STOP, 1, 1,
                          construct_state(eol, apply_space_ident, 0, 0, 0))),
                      construct_state(eol, apply_space_ident, STOP, 0, 0),
                      construct_state(whitespace, NULL, STOP, 1, 1,
                        construct_state(eol, apply_space_ident, 0, 0, 0))
    ))))))));


  // TODO(zforman) Allow for multiple vars in one word_state.
  state *word_state
    = construct_state(dot, NULL, 0, 0, 1,
        construct_state(word1, NULL, 0, 0, 1,
          construct_state(word2, NULL, 0, 0, 1,
            construct_state(word3, NULL, 0, 0, 1,
              construct_state(word4, NULL, 0, 0, 2,
                construct_state(whitespace, NULL, 0, 1, 2,
                  construct_state(number, NULL, START, 1, 1,
                    construct_state(eol, apply_word_num, STOP, 0, 0)),
                  construct_state(ascii, NULL, START, 0, 1,
                    construct_state(identifier, NULL, 0, 1, 1,
                      construct_state(eol, apply_word_ident, STOP, 0, 0)))),
                construct_state(uns, NULL, 0, 0, 1,
                  construct_state(whitespace, NULL, 0, 1, 2,
                    construct_state(number, NULL, START, 1, 1,
                      construct_state(eol, apply_wordu_num, STOP, 0, 0)),
                    construct_state(ascii, NULL, START, 0, 1,
                      construct_state(identifier, NULL, 0, 1, 1,
                        construct_state(eol, apply_wordu_ident, STOP, 0, 0)))))
      )))));

  state *half_state
    = construct_state(dot, NULL, 0, 0, 1,
        construct_state(half1, NULL, 0, 0, 1,
          construct_state(half2, NULL, 0, 0, 1,
            construct_state(half3, NULL, 0, 0, 1,
              construct_state(half4, NULL, 0, 0, 2,
                construct_state(whitespace, NULL, 0, 1, 2,
                  construct_state(number, NULL, START, 1, 1,
                    construct_state(eol, apply_half_num, STOP, 0, 0)),
                  construct_state(ascii, NULL, START, 0, 1,
                    construct_state(identifier, NULL, 0, 1, 1,
                      construct_state(eol, apply_half_ident, STOP, 0, 0)))),
                construct_state(uns, NULL, 0, 0, 1,
                  construct_state(whitespace, NULL, 0, 1, 2,
                    construct_state(number, NULL, START, 1, 1,
                      construct_state(eol, apply_halfu_num, STOP, 0, 0)),
                    construct_state(ascii, NULL, START, 0, 1,
                      construct_state(identifier, NULL, 0, 1, 1,
                        construct_state(eol, apply_halfu_ident, STOP, 0, 0)))))
      )))));

  state *byte_state
    = construct_state(dot, NULL, 0, 0, 1,
        construct_state(byte1, NULL, 0, 0, 1,
          construct_state(byte2, NULL, 0, 0, 1,
            construct_state(byte3, NULL, 0, 0, 1,
              construct_state(byte4, NULL, 0, 0, 2,
                construct_state(whitespace, NULL, 0, 1, 2,
                  construct_state(number, NULL, START, 1, 1,
                    construct_state(eol, apply_byte_num, STOP, 0, 0)),
                  construct_state(ascii, NULL, START, 0, 1,
                    construct_state(identifier, NULL, 0, 1, 1,
                      construct_state(eol, apply_byte_ident, STOP, 0, 0)))),
                construct_state(uns, NULL, 0, 0, 1,
                  construct_state(whitespace, NULL, 0, 1, 2,
                    construct_state(number, NULL, START, 1, 1,
                      construct_state(eol, apply_byteu_num, STOP, 0, 0)),
                    construct_state(ascii, NULL, START, 0, 1,
                      construct_state(identifier, NULL, 0, 1, 1,
                        construct_state(eol, apply_byteu_ident, STOP, 0, 0)))))
      )))));


  state *post_label_state
    = construct_state(epsilon, NULL, 0, 0, 1, 
        space_state,
        word_state,
        half_state,
        byte_state,
        make_instruction_state()
      );

  state *optional_label_state 
    = construct_state(ascii, NULL, START, 0, 1,
        construct_state(identifier, NULL, 0, 1, 1,
          construct_state(whitespace, NULL, STOP, 1, 2,
            post_label_state,
            equ_state
      )));

  state *leading_whitespace_state
    = construct_state(whitespace, NULL, 0, 1, 2,
        optional_label_state,
        post_label_state
      );

  state *base_state
    = construct_state(epsilon, NULL, 0, 0, 3,
        leading_whitespace_state,
        optional_label_state,
        post_label_state
      );

    return base_state;
}

typedef struct _parser {
  // State machine representing the initial state
  state *st;
  // Parser state
  parse_state *ps;
} parser;

int pass_one(parser *p)
{

}

int pass_two(parser *p)
{

}