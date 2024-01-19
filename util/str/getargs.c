#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <bits/getopt_core.h>

void getargs(int argc, char *argv[], char **recipe_file) {
    int opt;
    char *recipe_file = NULL;

    while ((opt = getopt(argc, argv, "f:")) != -1) {
        switch (opt) {
        case 'f':
            recipe_file = optarg;
            break;
        default: /* '?' */
            fprintf(stderr, "Usage: %s -f recipe_file\n", argv[0]);
            return 1;
        }
    }

    if (recipe_file == NULL) {
        fprintf(stderr, "You must specify a recipe file with -f\n");
        return 1;
    }

    printf("Recipe file: %s\n", recipe_file);
}
