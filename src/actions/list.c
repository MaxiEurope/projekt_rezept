#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ncurses.h>

#include "../util/recipeutil.h"

/**
 * @brief Lists a recipe
 * 
 * @param recipe_count The number of recipes
 * @param recipe_file Path to the recipe file
 * @return True if listing was successful, false otherwise.
 */
bool list(int *recipe_count, char *recipe_file) {
    char *json_data = readfile(recipe_file);

    Recipe *recipes = parserecipe(json_data, *recipe_count);

    printw("Welches Rezept möchtest du ansehen?\n");

    for (int i = 0; i < *recipe_count; i++) {
        printw("[%d] %s\n", i + 1, recipes[i].name);
    }
    printw("Auswahl: ");
    refresh();

    int recipe_index;
    if (scanw("%d", &recipe_index) != 1) {
        clear();
        printw("Invalide Eingabe, die Nummer des Rezepts soll eine Zahl sein.\n");
        refresh();
        return false;
    }
    if (recipe_index < 1 || recipe_index > *recipe_count) {
        clear();
        printw("Ungültige Eingabe, dieses Rezept existiert nicht.\n");
        refresh();
        freerecipes(recipes, *recipe_count);
        free(json_data);
        return false;
    }

    printrecipes(recipes, *recipe_count, recipe_index - 1);
    freerecipes(recipes, *recipe_count);

    free(json_data);

    printw("\nDrücke eine beliebige Taste um fortzufahren.\n");
    getch();
    clear();

    return true;
}
