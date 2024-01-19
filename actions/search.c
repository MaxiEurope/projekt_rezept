#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../util/str/strfunctions.h"
#include "../util/recipe.h"
#include "../util/recipeutil.h"

bool search(int *recipe_count, char *recipe_file) {
    char *json_data = readfile(recipe_file);

    Recipe *recipes = parserecipe(json_data, *recipe_count);
    if (recipes == NULL) {
        fprintf(stderr, "Ein Fehler ist beim Laden von der JSON Rezept Datei aufgetreten.\n");
        free(json_data);
        return 1;
    }

    printf("Wie viele Zutaten möchtest du eingeben?\n");
    int ingredient_count;
    if (scanf("%d", &ingredient_count) != 1) {
        printf("Invalid Eingabe, die Anzahl der Zutaten soll eine Zahl sein.\n");
        return false;
    }
    clear_input_buffer();

    char **ingredients = (char**)malloc(ingredient_count * sizeof(char*));
    if (!ingredients) {
        printf("Ein Fehler ist beim Zuweisen von Speicher aufgetreten.\n");
        freerecipes(recipes, *recipe_count);
        free(json_data);
        return 1;
    }

    for (int i = 0; i < ingredient_count; i++) {
        printf("Zutat %d: ", i + 1);
        char ingredient_name[101];
        if (scanf("%100[^\n]", ingredient_name) != 1) {
            printf("Ungültiger Name für eine Zutat.\n");
            return 1;
        }
        clear_input_buffer();
        ingredients[i] = lowercase(duplicatestr(ingredient_name));
    }
    searchrecipe(recipes, *recipe_count, ingredients, ingredient_count);

    for (int i = 0; i < ingredient_count; i++) {
        free(ingredients[i]);
    }
    free(ingredients);
    freerecipes(recipes, *recipe_count);
    free(json_data);
}
