#include <stdio.h>
#include "recipe.h"

void printrecipes(Recipe* recipes, int count) {
    printf("Lädt %d Rezepte 2\n", count);
    for (int i = 0; i < count; i++) {
        printf("Recipe %d:\n", i + 1);
        printf("Name: %s\n", recipes[i].name);
        printf("Instructions: %s\n", recipes[i].instructions);
        printf("Ingredients:\n");
        for (int j = 0; j < recipes[i].ingredient_count; j++) {
            printf("- %s: %s\n", recipes[i].ingredients[j].name, recipes[i].ingredients[j].quantity);
        }
        printf("\n");
    }
}
