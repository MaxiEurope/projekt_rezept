#include <stdlib.h>
#include "recipe.h"

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
