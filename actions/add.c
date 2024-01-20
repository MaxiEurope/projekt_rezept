#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
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
    clear_input_buffer();
    new_recipe->name = duplicatestr(name);

    printw("Anzahl Zutaten: ");
    refresh();
    int ingredient_count;
    if (scanw("%d", &ingredient_count) != 1) {
        clear();
        printw("Invalide Eingabe, die Anzahl der Zutaten soll eine Zahl sein.\n");
        refresh();
        return false;
    }
    clear_input_buffer();
    new_recipe->ingredient_count = ingredient_count;

    new_recipe->ingredients = (Ingredient*)malloc(ingredient_count * sizeof(Ingredient));
    for (int i = 0; i < ingredient_count; i++) {
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
        clear_input_buffer();

        new_recipe->ingredients[i].name = lowercase(duplicatestr(ingredient_name));

        printw("Menge: ");
        refresh();
        char ingredient_quantity[101];
        if (scanw("%100[^\n]", ingredient_quantity) != 1) {
            clear();
            printw("Ungültiger Menge an Zutaten.\n");
            refresh();
            return false;
        }
        clear_input_buffer();

        new_recipe->ingredients[i].quantity = duplicatestr(ingredient_quantity);
    }

    printw("Anleitung:\n");
    refresh();
    char instructions[1001];
    if (scanw("%1000[^\n]", instructions) != 1) {
        clear();
        printw("Ungültige Eingabe.\n");
        refresh();
        return false;
    }
    instructions[strcspn(instructions, "\n")] = '\0'; // https://stackoverflow.com/a/28462221
    new_recipe->instructions = duplicatestr(instructions);

    addrecipe(new_recipe, recipe_count, recipe_file);
    free(new_recipe);

    return true;
}
