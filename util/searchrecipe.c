#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "ext/cJSON.h"
#include "recipe.h"

bool hasingredient(Recipe* recipe, char* ingredient) {
    if (recipe == NULL || ingredient == NULL) {
        return false;
    }
    for (int i = 0; i < recipe->ingredient_count; i++) {
        if (strcmp(recipe->ingredients[i].name, ingredient) == 0) {
            return true;
        }
    }
    return false;
}

void searchrecipe(Recipe* recipes, int recipe_count, char** ingredients, int ingredient_count) {

    // printf("recipe_count: %d\n", recipe_count);
    // for (int i = 0; i < ingredient_count; i++) {
    //     printf("ingredient: %s\n", ingredients[i]);
    // }

    printf("Suche nach Rezepten:\n");

    int found = 0;

    for (int i = 0; i < recipe_count; i++) {
        bool has_all = true;
        for (int j = 0; j < ingredient_count; j++) {
            if (!hasingredient(&recipes[i], ingredients[j])) {
                has_all = false;
                break;
            }
        }
        if (has_all) {
            printf("> %s\n", recipes[i].name);
            found = 1;
        }
    }

    if (!found) {
        printf("Keine Rezepte gefunden\n");
    }
}
