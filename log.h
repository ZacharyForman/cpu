#ifndef _CPU__LOGH
#define _CPU__LOGH

#include <stdio.h>
#include <stdlib.h>

#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL 0
#endif // DEBUG_LEVEL

// Generic logging function. Prefer to use a specific variant.
#define _LOG(tag, msg, level, ...) \
  do { \
    fprintf(stderr, "%s/%s" msg "\n", level, tag, ## __VA_ARGS__); \
  } while(0)

// Always have fatal messages on.
#define FATAL(msg, ...) \
  do { \
    _LOG(__FILE__, msg, "F", ## __VA_ARGS__); \
    exit(1); \
  } while(0)

// Always have error messages on.
#define ERROR(msg, ...) \
  do { \
    _LOG(__FILE__, msg, "E", ## __VA_ARGS__); \
  } while(0)

// Turn on warnings if DEBUG_LEVEL > 0
#if DEBUG_LEVEL > 0
#define WARNING(msg, ...) \
  do { \
    _LOG(__FILE__, msg, "W", ## __VA_ARGS__); \
  } while(0)
#else
#define WARNING(msg, ...)
#endif // DEBUG_LEVEL > 0

// Turn on informational messages if DEBUG_LEVEL > 1
#if DEBUG_LEVEL > 1
#define INFO(msg, ...) \
  do { \
    _LOG(__FILE__, msg, "I", ## __VA_ARGS__); \
  } while(0)
#else
#define INFO(msg, ...)
#endif // DEBUG_LEVEL > 1

// Turn on debugging messages if DEBUG_LEVEL > 2
#if DEBUG_LEVEL > 2
#define DEBUG(msg, ...) \
  do { \
    _LOG(__FILE__, msg, "D", ## __VA_ARGS__); \
  } while(0)
#else
#define DEBUG(msg, ...)
#endif // DEBUG_LEVEL > 2

// Turn on verbose messages if DEBUG_LEVEL > 3
#if DEBUG_LEVEL > 3
#define VERBOSE(msg, ...) \
  do { \
    _LOG(__FILE__, msg, "V", ## __VA_ARGS__); \
  } while(0)
#else
#define VERBOSE(msg, ...)
#endif // DEBUG_LEVEL > 1

#endif // _CPU__LOGH
