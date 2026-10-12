.POSIX:

CC      = gcc
CFLAGS  = -Wall -Wextra -std=c23

.PHONY: all

all: bus_segv

bus_segv: bus_segv.c
	$(CC) $< $(CFLAGS) -o $@	

.PHONY: run

run: bus_segv
	-./bus_segv 1024.bin 2048 8192 1024
	-./bus_segv 3072.bin 2048 8192 3072
	-./bus_segv 7168.bin 2048 8192 7168
	-./bus_segv 16384.bin 2048 8192 16384

.PHONY: clean

clean:
	rm -f ./1024.bin
	rm -f ./3072.bin
	rm -f ./7168.bin
	rm -f ./16384.bin
	rm -f ./bus_segv
	