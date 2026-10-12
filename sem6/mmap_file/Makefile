.POSIX:

CC      = gcc
CFLAGS  = -Wall -Wextra -std=gnu23

.PHONY: all

all: rw_slice mm_slice

rw_slice: rw_slice.c
	$(CC) $< $(CFLAGS) -o $@	

mm_slice: mm_slice.c
	$(CC) $< $(CFLAGS) -o $@	

.PHONY: run

run: rw_slice mm_slice
	dd if=/dev/random of=64MiB.bin bs=1MiB count=64
	bash -c "time ./rw_slice 64MiB.bin 64MiB.rw_slice.1024.bin 1024"
	bash -c "time ./mm_slice 64MiB.bin 64MiB.mm_slice.1024.bin 1024"
	cmp 64MiB.rw_slice.1024.bin 64MiB.mm_slice.1024.bin

.PHONY: clean

clean:
	rm -f ./64MiB.rw_slice.1024.bin
	rm -f ./64MiB.mm_slice.1024.bin
	rm -f ./64MiB.bin
	rm -f ./rw_slice
	rm -f ./mm_slice
	
