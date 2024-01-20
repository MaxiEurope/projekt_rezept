#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ncurses.h>

#include "../util/str/strfunctions.h"
#include "../util/recipe.h"
#include "../util/recipeutil.h"

/**
 * @brief Searches for recipes with specific ingredients
 *
 * @param recipe_count The number of recipes
 * @param recipe_file Path to the recipe file
 * @return True if the search was successful, false otherwise.
 */
bool search(int *recipe_count, char *recipe_file) {
    char *json_data = readfile(recipe_file);

    Recipe *recipes = parserecipe(json_data, *recipe_count);

    printw("Wie viele Zutaten möchtest du eingeben? ");
    refresh();
    int ingredient_count;
    if (scanw("%d", &ingredient_count) != 1) {
        clear();
        printw("Invalid Eingabe, die Anzahl der Zutaten soll eine Zahl sein.\n");
        refresh();
        return false;
    }

    char **ingredients = (char**)malloc(ingredient_count * sizeof(char*));
    if (!ingredients) {
        clear();
        printw("Ein Fehler ist beim Zuweisen von Speicher aufgetreten.\n");
        refresh();
        freerecipes(recipes, *recipe_count);
        free(json_data);
        return 1;
    }

    for (int i = 0; i < ingredient_count; i++) {
        printw("Zutat %d: ", i + 1);
        refresh();
        char ingredient_name[101];
        if (scanw("%100[^\n]", ingredient_name) != 1) {
            clear();
            printw("Ungültiger Name für eine Zutat.\n");
            refresh();
            return 1;
        }
        ingredients[i] = lowercase(duplicatestr(ingredient_name));
    }
    searchrecipe(recipes, *recipe_count, ingredients, ingredient_count);

    for (int i = 0; i < ingredient_count; i++) {
        free(ingredients[i]);
    }
    free(ingredients);
    freerecipes(recipes, *recipe_count);
    free(json_data);

    return true;
}
