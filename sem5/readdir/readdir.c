#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
void
list(const char *path)
{

    DIR *dirp = opendir(path);

    if (dirp == nullptr) {
        fprintf(stderr, "Opendir (%s) error (%d). %s!\n", path, errno, strerror(errno));
        return;
    }

    for (;;) {

        errno = 0;

        struct dirent *entp = readdir(dirp);

        if (entp == nullptr) {
            if (errno != 0) {
                fprintf(stderr, "Readdir (%s) error (%d). %s!\n", path, errno, strerror(errno));
            }
            break;
        }

        if (strcmp(entp->d_name, ".") == 0 || strcmp(entp->d_name, "..") == 0) {
            continue;
        }

        printf("%llu %s %s\n", (unsigned long long) entp->d_ino, path, entp->d_name);

    }

    if (closedir(dirp) == -1) {
        fprintf(stderr, "Closedir (%s) error (%d). %s!\n", path, errno, strerror(errno));
    }

    return;

}

int
main(int argc, char *argv[])
{

    if (argc == 1) {
        list(".");
    } else {
         for (argv++; *argv; argv++)
            list(*argv);
    }
    
    return EXIT_SUCCESS;

}
