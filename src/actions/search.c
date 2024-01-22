#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ncurses.h>

#include "../util/str/strfunctions.h"
#include "../util/recipe.h"
#include "../util/recipeutil.h"

/**
 * @brief Parses a string of ingredients separated by commas and saves them in an array
 *
 * @param str The string of ingredients
 * @param array A pointer to an array
 * @return The number of ingredients parsed, -1 if an error occurred
 */
// int parseingredients(char *str, char ***array) {
//     int count = 0;
//     char *token;
//     char *rest = str;

//     while ((token = strtok_r(rest, ",", &rest))) {
//         count++;
//     }

//     *array = (char **)malloc(count * sizeof(char *));
//     if (!*array) {
//         return -1;
//     }

//     rest = str;
//     int i = 0;
//     while ((token = strtok_r(rest, ",", &rest))) {
//         (*array)[i] = strdup(token);
//         i++;
//     }

//     return count;
// }

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

    // TODO: sort by ingredient quantity
 
    // char ingredients_str[256];  // Assuming the entire line will be less than 256 characters
    // printw("Geben Sie die Zutaten ein (durch Kommas getrennt): ");
    // refresh();

    // if (scanw("%255[^\n]", ingredients_str) != 1) {
    //     printw("Ungültige Eingabe.\n");
    //     refresh();
    //     // Handle error
    // }

    // char **ingredients;
    // int ingredient_count = splitIngredients(ingredients_str, &ingredients);

    // if (ingredient_count < 0) {
    //     printw("Ein Fehler ist beim Zuweisen von Speicher aufgetreten.\n");
    //     refresh();
    //     // Handle error
    // }

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
