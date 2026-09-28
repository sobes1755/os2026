.POSIX:

CC      = gcc
CFLAGS  = -Wall -Wextra -std=gnu23
LDFLAGS = 
LDLIBS  = 

.PHONY: all run clean

all: stat

.c:
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@ $(LDLIBS)

stat: stat.c

run: stat
	./stat

clean:
	-rm -f stat
