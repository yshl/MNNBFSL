CC=gcc
CFLAGS=-Wall -O3 -DDEBUG

all: mnnbfsl

mnnbfsl: mnnbfsl.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	$(RM) mnnbfsl
