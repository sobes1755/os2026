.POSIX:

CC      = gcc
CFLAGS  = -Wall -Wextra -std=c23

.PHONY: all

all: layout

layout: layout.c
	$(CC) $< $(CFLAGS) -o $@	

.PHONY: run

run: layout
	./layout

.PHONY: clean

clean:
	rm -f ./layout
