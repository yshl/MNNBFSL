CC=gcc
CFLAGS=-Wall -O3 -DDEBUG
DC=dmd
DFLAGS=-O -release

all: mnnbfsl mnnbfsld

mnnbfsl: mnnbfsl.c
	$(CC) $(CFLAGS) -o $@ $<
mnnbfsld: mnnbfsl.d
	$(DC) $(DFLAGS) -of$@ $<

clean:
	$(RM) mnnbfsl mnnbfsld
