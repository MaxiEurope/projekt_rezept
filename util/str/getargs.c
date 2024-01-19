#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <bits/getopt_core.h>

bool getargs(int argc, char *argv[], char **recipe_file) {
    int opt;

    while ((opt = getopt(argc, argv, "f:")) != -1) {
        switch (opt) {
        case 'f':
            *recipe_file = optarg;
            break;
        default: /* '?' */
            fprintf(stderr, "Verwendung: %s -f Rezeptdatei\n", argv[0]);
            return false;
        }
    }

    if (*recipe_file == NULL) {
        fprintf(stderr, "Der Pfad für die Rezeptdatei muss hinter -f angegeben werden.\n");
        return false;
    }

    if (strstr(*recipe_file, ".json") == NULL) {
        fprintf(stderr, "Die Rezeptdatei muss eine JSON-Datei sein.\n");
        return false;
    }

    // printf("Rezeptdatei: %s\n", *recipe_file); // debug
    return true;
}
