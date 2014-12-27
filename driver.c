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
  *memword(mem,  0) = 0x20010005;
  *memword(mem,  4) = 0x20220003;
  *memword(mem,  8) = 0x00411822;
  *memword(mem, 12) = 0x8C040008;

  // Inspect memory
  printf("Memory location %d = %X\n",  0, *memword(mem,  0));
  printf("Memory location %d = %X\n",  4, *memword(mem,  4));
  printf("Memory location %d = %X\n",  8, *memword(mem,  8));
  printf("Memory location %d = %X\n", 12, *memword(mem, 12));
  
  // Execute it
  cycle(c);
  cycle(c);
  cycle(c);
  cycle(c);

  // Inspect results
  printf("r1 = %d\n", read_register(c, 1));
  printf("r2 = %d\n", read_register(c, 2));
  printf("r3 = %d\n", read_register(c, 3));
  printf("r4 = %X\n", read_register(c, 4));

  // Free objects
  free_cpu(c);
  free_memory(mem);

  return 0;
}