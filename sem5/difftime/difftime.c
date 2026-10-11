/*

NAME
difftime - TODO

SYNOPSIS
difftime "yyyy-mm-dd hh:mm:ss" "yyyy-mm-dd hh:mm:ss"

DESCRIPTION
TODO

*/

#define _XOPEN_SOURCE

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{

    if (argc <= 2) {
        return EXIT_FAILURE;
    }

    struct tm ta = {};
    struct tm tb = {};

    strptime(argv[1], "%F %T", &ta);
    strptime(argv[2], "%F %T", &tb);

    time_t a = mktime(&ta);
    time_t b = mktime(&tb);

    double c = difftime(b, a);

    printf("%.0F (seconds)\n", c);
    printf("%.2f (minutes)\n", c / 60);
    printf("%.2f (hours)\n", c / 3600);
    printf("%.2f (days)\n", c / 86400);

    return EXIT_SUCCESS;

}
