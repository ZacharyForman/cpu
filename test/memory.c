// Tests for memory.h and memory.c

#include "../memory.h"
#include "test.h"

// Test expected cases.
void test1()
{
  memory mem = new_memory();
  REQUIRE(1, mem != NULL, 1);
  REQUIRE(2, sizeof(*membyte(mem, 5)), 1);
  REQUIRE(3, sizeof(*memhalfword(mem, 6)), 2);
  REQUIRE(4, sizeof(*memword(mem, 8)), 4);
  free_memory(mem);
}

void test2()
{
  memory mem = new_memory();

  word_t *p1 = memword(mem, 12);
  word_t *p2 = memword(mem, 16);

  REQUIRE(1, p2-p1, 4);

  word_t *p3 = memword(mem, 0x1000000F);
  word_t *p4 = memword(mem, 0x10000FFF);
  REQUIRE(2, p4-p3, 0xFF0);

  free_memory(mem);
}

void test3()
{
  int i,j;
  word_t instructions[] = {
    0x20010005,
    0x20220003,
    0x00411822,
    0x8C040008,
    0x00000001,
    0xFFFFFFFF
  };
  memory mem = new_memory();
  load_memory(mem, 0, instructions, sizeof(instructions)/sizeof(word_t));

  for (i = 0; i < 5; i++) {
    for (j = 0; j < 5; j++) {
      REQUIRE(i*5+j, *memword(mem, j*4), instructions[j]);
    }
  }

  free_memory(mem);
}

int main(int argc, char **argv)
{
  test1();
  test2();
  test3();

  return 0;
}