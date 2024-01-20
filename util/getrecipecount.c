#include <stdio.h>
#include <stdlib.h>
#include "ext/cJSON.h"
#include "recipe.h"
#include "recipeutil.h"
#include <ncurses.h>

/**
 * @brief Gets the count of the recipes in recipe file
 * 
 * @param recipe_count The number of recipes
 * @param recipe_file Path to the recipe file
 */
void getrecipecount(int *recipe_count, char *recipe_file) {
    char *json_data = readfile(recipe_file);

    cJSON* json = cJSON_Parse(json_data);
    if (json == NULL) {
        clear();
        printw("Ein Fehler ist beim Laden von der JSON Rezept Datei aufgetreten.\n");
        refresh();
        napms(2000);
        exit(1);
        return;
    }

    *recipe_count = cJSON_GetArraySize(json);

    cJSON_Delete(json);
    free(json_data);
}
