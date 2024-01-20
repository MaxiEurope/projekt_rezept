#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ncurses.h>

#include "../util/ext/cJSON.h"
#include "../util/str/strfunctions.h"
#include "../util/recipe.h"
#include "../util/recipeutil.h"

/**
 * @brief Deletes a recipe from the recipe list
 *
 * @param recipe_count The number of recipes
 * @param recipe_file Path to the recipe file
 * @return Return true if deleting a recipe was successful, false otherwise
 */
bool del(int *recipe_count, char *recipe_file) {
    char *json_data = readfile(recipe_file);

    Recipe *recipes = parserecipe(json_data, *recipe_count);

    printw("Welches Rezept möchtest du löschen?\n");

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
        printw("Invalide Eingabe, die Nummer des Rezepts soll zwischen 1 und %d sein.\n", *recipe_count);
        refresh();
        freerecipes(recipes, *recipe_count);
        free(json_data);
        return false;
    }

    cJSON *json_recipes = cJSON_Parse(json_data);
    if (json_recipes == NULL) {
        clear();
        printw("Ein Fehler ist beim Laden von der JSON Rezept Datei aufgetreten.\n");
        refresh();
        freerecipes(recipes, *recipe_count);
        free(json_data);
        napms(2000);
        exit(1);
        return false;
    }

    cJSON_DeleteItemFromArray(json_recipes, recipe_index - 1);

    char *updated_json_data = cJSON_Print(json_recipes);
    if (updated_json_data == NULL) {
        clear();
        printw("Ein Fehler ist beim Speichern aufgetreten.\n");
        refresh();
        cJSON_Delete(json_recipes);
        freerecipes(recipes, *recipe_count);
        free(json_data);
        napms(2000);
        exit(1);
        return false;
    }

    FILE *file = fopen(recipe_file, "w");
    if (file == NULL) {
        clear();
        printw("Konnte Datei nicht öffnen.\n");
        refresh();
        free(updated_json_data);
        cJSON_Delete(json_recipes);
        freerecipes(recipes, *recipe_count);
        free(json_data);
        napms(2000);
        exit(1);
        return false;
    }

    (*recipe_count)--;
    clear();
    printw("Rezept mit der Nummber %d erfolgreich gelöscht.\n", recipe_index);
    refresh();

    fprintf(file, "%s\n", updated_json_data);
    fclose(file);

    free(updated_json_data);
    cJSON_Delete(json_recipes);
    freerecipes(recipes, *recipe_count);
    free(json_data);

    return true;
}
