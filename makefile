CC=gcc
CFLAGS=-c -Wall

all: bin/cpu.o bin/memory.o

bin/cpu.o: cpu.h cpu.c log.h opcodes.h
	CC -o bin/cpu.o $(CFLAGS) cpu.c

bin/memory.o: memory.c memory.h types.h
	CC -o bin/memory.o $(CFLAGS) memory.c

test: test_opcodes

test_opcodes: test/opcodes.c test/test.h opcodes.h
	$(CC) -o bin/test_opcodes test/opcodes.c
	bin/test_opcodes
	rm -f bin/test_opcodes

clean:
	rm -f bin/*