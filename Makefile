CC=gcc
CFLAGS=-Wall -O2 -DDEBUG

all: mnnbfsl

mnnbfsl: mnnbfsl.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	$(RM) mnnbfsl
