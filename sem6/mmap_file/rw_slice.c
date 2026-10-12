#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
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

    // Opening...

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

    int ofd = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC | O_APPEND, 0666);

    if (ofd == -1) {
        fprintf(stderr, "CREAT (of) failed (%d)! %s!\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Naive slicing...

    off_t step = strtol(argv[3], nullptr, 10);

    for (off_t offset = 0; offset < step; offset ++)
        for (off_t ifpos = lseek(ifd, offset, SEEK_SET); ifpos < length; ifpos = lseek(ifd, step - 1, SEEK_CUR)) {
            char c;
            read(ifd, &c, 1);    // TODO: error checking...
            write(ofd, &c, 1);    // TODO: error checking...
        }

    // Closing...

    if (close(ofd) == -1) {
        fprintf(stderr, "CLOSE (if) failed (%d)! %s!\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (close(ifd) == -1) {
        fprintf(stderr, "CLOSE (if) failed (%d)! %s!\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
 
}
