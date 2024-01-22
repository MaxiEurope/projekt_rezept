#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <limits.h>
#include <ncurses.h>

#include "../util/str/strfunctions.h"
#include "../util/recipe.h"
#include "../util/recipeutil.h"

/**
 * @brief Adds a new recipe to the recipe list
 *
 * @param recipe_count The number of recipes
 * @param recipe_file Path to the recipe file
 * @return Return true if adding recipe was successful, false otherwise
 */
bool add(int *recipe_count, char *recipe_file) {
    Recipe *new_recipe = (Recipe*)malloc(sizeof(Recipe));

    printw("Name: ");
    refresh();
    char name[101];
    if (scanw("%100[^\n]", name) != 1) {
        clear();
        printw("Ungültiger Rezept Name.\n");
        refresh();
        return false;
    }
    new_recipe->name = duplicatestr(name);

    clear();
    printw("Anzahl Zutaten: ");
    refresh();
    int ingredient_count;
    if (scanw("%d", &ingredient_count) != 1) {
        clear();
        printw("Invalide Eingabe, die Anzahl der Zutaten soll eine Zahl sein.\n");
        refresh();
        return false;
    }
    new_recipe->ingredient_count = ingredient_count;

    new_recipe->ingredients = (Ingredient*)malloc(ingredient_count * sizeof(Ingredient));
    for (int i = 0; i < ingredient_count; i++) {
        clear();
        printw("Zutat %d:\n", i + 1);
        printw("Name: ");
        refresh();
        char ingredient_name[101];
        if (scanw("%100[^\n]", ingredient_name) != 1) {
            clear();
            printw("Ungültiger Name für eine Zutat.\n");
            refresh();
            return false;
        }

        new_recipe->ingredients[i].name = lowercase(duplicatestr(ingredient_name));

        printw("Menge: ");
        refresh();
        char raw_quantity[101];
        if (scanw("%100s", raw_quantity) != 1) {
            clear();
            printw("Ungültige Menge für diese Zutat.\n");
            refresh();
            return false;
        }

        char *endptr;
        errno = 0;
        unsigned long ingredient_quantity = strtoul(raw_quantity, &endptr, 10);

        if (endptr == raw_quantity) {
            clear();
            printw("Keine Zahlen gefunden, die Menge soll nur eine Zahl beinhalten.\n");
            refresh();
            return false;
        }

        if ((*endptr != '\0') || (errno == ERANGE)) {
            clear();
            printw("Ungültige Zahl/Menge für diese Zutat.\n");
            refresh();
            return false;
        }

        if (ingredient_quantity > UINT_MAX) {
            clear();
            printw("Die Zahl/Menge ist zu groß für diese Zutat.\n");
            refresh();
            return false;
        }

        new_recipe->ingredients[i].quantity = (unsigned int)ingredient_quantity;

        Unit unit_choice;
        int choice;
        print_units();
        scanw("%d", &choice);

        switch (choice) {
            case 1: {
                unit_choice = GRAM;
                break;
            }
            case 2: {
                unit_choice = MILLILITER;
                break;
            }
            case 3: {
                unit_choice = PIECE;
                break;
            }
            case 4: {
                unit_choice = TABLESPOON;
                break;
            }
            case 5: {
                unit_choice = TEASPOON;
                break;
            }
            case 6: {
                unit_choice = CUP;
                break;
            }
            default: {
                clear();
                printw("Ungültige Auswahl/Einheit für diese Zutat.\n");
                refresh();
                return false;
            }
        }

        new_recipe->ingredients[i].unit = unit_choice;
    }

    clear();
    printw("Anleitung: ");
    refresh();
    char instructions[1001];
    if (scanw("%1000[^\n]", instructions) != 1) {
        clear();
        printw("Ungültige Eingabe.\n");
        refresh();
        return false;
    }
    new_recipe->instructions = duplicatestr(instructions);

    addrecipe(new_recipe, recipe_count, recipe_file);
    free(new_recipe);

    return true;
}

// Unit get_unit() {
//     int choice;
//     print_units();
//     scanw("%d", &choice);

//     switch (choice) {
//         case 1: return GRAM;
//         case 2: return MILLILITER;
//         case 3: return PIECE;
//         case 4: return TABLESPOON;
//         case 5: return TEASPOON;
//         case 6: return CUP;
//         default:
//             printw("Ungültige Auswahl/Einheit für diese Zutat.\n");
//             refresh();
//             return get_unit();
//     }
// }
