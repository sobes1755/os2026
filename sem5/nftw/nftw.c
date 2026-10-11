#define _XOPEN_SOURCE 700

#include <errno.h>
#include <ftw.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int                      
print_dentry(const char *pathname, const struct stat *stat, int type, struct FTW *ftwb)
{

    if (type != FTW_NS) {

        switch (stat->st_mode & S_IFMT)    {    
        case S_IFREG:
            printf("r "); break;
            case S_IFDIR: 
            printf("d "); break;
        case S_IFCHR: 
            printf("c "); break;
        case S_IFBLK:
            printf("b "); break;
        case S_IFLNK: 
            printf("l "); break;
        case S_IFIFO: 
            printf("p "); break;
        case S_IFSOCK:
            printf("s "); break;
        default:
            printf("? "); break;
        }

        printf("%8llu ", (unsigned long long) stat->st_ino);
        printf("%8lld ", (long long) stat->st_size);

        struct passwd *p = getpwuid(stat->st_uid);
        struct group *g = getgrgid(stat->st_gid);

        if (p != nullptr)
            printf("%12s ", p->pw_name);
        else
            printf("%12s ", "");

        if (g != nullptr)
            printf("%12s ", g->gr_name);
        else
            printf("%12s ", "");        

    } else {

        printf("%2s ", "");
        printf("%8s ", "");
        printf("%8s ", "");
        printf("%12s ", "");
        printf("%12s ", "");

    }

    printf("%*s %s\n", 4 * ftwb->level, "", &pathname[ftwb->base]);

    return 0;

}

int
main(int argc, char *argv[])
{

    int fdmax = 16;
    int flags = FTW_PHYS;

    if (nftw((argc > 1) ? argv[1] : ".", print_dentry, fdmax, flags) == -1) {

        fprintf(stderr, "nftw error (%d). %s!\n", errno, strerror(errno));
        return EXIT_FAILURE;

    }

    return EXIT_SUCCESS;

}
