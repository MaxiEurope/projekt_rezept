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
            int quantity;
            if (recipes[i].ingredients[j].quantity == NULL) {
                quantity = 0;
            } else {
                quantity = recipes[i].ingredients[j].quantity;
            }
            char *unit = printunit(recipes[i].ingredients[j].unit);

            printw("> - %s: %d%s\n", uppercasefirst(recipes[i].ingredients[j].name), quantity, unit);
        }
        printw("\n");
        refresh();
    }
}

/**
 * @brief Returns a string representation of the given unit.
 *
 * @param unit The unit to be converted to a string.
 * @return A string representation of the unit.
 */
const char* printunit(Unit unit) {
    switch (unit) {
        case GRAM:       return "gram";
        case MILLILITER: return "milliliter";
        case PIECE:      return "piece";
        case TABLESPOON: return "tablespoon";
        case TEASPOON:   return "teaspoon";
        case CUP:        return "cup";
        default:         return "unknown";
    }
}

/**
 * @brief Prints the available units to choose from
 */
void print_units() {
    printw("Wähle eine Einheit:\n");
    printw("1. Gramm\n");
    printw("2. Milliliter\n");
    printw("3. Stück\n");
    printw("4. Esslöffel\n");
    printw("5. Teelöffel\n");
    printw("6. Tasse\n");
}
