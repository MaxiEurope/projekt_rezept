#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <bits/getopt_core.h>

int getargs(int argc, char *argv[], char **recipe_file) {
    int opt;

    while ((opt = getopt(argc, argv, "f:")) != -1) {
        switch (opt) {
        case 'f':
            *recipe_file = optarg;
            break;
        default: /* '?' */
            fprintf(stderr, "Verwendung: %s -f Rezeptdatei\n", argv[0]);
            return 1;
        }
    }

    if (*recipe_file == NULL) {
        fprintf(stderr, "Der Pfad für die Rezeptdatei muss hinter -f angegeben werden.\n");
        return 1;
    }

    if (strstr(*recipe_file, ".json") == NULL) {
        fprintf(stderr, "Die Rezeptdatei muss eine JSON-Datei sein.\n");
        return 1;
    }

    // printf("Rezeptdatei: %s\n", *recipe_file); // debug
    return 0;
}
