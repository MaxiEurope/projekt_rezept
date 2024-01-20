#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

/**
 * @brief Reads a file and returns its content
 * 
 * @param filepath Path to the file
 * @return Pointer to the string containing the content
 */
char* readfile(const char* filepath) {
    FILE* file = fopen(filepath, "r");
    if (file == NULL) {
        clear();
        printw("Ein Fehler ist beim Lesen der Datei aufgetreten, die Datei scheint nicht zu existieren.\n");
        refresh();
        napms(2000);
        refresh();
        exit(1);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* json_data = (char*)malloc(file_size + 1);
    fread(json_data, 1, file_size, file);
    json_data[file_size] = '\0';

    fclose(file);
    return json_data;
}
