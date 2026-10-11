/*

NAME
mktime - TODO

SYNOPSIS
mktime -Y 2026 -M 10 -D 05 -h 12 -m 15 -s 0

DESCRIPTION
TODO

*/

#include <getopt.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

extern int optind, opterr, optopt;
extern char *optarg;

int
main(int argc, char *argv[])
{

    struct tm tm = { .tm_isdst = -1, };

    int opt;

    while ((opt = getopt(argc, argv, ":Y:M:D:h:m:s:")) != -1) {

        switch (opt) {
        case 'Y':
            tm.tm_year = strtol(optarg, nullptr, 10) - 1900;
            break;
        case 'M':
            tm.tm_mon = strtol(optarg, nullptr, 10) - 1;
            break;
        case 'D':
            tm.tm_mday = strtol(optarg, nullptr, 10);
            break;
        case 'h':
            tm.tm_hour = strtol(optarg, nullptr, 10);
            break;
        case 'm':
            tm.tm_min = strtol(optarg, nullptr, 10);
            break;
        case 's':
            tm.tm_sec = strtol(optarg, nullptr, 10);
            break;
        default:
            fprintf(stderr, "Unrecognized option (%c)!\n", optopt);
            return EXIT_FAILURE;
        }

    }

    printf("Input (by getopt):\n\n");

    printf("tm_year = %d,\n", tm.tm_year);
    printf("tm_mon = %d,\n", tm.tm_mon);
    printf("tm_mday = %d,\n", tm.tm_mday);
    printf("tm_hour = %d,\n", tm.tm_hour);
    printf("tm_min = %d,\n", tm.tm_min);
    printf("tm_sec = %d,\n", tm.tm_sec);
    printf("tm_sdst = %d.\n\n", tm.tm_isdst);

    //

    time_t t = mktime(&tm);

    printf("Output (by mktime):\n\n");

    printf("mktime = %ld,\n\n", (long) t);

    printf("tm_year = %d,\n", tm.tm_year);
    printf("tm_yday = %d,\n", tm.tm_yday);
    printf("tm_mon = %d,\n", tm.tm_mon);
    printf("tm_mday = %d,\n", tm.tm_mday);
    printf("tm_wday = %d,\n", tm.tm_wday);
    printf("tm_hour = %d,\n", tm.tm_hour);
    printf("tm_min = %d,\n", tm.tm_min);
    printf("tm_sec = %d,\n", tm.tm_sec);
    printf("tm_sdst = %d,\n\n", tm.tm_isdst);

    //

    if (setlocale(LC_ALL, "") == nullptr) {
        fprintf(stderr, "Setlocale error!\n");
    }

    //
    
    char *asctm = asctime(&tm);
    printf("asctime = %.*s,\n", (int) (strlen(asctm) - 1), asctm);

    char str[256];
    strftime(str, sizeof(str), "%c", &tm);
    printf("strftime = %s,\n\n", str);

    //

    struct tm *gmtm = gmtime(&t);
    char *ascgm = asctime(gmtm);
    printf("asctime(gmtime(mktime)) = %.*s.\n", (int) (strlen(ascgm) - 1), ascgm);

    struct tm *localtm = localtime(&t);
    char *asclocal = asctime(localtm);
    printf("asctime(localtime(mktime)) = %.*s.\n", (int) (strlen(asclocal) - 1), asclocal);

    return EXIT_SUCCESS;

}
