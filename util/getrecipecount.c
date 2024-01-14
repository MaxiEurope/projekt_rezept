#include <stdio.h>
#include <stdlib.h>
#include "ext/cJSON.h"
#include "recipe.h"

/**
 * @brief Gets the count of the recipes in recipes.json
 * 
 * @param recipe_count Pointer to the int variable
 * 
 * @return void
 */
void getrecipecount(int *recipe_count) {
    // Read the contents of the "recipes.json" file
    char *json_data = readfile("recipes.json");
    if (json_data == NULL) {
        printf("Konnte Datei nicht öffnen.\n");
        return;
    }

    // Parse the JSON data
    cJSON* json = cJSON_Parse(json_data);
    if (json == NULL) {
        printf("Ein Fehler ist beim Laden von der JSON Rezept Datei aufgetreten.\n");
        return;
    }

    // Get the count of recipes from the JSON array
    *recipe_count = cJSON_GetArraySize(json);

    // Clean up memory
    cJSON_Delete(json);
    free(json_data);
}
