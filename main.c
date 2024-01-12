#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util/ext/cJSON.h"
#include "util/recipe.h"
#include "util/readfile.c"
#include "util/parserecipe.c"
#include "util/printrecipe.c"
#include "util/freerecipes.c"

int recipe_count = 0;

void addrecipe(Recipe *new_recipe, int *recipe_count) {
    char *json_data = readfile("recipes.json");
    if (json_data == NULL) {
        printf("Konnte Datei nicht öffnen.\n");
        return NULL;
    }

    cJSON *json_recipes = cJSON_Parse(json_data);
    if (json_recipes == NULL) {
        printf("Ein Fehler ist beim Laden von der JSON Rezept Datei aufgetreten.\n");
        free(json_data);
        return NULL;
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
        return NULL;
    }
    FILE *file = fopen("recipes.json", "w");
    if (file == NULL) {
        printf("Konnte Datei nicht öffnen.\n");
        free(updated_json_data);
        cJSON_Delete(json_recipes);
        free(json_data);
        return NULL;
    }

    *recipe_count++;
    printf("Rezept hinzugefügt.\n");
    printf("Rezeptanzahl: %d\n", *recipe_count);

    fprintf(file, "%s\n", updated_json_data);
    fclose(file);

    free(updated_json_data);
    cJSON_Delete(json_recipes);
    free(json_data);
}

int main() {
    printf("Hello, World!\n");
    char *json_data = readfile("recipes.json");
    // printf("%s\n", json_data);
    if (json_data == NULL) {
        printf("json_data is null\n");
        return 1;
    } else {
        printf("json_data is not null\n");
    }

    Recipe *recipes = parserecipe(json_data, &recipe_count);
    if (recipes == NULL) {
        printf("recipes is null\n");
        free(json_data);
        return 1;
    } else {
        printf("recipes is not null\n");
    }

    printrecipes(recipes, recipe_count);

    Recipe new_recipe;
    new_recipe.name = "Testrezept";
    new_recipe.instructions = "Testanleitung";
    new_recipe.ingredient_count = 2;
    new_recipe.ingredients = (Ingredient*)malloc(2 * sizeof(Ingredient));
    new_recipe.ingredients[0].name = "Zutat 1";
    new_recipe.ingredients[0].quantity = "1";
    new_recipe.ingredients[1].name = "Zutat 2";
    new_recipe.ingredients[1].quantity = "2";
    addrecipe(&new_recipe, &recipe_count);

    freerecipes(recipes, recipe_count);
    free(json_data);

    return 0;
}
