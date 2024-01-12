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

/**
 * TODO
 * 
 * put functions into separate files
*/



void addrecipe(Recipe *new_recipe) {
    char *json_data = readfile("recipes.json");
    if (json_data == NULL) {
        printf("Konnte Datei nicht öffnen.\n");
        return;
    }

    // Parse the existing recipes
    cJSON *json_recipes = cJSON_Parse(json_data);
    if (json_recipes == NULL) {
        printf("Could not parse JSON data.\n");
        free(json_data);
        return;
    }

    // Create a cJSON object for the new recipe
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

    // Add the new recipe to the existing recipes
    cJSON_AddItemToArray(json_recipes, json_new_recipe);

    // Convert the updated recipes to a string
    char *updated_json_data = cJSON_Print(json_recipes);

    // Write the updated recipes to the file
    FILE *file = fopen("recipes.json", "w");
    if (file == NULL) {
        printf("Could not open file for writing.\n");
        free(updated_json_data);
        cJSON_Delete(json_recipes);
        free(json_data);
        return;
    }

    fprintf(file, "%s\n", updated_json_data);
    fclose(file);

    // Clean up
    free(updated_json_data);
    cJSON_Delete(json_recipes);
    free(json_data);
}

int main() {
    printf("Hello, World!\n");
    char* json_data = readfile("recipes.json");
    // printf("%s\n", json_data);
    if (json_data == NULL) {
        printf("json_data is null\n");
        return 1;
    } else {
        printf("json_data is not null\n");
    }

    Recipe* recipes = parserecipe(json_data, &recipe_count);
    if (recipes == NULL) {
        printf("recipes is null\n");
        free(json_data);
        return 1;
    } else {
        printf("recipes is not null\n");
    }

    printrecipes(recipes, recipe_count);

    // for (int i = 0; i < recipe_count; i++) {
    //     Recipe* recipe = &recipes[i];

    //     printf("Recipe %d: %s\n", i + 1, recipe->name);
    //     printf("Instructions:\n%s\n", recipe->instructions);
    //     printf("Ingredients:\n");
    //     for (int j = 0; j < recipe->ingredient_count; j++) {
    //         Ingredient* ingredient = &recipe->ingredients[j];
    //         printf("  %s %s\n", ingredient->quantity, ingredient->name);
    //     }
    //     printf("\n");
    // }

    freerecipes(recipes, recipe_count);
    free(json_data);

    return 0;
}
