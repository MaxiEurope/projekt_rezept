#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <bits/getopt_core.h>

/**
 * @brief Parses cmd line args
 * 
 * @param argc The number of cmd line args
 * @param argv Array of cmd line args
 * @param recipe_file Path to the recipe file, NULL at first
 * @return True if parsing was successful, false otherwise
 */
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
