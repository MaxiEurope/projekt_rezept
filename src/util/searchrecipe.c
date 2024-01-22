#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "ext/cJSON.h"
#include "recipe.h"
#include <ncurses.h>

/**
 * @brief Checks if a recipe has a specific ingredient
 *
 * @param recipe The recipe to check
 * @param ingredient The ingredient to search for
 * @return True if the recipe has the ingredient, false otherwise.
 */
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

/**
 * @brief Searches for recipes that contain all the specified ingredients
 *
 * @param recipes The array of the recipes
 * @param recipe_count The number of recipes
 * @param ingredients The array of ingredients
 * @param ingredient_count The number of ingredients
 */
void searchrecipe(Recipe* recipes, int recipe_count, char** ingredients, int ingredient_count) {
    clear();
    printw("Suche nach Rezepten:\n");
    refresh();

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
            printw("> [%d] %s\n", i + 1, recipes[i].name);
            refresh();
            found = 1;
        }
    }

    if (!found) {
        printw("Keine Rezepte gefunden\n");
        refresh();
    }
}
