#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ext/cJSON.h"
#include "str/strfunctions.h"
#include "recipe.h"
#include "recipeutil.h"

/**
 * @brief Adds a new recipe to the list of recipes
 * 
 * @param new_recipe New recipe to add
 * @param recipe_count Updates the recipe count
 * @param recipe_file Path to the recipe file
 */
void addrecipe(Recipe *new_recipe, int *recipe_count, char *recipe_file) {
    char *json_data = readfile(recipe_file);
    if (json_data == NULL) {
        printf("Konnte Datei nicht öffnen.\n");
        return;
    }

    cJSON *json_recipes = cJSON_Parse(json_data);
    if (json_recipes == NULL) {
        printf("Ein Fehler ist beim Laden von der JSON Rezept Datei aufgetreten.\n");
        free(json_data);
        return;
    }

    cJSON *json_new_recipe = cJSON_CreateObject();
    cJSON_AddStringToObject(json_new_recipe, "name", new_recipe->name);
    cJSON_AddStringToObject(json_new_recipe, "instructions", new_recipe->instructions);

    cJSON *json_ingredients = cJSON_CreateArray();
    for (int i = 0; i < new_recipe->ingredient_count; i++) {
        cJSON *json_ingredient = cJSON_CreateObject();
        cJSON_AddStringToObject(json_ingredient, "name", new_recipe->ingredients[i].name);
        cJSON_AddStringToObject(json_ingredient, "quantity", new_recipe->ingredients[i].quantity);
        cJSON_AddItemToArray(json_ingredients, json_ingredient);
    }
    cJSON_AddItemToObject(json_new_recipe, "ingredients", json_ingredients);

    cJSON_AddItemToArray(json_recipes, json_new_recipe);

    char *updated_json_data = cJSON_Print(json_recipes);
    if (updated_json_data == NULL) {
        printf("Ein Fehler ist beim Speichern aufgetreten.\n");
        cJSON_Delete(json_recipes);
        free(json_data);
        return;
    }
    FILE *file = fopen(recipe_file, "w");
    if (file == NULL) {
        printf("Konnte Datei nicht öffnen.\n");
        free(updated_json_data);
        cJSON_Delete(json_recipes);
        free(json_data);
        return;
    }

    (*recipe_count)++;
    printf("Rezept hinzugefügt.\n");
    printf("Rezeptanzahl: %d\n", *recipe_count);

    fprintf(file, "%s\n", updated_json_data);
    fclose(file);

    free(updated_json_data);
    cJSON_Delete(json_recipes);
    free(json_data);
}
