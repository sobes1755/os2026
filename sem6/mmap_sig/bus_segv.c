#define _XOPEN_SOURCE 500

#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{

    if (argc < 5) {
        fprintf(stderr, "Usage: %s filename filesize mapsize position\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Creating file...

    int fd = open(argv[1], O_CREAT | O_RDWR | O_TRUNC, 0666);

    if (fd == -1) {
        fprintf(stderr, "CREAT failed (%d)! %s!\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    off_t filesize = strtol(argv[2], nullptr, 10);

    if (ftruncate(fd, filesize) == -1) {
        fprintf(stderr, "FTRUNCATE failed (%d)! %s!\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    size_t mapsize = strtol(argv[3], nullptr, 10);
    
    char *map = mmap(nullptr, mapsize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (map == MAP_FAILED) {
        fprintf(stderr, "MMAP failed (%d)! %s!\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (close(fd) == -1) {
        fprintf(stderr, "CLOSE failed (%d)! %s!\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // SIGBUS or SIGSEGV...

    size_t position = strtol(argv[4], nullptr, 10);

    printf("filename = \"%s\"\n", argv[1]);
    printf("filesize = %ld\n", filesize);
    printf("mapsize = %ld\n", mapsize);
    printf("position = %ld\n", position);

    map[position] = 'Z';

    printf("map[%ld] = \"%c\".\n", position, map[position]);

    // Unmapping...

    if (munmap(map, mapsize) == -1) {
        fprintf(stderr, "MUNMAP failed (%d)! %s!\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
 
}

