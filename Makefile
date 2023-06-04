CC=gcc
ifndef CC_FLAGS
CC_FLAGS=-Wall -ggdb
endif

ifndef LD_FLAGS
LD_FLAGS=-I ./src/
endif

all: clean mman_example.o normal.o

mman.o:
	$(CC) $(CC_FLAGS) $(LD_FLAGS) -c ./src/mman.c

vmemory.o:
	$(CC) $(CC_FLAGS) $(LD_FLAGS) -c ./src/vmemory.c

normal.o: normal.c
	$(CC) $(CC_FLAGS) -o normal.o normal.c

mman_example.o: mman.o vmemory.o
	$(CC) $(CC_FLAGS) $(LD_FLAGS) -o mman_example.o mman_example.c mman.o vmemory.o

test_mman: mman.o vmemory.o
	$(CC) $(CC_FLAGS) $(LD_FLAGS) -o test_mman.o ./tests/test_mman.c mman.o vmemory.o && ./test_mman.o

test_vmemory: vmemory.o
	$(CC) $(CC_FLAGS) $(LD_FLAGS) -o test_vmemory.o ./tests/test_vmemory.c vmemory.o && ./test_vmemory.o

test: clean test_mman test_vmemory

clean:
	rm -f mman_example.o normal.o vmemory.o mman.o test_mman.o test_vmemory.o

.PHONY: all