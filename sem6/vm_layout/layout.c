#include <stdio.h>
#include <stdlib.h>

char global_1[] = "GLobal variable 1!";
char global_2[1024];
char global_3[] = "Global variable 3!";
char global_4[1024];

extern char **environ;

int
main(int argc, char *argv[], char *env[])
{
    char local_1[] = "Local variable 1";
    char local_2[1024];
    char local_3[] = "Local variable 3!";
    char local_4[1024];

    char *malloc_1 = malloc(64 * 1024);
    char *malloc_2 = malloc(256 * 1024);
    char *malloc_3 = malloc(32 * 1024);
    char *malloc_4 = malloc(512 * 1024);

    printf("Global 1: %p.\n", global_1);
    printf("Global 2: %p.\n", global_2);
    printf("Global 3: %p.\n", global_3);
    printf("Global 4: %p.\n\n", global_4);
    
    printf("Local 1: %p.\n", local_1);
    printf("Local 2: %p.\n", local_2);
    printf("Local 3: %p.\n", local_3);
    printf("Local 4: %p.\n\n", local_4);

    printf("Malloc 1: %p.\n", malloc_1);
    printf("Malloc 2: %p.\n", malloc_2);
    printf("Malloc 3: %p.\n", malloc_3);
    printf("Malloc 4: %p.\n\n", malloc_4);

    printf("argc: %p.\n", &argc);
    printf("argv: %p.\n", argv);
    printf("env: %p.\n\n", env);

    printf("environ: %p.\n\n", environ);
    
    for (char **ptr = argv; *ptr != nullptr; ptr++)
        printf("*%p = \"%s\".\n", ptr, *ptr);
    printf("\n");
    for (char **ptr = env; *ptr != nullptr; ptr++)
        printf("*%p = \"%s\".\n", ptr, *ptr);
    printf("\n");

    free(malloc_4);
    free(malloc_3);
    free(malloc_2);
    free(malloc_1);

    return EXIT_SUCCESS;
}
