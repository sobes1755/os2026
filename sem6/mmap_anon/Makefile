.POSIX:

CC      = gcc
CFLAGS  = -Wall -Wextra -std=gnu23

.PHONY: all

all: private_shared

private_shared: private_shared.c private_shared.h
	$(CC) $< $(CFLAGS) -o $@	

.PHONY: clean

run: private_shared
	./private_shared MAP_PRIVATE 268435456 p_p_256.bin ch_p_256.bin
	-cmp p_p_256.bin ch_p_256.bin
	./private_shared MAP_SHARED 268435456 p_sh_256.bin ch_sh_256.bin
	-cmp p_sh_256.bin ch_sh_256.bin

clean:
	rm -f ./private_shared
