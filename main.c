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
    if (recipe == NULL || !cJSON_IsObject(recipe)) {
        return printf("Failed to get recipe.\n");
    }
    
    cJSON* name = cJSON_GetObjectItem(recipe, "name");
    if (name != NULL && cJSON_IsString(name)) {
        printf("Name: %s\n", name->valuestring);
    }
    cJSON* ingredients = cJSON_GetObjectItem(recipe, "ingredients");
    if (ingredients != NULL && cJSON_IsArray(ingredients)) {
        int numIngredients = cJSON_GetArraySize(ingredients);
        printf("Ingredients:\n");
        for (int i = 0; i < numIngredients; i++) {
            cJSON* ingredient = cJSON_GetArrayItem(ingredients, i);
            cJSON* ingredientName = cJSON_GetObjectItem(ingredient, "name");
            cJSON* ingredientQuantity = cJSON_GetObjectItem(ingredient, "quantity");
            if (ingredientName != NULL && cJSON_IsString(ingredientName)) {
                printf("- %s", ingredientName->valuestring);
                if (ingredientQuantity != NULL && cJSON_IsString(ingredientQuantity)) {
                    printf(", Quantity: %s\n", ingredientQuantity->valuestring);
                } else {
                    printf("\n");
                }
            }
        }
    }

    // Clean up
    cJSON_Delete(json);
    free(json_data);

    return 0;
}
