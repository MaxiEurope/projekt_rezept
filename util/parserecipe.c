#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ext/cJSON.h"
#include "recipe.h"

Recipe* parserecipe(const char* json_data, int *recipe_count) {
    cJSON* json = cJSON_Parse(json_data);
    if (json == NULL) {
        printf("Ein Fehler ist beim Laden von der JSON Rezept Datei aufgetreten.\n");
        return NULL;
    }

    *recipe_count = cJSON_GetArraySize(json);
    printf("Lädt %d Rezepte\n", *recipe_count);
    Recipe* recipes = (Recipe*)malloc(*recipe_count * sizeof(Recipe));

    for (int i = 0; i < *recipe_count; i++) {
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

        recipes[i].name = duplicatestr(name->valuestring);
        recipes[i].instructions = duplicatestr(instructions->valuestring);

        int ingredient_count = cJSON_GetArraySize(ingredients);
        recipes[i].ingredient_count = ingredient_count;
        recipes[i].ingredients = (Ingredient*)malloc(ingredient_count * sizeof(Ingredient));

        for (int j = 0; j < ingredient_count; j++) {
            cJSON* ingredient = cJSON_GetArrayItem(ingredients, j);
            if (ingredient != NULL && cJSON_IsObject(ingredient)) {
                cJSON* ingredient_name = cJSON_GetObjectItem(ingredient, "name");
                cJSON* ingredient_quantity = cJSON_GetObjectItem(ingredient, "quantity");

                if (ingredient_name != NULL && cJSON_IsString(ingredient_name)) {
                    recipes[i].ingredients[j].name = duplicatestr(ingredient_name->valuestring);
                } else {
                    recipes[i].ingredients[j].name = NULL;
                }

                if (ingredient_quantity != NULL && cJSON_IsString(ingredient_quantity)) {
                    recipes[i].ingredients[j].quantity = duplicatestr(ingredient_quantity->valuestring);
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
