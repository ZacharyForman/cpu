#ifndef _testing_test_h
#define _testing_test_h

// Macros for performing testing

#include <stdio.h>

// If expr != expect, prints a message to stderr.
// Tag is an int used to help differentiate tests.
#define REQUIRE(tag, expr, expect) \
  do { \
    if (expr != expect) { \
      fprintf(stderr, "%3d: FAIL %10s(0x%.2X) != %10s(0x%.2X)\n", \
        tag, #expr, expr, #expect, expect); \
    } \
  } while (0)

#endif // _testing_test_h
