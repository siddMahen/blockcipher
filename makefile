CC = gcc
CFLAGS = -I.

OBJS = ./build/main.o

blockcipher: $(OBJS)
	$(CC) -o $@ $(CFLAGS) $(OBJS)

./build/main.o:
	$(CC) -c ./src/main.c
	@mv main.o ./build/main.o

.PHONY: clean clean-all 

clean:
	rm ./build/*

clean-all:
	rm ./build/*
	rm blockcipher 
