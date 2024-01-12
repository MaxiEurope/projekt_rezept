#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util/ext/cJSON.h"
#include "util/recipe.h"
#include "util/readfile.c"

int recipe_count = 0;

/**
 * TODO
 * 
 * put functions into separate files
*/

Recipe* parserecipe(const char* json_data) {
    cJSON* json = cJSON_Parse(json_data);
    if (json == NULL) {
        printf("Ein Fehler ist beim Laden von der JSON Rezept Datei aufgetreten.\n");
        return NULL;
    }

    recipe_count = cJSON_GetArraySize(json);
    printf("Lädt %d Rezepte\n", recipe_count);
    Recipe* recipes = (Recipe*)malloc(recipe_count * sizeof(Recipe));

    for (int i = 0; i < recipe_count; i++) {
        cJSON* recipe = cJSON_GetArrayItem(json, i);
        if (recipe == NULL || !cJSON_IsObject(recipe)) {
            printf("Ein Fehler ist beim Laden von Rezept %d aufgetreten.\n", i + 1);
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

        recipes[i].name = strdup(name->valuestring);
        recipes[i].instructions = strdup(instructions->valuestring);

        int ingredient_count = cJSON_GetArraySize(ingredients);
        recipes[i].ingredient_count = ingredient_count;
        recipes[i].ingredients = (Ingredient*)malloc(ingredient_count * sizeof(Ingredient));

        for (int j = 0; j < ingredient_count; j++) {
            cJSON* ingredient = cJSON_GetArrayItem(ingredients, j);
            if (ingredient != NULL && cJSON_IsObject(ingredient)) {
                cJSON* ingredient_name = cJSON_GetObjectItem(ingredient, "name");
                cJSON* ingredient_quantity = cJSON_GetObjectItem(ingredient, "quantity");

                if (ingredient_name != NULL && cJSON_IsString(ingredient_name)) {
                    recipes[i].ingredients[j].name = strdup(ingredient_name->valuestring);
                } else {
                    recipes[i].ingredients[j].name = NULL;
                }

                if (ingredient_quantity != NULL && cJSON_IsString(ingredient_quantity)) {
                    recipes[i].ingredients[j].quantity = strdup(ingredient_quantity->valuestring);
                } else {
                    recipes[i].ingredients[j].quantity = NULL;
                }
            }
        }
        recipes[i].valid = 1;
    }

    cJSON_Delete(json);
    
    return recipes;
}

void freerecipes(Recipe* recipes, int count) {
    if (recipes == NULL) {
        return;
    }

    for (int i = 0; i < count; i++) {
        free(recipes[i].name);
        free(recipes[i].instructions);

        for (int j = 0; j < recipes[i].ingredient_count; j++) {
            free(recipes[i].ingredients[j].name);
            free(recipes[i].ingredients[j].quantity);
        }
        free(recipes[i].ingredients);
    }

    free(recipes);
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

    Recipe* recipes = parserecipe(json_data);
    if (recipes == NULL) {
        printf("recipes is null\n");
        free(json_data);
        return 1;
    } else {
        printf("recipes is not null\n");
    }

    printf("Lädt %d Rezepte 2\n", recipe_count);
    for (int i = 0; i < recipe_count; i++) {
        Recipe* recipe = &recipes[i];

        printf("Recipe %d: %s\n", i + 1, recipe->name);
        printf("Instructions:\n%s\n", recipe->instructions);
        printf("Ingredients:\n");
        for (int j = 0; j < recipe->ingredient_count; j++) {
            Ingredient* ingredient = &recipe->ingredients[j];
            printf("  %s %s\n", ingredient->quantity, ingredient->name);
        }
        printf("\n");
    }

    freerecipes(recipes, recipe_count);
    free(json_data);

    return 0;
}
