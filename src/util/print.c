#include <stdio.h>
#include <string.h>
#include "recipe.h"
#include "str/strfunctions.h"
#include <ncurses.h>

#include "recipeutil.h"

/**
 * @brief Prints all recipes
 *
 * @param recipes The array of recipes
 * @param count The number of recipes
 */
void printrecipes(Recipe* recipes, int count, int recipe_index) {
    clear();
    refresh();
    
    printw("[%d] Rezept:\n", recipe_index + 1);
    printw("  Kategorie: %s\n", recipes[recipe_index].category);
    printw("  Name: %s\n", recipes[recipe_index].name);
    printw("  Anleitung: %s\n", recipes[recipe_index].instructions);
    printw("  Zutaten:\n");
    for (int j = 0; j < recipes[recipe_index].ingredient_count; j++) {
        int quantity = recipes[recipe_index].ingredients[j].quantity;
        char *unit = print_unit(recipes[recipe_index].ingredients[j].unit);

        printw("  - %s: %d %s\n", uppercasefirst(recipes[recipe_index].ingredients[j].name), quantity, unit);
    }
    refresh();
}

/**
 * @brief Returns a string representation of the given unit.
 *
 * @param unit The unit to be converted to a string.
 * @return A string representation of the unit.
 */
char* print_unit(Unit unit) {
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

Unit string_to_unit(const char* str) {
    if (strcmp(str, "gram") == 0) return GRAM;
    else if (strcmp(str, "milliliter") == 0) return MILLILITER;
    else if (strcmp(str, "piece") == 0) return PIECE;
    else if (strcmp(str, "tablespoon") == 0) return TABLESPOON;
    else if (strcmp(str, "teaspoon") == 0) return TEASPOON;
    else if (strcmp(str, "cup") == 0) return CUP;
    else return GRAM; // assume gram as default
}

