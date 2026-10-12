#define _XOPEN_SOURCE 500    // ftruncate

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{

    if (argc < 4) {
        fprintf(stderr, "Usage: %s if of step\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Mapping ifd to ifmap...

    int ifd = open(argv[1], O_RDONLY);

    if (ifd == -1) {
        fprintf(stderr, "OPEN (if) failed (%d)! %s!\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    struct stat ifs;

    if (fstat(ifd, &ifs) == -1) {
        fprintf(stderr, "FSTAT failed (%d)! %s!\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    off_t length = ifs.st_size;

    char *ifmap = mmap(nullptr, length, PROT_READ, MAP_PRIVATE, ifd, 0);

    if (ifmap == MAP_FAILED) {
        fprintf(stderr, "MMAP (if) failed (%d)! %s!\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (close(ifd) == -1) {
        fprintf(stderr, "CLOSE (if) failed (%d)! %s!\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Mapping ofd to ofmap...

    int ofd = open(argv[2], O_CREAT | O_RDWR | O_TRUNC, 0666);

    if (ofd == -1) {
        fprintf(stderr, "CREAT (of) failed (%d)! %s!\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (ftruncate(ofd, length) == -1) {
        fprintf(stderr, "FTRUNCATE (of) failed (%d)! %s!\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    char *ofmap = mmap(nullptr, length, PROT_READ | PROT_WRITE, MAP_SHARED, ofd, 0);

    if (ofmap == MAP_FAILED) {
        fprintf(stderr, "MMAP (of) failed (%d)! %s!\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (close(ofd) == -1) {
        fprintf(stderr, "CLOSE (if) failed (%d)! %s!\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Slicing...

    off_t step = strtol(argv[3], nullptr, 10);

    off_t ofpos = 0;

    for (off_t offset = 0; offset < step; offset ++)
        for (off_t ifpos = offset; ifpos < length; ifpos += step)
            ofmap[ofpos++] = ifmap[ifpos];

    // Unmapping...

    if (munmap(ofmap, length) == -1) {
        fprintf(stderr, "MUNMAP (of) failed (%d)! %s!\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (munmap(ifmap, length) == -1) {
        fprintf(stderr, "MUNMAP (if) failed (%d)! %s!\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
 
}
