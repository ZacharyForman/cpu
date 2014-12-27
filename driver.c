// Driver for CPU simulator

#include "cpu.h"
#include "memory.h"

#include <stdio.h>

int main(int argc, char **argv)
{
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

  // Inspect memory
  print_words(mem, 0, 10);
  
  // Execute it
  cycle(c);
  cycle(c);
  cycle(c);
  cycle(c);
  cycle(c);
  cycle(c);

  // Inspect results
  print_cpu_details(c);

  // Free objects
  free_cpu(c);
  free_memory(mem);

  return 0;
}
