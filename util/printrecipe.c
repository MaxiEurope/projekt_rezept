#include <stdio.h>
#include "recipe.h"
#include "str/strfunctions.h"

/**
 * @brief Prints all recipes
 *
 * @param recipes The array of recipes
 * @param count The number of recipes
 */
void printrecipes(Recipe* recipes, int count) {
    printf("Lädt %d Rezepte\n", count);
    for (int i = 0; i < count; i++) {
        printf("[%d] Rezept:\n", i + 1);
        printf("> Name: %s\n", recipes[i].name);
        printf("> Anleitung: %s\n", recipes[i].instructions);
        printf("> Zutaten:\n");
        for (int j = 0; j < recipes[i].ingredient_count; j++) {
            printf("> - %s: %s\n", uppercasefirst(recipes[i].ingredients[j].name), recipes[i].ingredients[j].quantity);
        }
        printf("\n");
    }
}
