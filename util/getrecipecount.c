#include <stdio.h>
#include <stdlib.h>
#include "ext/cJSON.h"
#include "recipe.h"
#include "recipeutil.h"

/**
 * @brief Gets the count of the recipes in recipes.json
 * 
 * @param recipe_count Pointer to the int variable
 * 
 * @return void
 */
void getrecipecount(int *recipe_count) {
    char *json_data = readfile("recipes.json");
    if (json_data == NULL) {
        printf("Konnte Datei nicht öffnen.\n");
        return;
    }

    cJSON* json = cJSON_Parse(json_data);
    if (json == NULL) {
        printf("Ein Fehler ist beim Laden von der JSON Rezept Datei aufgetreten.\n");
        return;
    }

    *recipe_count = cJSON_GetArraySize(json);

    cJSON_Delete(json);
    free(json_data);
}
