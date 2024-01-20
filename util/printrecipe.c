#include <stdio.h>
#include "recipe.h"
#include "str/strfunctions.h"
#include <ncurses.h>

/**
 * @brief Prints all recipes
 *
 * @param recipes The array of recipes
 * @param count The number of recipes
 */
void printrecipes(Recipe* recipes, int count, int recipe_index) {
    clear();
    refresh();
    for (int i = 0; i < count; i++) {
        if (i != recipe_index) { continue; }
        printw("[%d] Rezept:\n", i + 1);
        printw("> Name: %s\n", recipes[i].name);
        printw("> Anleitung: %s\n", recipes[i].instructions);
        printw("> Zutaten:\n");
        for (int j = 0; j < recipes[i].ingredient_count; j++) {
            char *quantity;
            if (recipes[i].ingredients[j].quantity == NULL) {
                quantity = "0";
            } else {
                quantity = recipes[i].ingredients[j].quantity;
            }

            printw("> - %s: %s\n", uppercasefirst(recipes[i].ingredients[j].name), quantity);
        }
        printw("\n");
        refresh();
    }
}
