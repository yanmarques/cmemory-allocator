CC=gcc
ifndef CC_FLAGS
CC_FLAGS=-Wall -ggdb
endif

all: normal live

live:
	$(CC) $(CC_FLAGS) -o live.o live.c

normal: normal.c
	$(CC) $(CC_FLAGS) -o normal.o normal.c

.PHONY: all