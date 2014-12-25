CC=gcc
CFLAGS=-c -Wall

all: cpu.o

cpu.o: cpu.h cpu.c log.h opcodes.h
	CC -o bin/cpu.o $(CFLAGS) cpu.c

test: test_opcodes

test_opcodes: test/opcodes.c opcodes.h test.h
	$(CC) -o bin/test_opcodes test/opcodes.c
	bin/test_opcodes
	rm -f bin/test_opcodes

clean:
	rm -f bin/*