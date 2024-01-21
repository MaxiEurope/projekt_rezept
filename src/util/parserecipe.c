#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ext/cJSON.h"
#include "str/strfunctions.h"
#include "recipe.h"
#include <ncurses.h>

/**
 * @brief Parses JSON and returns an array of recipes
 *
 * @param json_data The JSON data to be parsed
 * @param recipe_count The number of recipes, set to 0 at first
 * @return Pointer to the array of recipes
 */
Recipe* parserecipe(const char* json_data, int recipe_count) {
    cJSON* json = cJSON_Parse(json_data);
    if (json == NULL) {
        clear();
        printw("Ein Fehler ist beim Laden von der JSON Rezept Datei aufgetreten.\n");
        refresh();
        napms(2000);
        refresh();
        exit(1);
        return NULL;
    }

    Recipe* recipes = (Recipe*)malloc(recipe_count * sizeof(Recipe));

    for (int i = 0; i < recipe_count; i++) {
        cJSON* recipe = cJSON_GetArrayItem(json, i);
        if (recipe == NULL || !cJSON_IsObject(recipe)) {
            clear();
            printw("Ein Fehler ist beim Laden von Rezept %d aufgetreten.\n", i + 1);
            refresh();
            recipes[i].valid = 0;
            continue;
        }

        cJSON* name = cJSON_GetObjectItem(recipe, "name");
        cJSON* ingredients = cJSON_GetObjectItem(recipe, "ingredients");
        cJSON* instructions = cJSON_GetObjectItem(recipe, "instructions");

        if (name == NULL || !cJSON_IsString(name) || ingredients == NULL || !cJSON_IsArray(ingredients) || instructions == NULL || !cJSON_IsString(instructions)) {
            recipes[i].valid = 0;
            continue;
        }

        recipes[i].name = duplicatestr(name->valuestring);
        recipes[i].instructions = duplicatestr(instructions->valuestring);

        int ingredient_count = cJSON_GetArraySize(ingredients);
        recipes[i].ingredient_count = ingredient_count;
        recipes[i].ingredients = (Ingredient*)malloc(ingredient_count * sizeof(Ingredient));

        for (int j = 0; j < ingredient_count; j++) {
            cJSON* ingredient = cJSON_GetArrayItem(ingredients, j);
            if (ingredient == NULL || !cJSON_IsObject(ingredient)) {
                continue;
            }

            cJSON *ingredient_name = cJSON_GetObjectItem(ingredient, "name");
            cJSON *ingredient_quantity = cJSON_GetObjectItem(ingredient, "quantity");
            cJSON *ingredient_unit = cJSON_GetObjectItem(ingredient, "unit");

            if (ingredient_name != NULL && cJSON_IsString(ingredient_name)) {
                recipes[i].ingredients[j].name = lowercase(duplicatestr(ingredient_name->valuestring));
            } else {
                recipes[i].ingredients[j].name = "Unknown Ingredient";
            }

            if (ingredient_quantity != NULL && cJSON_IsNumber(ingredient_quantity)) {
                recipes[i].ingredients[j].quantity = (unsigned int)cJSON_GetNumberValue(ingredient_quantity);
            } else {
                recipes[i].ingredients[j].quantity = 0;
            }

            if (ingredient_unit != NULL && cJSON_IsString(ingredient_unit)) {
                recipes[i].ingredients[j].unit = string_to_unit(ingredient_unit->valuestring);
            } else {
                recipes[i].ingredients[j].unit = NULL;
            }

        }
        recipes[i].valid = 1;
    }

    cJSON_Delete(json);
    
    return recipes;
}
