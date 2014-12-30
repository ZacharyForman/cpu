CC=gcc
CFLAGS=-c -Wall

all: bin/driver.o bin/cpu.o bin/memory.o bin/disassembler.o
	$(CC) -o bin/driver bin/driver.o bin/cpu.o bin/memory.o bin/disassembler.o

bin/driver.o: driver.c
	$(CC) -o bin/driver.o $(CFLAGS) driver.c

bin/cpu.o: cpu.h cpu.c log.h opcodes.h i_ops.c r_ops.c l_ops.c types.h
	$(CC) -o bin/cpu.o $(CFLAGS) cpu.c

bin/memory.o: memory.c memory.h types.h
	$(CC) -o bin/memory.o $(CFLAGS) memory.c

bin/disassembler.o: disassembler.c disassembler.h opcodes.h
	$(CC) -o bin/disassembler.o $(CFLAGS) disassembler.c

test: test_opcodes test_memory

test_opcodes: test/opcodes.c test/test.h opcodes.h
	$(CC) -o bin/test_opcodes test/opcodes.c
	bin/test_opcodes
	rm -f bin/test_opcodes

test_memory: test/memory.c test/test.h memory.h memory.c types.h
	$(CC) -o bin/test_memory test/memory.c memory.c
	bin/test_memory
	rm -f bin/test_memory

clean:
	rm -f bin/*