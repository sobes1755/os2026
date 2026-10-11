.POSIX:

CC      = gcc
CFLAGS  = -Wall -Wextra -std=c23
LDFLAGS = 
LDLIBS  = 

.PHONY: all run clean

all: readdir

.c:
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@ $(LDLIBS)

readdir: readdir.c

run: readdir
	./readdir .
	./readdir ./*
	./readdir ..
	./readdir ../*

clean:
	-rm -f readdir
