CC=gcc
CFLAGS=-I.

SRCDIR = ./src
ODIR = ./build

_DEPS = blockcipher.h
_OBJS = blockcipher.o main.o
_SRC  = blockcipher.c main.c

DEPS = $(patsubst %, $(SRCDIR)/%, $(_DEPS));
OBJS = $(patsubst %, $(ODIR)/%, $(_OBJS));
SRC  = $(patsubst %, $(SRCDIR)/%, $(_SRC));

$(ODIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

blockcipher: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f ./blockcipher
	rm -f $(ODIR)/*.o

