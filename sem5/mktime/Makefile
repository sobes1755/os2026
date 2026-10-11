.POSIX:

CC      = gcc
CFLAGS  = -Wall -Wextra -std=c23
LDFLAGS = 
LDLIBS  = 

.PHONY: all run clean

all: mktime

.c:
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@ $(LDLIBS)

mktime: mktime.c

run: mktime
	@echo
	@echo === Mktime ===
	@echo
	./mktime -Y 1970 -M 1 -D 1 -h 00 -m 00 -s 00
	@echo
	@echo === Mktime with Europe/Berlin timezone ===
	@echo
	TZ="Europe/Berlin" ./mktime -Y 1970 -M 1 -D 1 -h 00 -m 00 -s 00
	@echo
	@echo === Mktime with Russian locale ===
	@echo
	LANG="ru_RU.utf8" ./mktime -Y 1970 -M 1 -D 1 -h 00 -m 00 -s 00

clean:
	-rm -f mktime
