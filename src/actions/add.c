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

static Unit get_unit(char *ingredient_name) {
    int choice;
    print_units();
    scanw("%d", &choice);

    switch (choice) {
        case 1: return GRAM;
        case 2: return MILLILITER;
        case 3: return PIECE;
        case 4: return TABLESPOON;
        case 5: return TEASPOON;
        case 6: return CUP;
        default:
            clear();
            printw("Ungültige Auswahl/Einheit für diese Zutat: %s\n", ingredient_name);
            refresh();
            return get_unit(ingredient_name);
    }
}

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

        Unit unit_choice = get_unit(new_recipe->ingredients[i].name);

        new_recipe->ingredients[i].unit = unit_choice;
    }

    clear();
    printw("Anleitung: ");
    refresh();
    char instructions[10001];
    if (scanw("%10000[^\n]", instructions) != 1) {
        clear();
        printw("Ungültige Eingabe.\n");
        refresh();
        return false;
    }
    new_recipe->instructions = duplicatestr(instructions);

    addrecipe(new_recipe, recipe_count, recipe_file);

    freerecipes(new_recipe, 1);

    printw("\nDrücke eine beliebige Taste um fortzufahren.\n");
    getch();
    clear();

    return true;
}
