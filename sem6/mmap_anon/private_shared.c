#include "private_shared.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{

    if (argc < 5) {
        fprintf(stderr, "Usage: %s MAP_PRIVATE/MAP_SHARED size parent.bin child.bin\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    printf("Parent process (pid = %d, ppid = %d) started.\n", getpid(), getppid());

    char *addr = nullptr;
    size_t length = strtol(argv[2], nullptr, 10);
    int prot = PROT_READ | PROT_WRITE;
    int flags = MAP_ANONYMOUS;
    int fd = -1;
    off_t offset = 0;

    char *parentfile = argv[3];
    char *childfile = argv[4];

    if (strcmp(argv[1], "MAP_PRIVATE") == 0)
        flags |= MAP_PRIVATE;
    else if (strcmp(argv[1], "MAP_SHARED") == 0)
        flags |= MAP_SHARED;

    printf("Parent is making anonymous mapping...\n");

    char *buf = mmap(addr, length, prot, flags, fd, offset);

    if (buf == MAP_FAILED) {
        fprintf(stderr, "MMAP failed (%d)! %s!\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    printf("Parent is randomizing memory...\n");

    srand(time(nullptr));
    rand_buf(buf, length);

    printf("Parent is forking...\n");

    switch (fork()) {
        case -1:
            fprintf(stderr, "FORK failed (%d)! %s!\n", errno, strerror(errno));
            exit(EXIT_FAILURE);

        case 0:
            printf("Child (pid = %d, ppid = %d) started.\n", getpid(), getppid());

            printf("Child is randomizing memory...\n");
            rand_buf(buf, length);

            printf("Child is writing to child file...\n");
            save_buf_to_file(buf, length, childfile);

            break;

        default:
            printf("Parent started to wait for child...\n");
            if (wait(nullptr) == -1) {
                fprintf(stderr, "WAIT failed (%d)! %s!\n", errno, strerror(errno));
                exit(EXIT_FAILURE);
            }
            printf("Parent finished to wait for child...\n");

            printf("Parent is writing to parent file...\n");
            save_buf_to_file(buf, length, parentfile);

            break;
    }

    printf("Process (pid = %d, ppid = %d) is finishing...\n", getpid(), getppid());

    if (munmap(buf, length) == -1) {
        fprintf(stderr, "MUNMAP failed (%d)! %s!\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
 
}

int
save_buf_to_file(char *buf, size_t size, char *file)
{
    int fd = creat(file, 0666);

    if (fd == -1) {
        fprintf(stderr, "CREAT failed (%d)! %s!\n", errno, strerror(errno));
        return -1;
    }

    if (write(fd, buf, size) < (ssize_t) size) {
        fprintf(stderr, "WRITE failed (%d)! %s!\n", errno, strerror(errno));
        return -1;
    }

    if (close(fd) == -1) {
        fprintf(stderr, "CLOSE failed (%d)! %s!\n", errno, strerror(errno));
        return -1;
    }

    return 0;
}

void
rand_buf(char *buf, size_t size)
{
    for (size_t i = 0; i < size; i ++)
        *(buf + i) = rand();
}