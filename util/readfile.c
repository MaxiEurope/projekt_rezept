#include <stdio.h>
#include <stdlib.h>

char* readfile(const char* filepath) {
    FILE* file = fopen(filepath, "r");
    if (file == NULL) {
        printf("Ein Fehler ist beim Lesen der Datei aufgetreten.\n");
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
