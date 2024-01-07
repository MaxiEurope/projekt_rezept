#include <stdio.h>
#include <stdlib.h>
#include "util/ext/cJSON.h"

char* readJsonFile(const char* filepath) {
    FILE* file = fopen(filepath, "r");
    if (file == NULL) {
        printf("Failed to open the file.\n");
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

int main() {
    char* json_data = readJsonFile("recipes.json");
    if (json_data == NULL) {
        return 1;
    }

    cJSON* json = cJSON_Parse(json_data);
    if (json == NULL) {
        printf("Failed to parse JSON.\n");
        free(json_data);
        return 1;
    }

    cJSON* recipe = cJSON_GetObjectItem(json, "recipe");
    if (recipe != NULL && cJSON_IsObject(recipe)) {
        cJSON* name = cJSON_GetObjectItem(recipe, "name");
        if (name != NULL && cJSON_IsString(name)) {
            printf("Name: %s\n", name->valuestring);
        }
    }

    // Clean up
    cJSON_Delete(json);
    free(json_data);

    return 0;
}
