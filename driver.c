// Driver for CPU simulator

#include "cpu.h"
#include "disassembler.h"
#include "memory.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  int i;
  // Create objects
  memory mem = new_memory();
  cpu c = new_cpu(mem);

  // Write program
  word_t instructions[] = {
    0x20010005,
    0x20220003,
    0x00411822,
    0x8C040008,
    0x00000001,
    0xFFFFFFFF
  };

  // Load it into the memory
  load_memory(mem, 0, instructions, sizeof(instructions)/sizeof(word_t));

  printf("Program to be executed:\n");
  char **s = disassemble(instructions, 5);
  for (i = 0; i < 5; i++) {
    printf("%s\n", s[i]);
    free(s[i]);
  }
  free(s);
  printf("\n");

  // Inspect memory
  print_words(stdout, mem, 0, 10);
  
  // Execute it
  cycle(c);
  cycle(c);
  cycle(c);
  cycle(c);
  cycle(c);
  cycle(c);

  // Inspect results
  print_cpu_details(stdout, c);

  // Free objects
  free_cpu(c);
  free_memory(mem);

  return 0;
}
