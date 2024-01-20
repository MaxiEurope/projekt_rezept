#include <stdlib.h>
#include "recipe.h"

/**
 * @brief Frees the memory allocated for the recipes
 * 
 * @param recipes The array of recipes
 * @param count The number of recipes
 */
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
