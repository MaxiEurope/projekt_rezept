#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

    printf("Name: ");
    char name[101];
    if (scanf("%100[^\n]", name) != 1) {
        fprintf(stderr, "Ungültiger Rezept Name.\n");
        return false;
    }
    clear_input_buffer();
    new_recipe->name = duplicatestr(name);

    printf("Anzahl Zutaten: ");
    int ingredient_count;
    if (scanf("%d", &ingredient_count) != 1) {
        fprintf(stderr, "Invalide Eingabe, die Anzahl der Zutaten soll eine Zahl sein.\n");
        return false;
    }
    clear_input_buffer();
    new_recipe->ingredient_count = ingredient_count;

    new_recipe->ingredients = (Ingredient*)malloc(ingredient_count * sizeof(Ingredient));
    for (int i = 0; i < ingredient_count; i++) {
        printf("Zutat %d:\n", i + 1);
        printf("Name: ");
        char ingredient_name[101];
        if (scanf("%100[^\n]", ingredient_name) != 1) {
            fprintf(stderr, "Ungültiger Name für eine Zutat.\n");
            return false;
        }
        clear_input_buffer();

        new_recipe->ingredients[i].name = lowercase(duplicatestr(ingredient_name));

        printf("Menge: ");
        char ingredient_quantity[100];
        if (scanf("%100[^\n]", ingredient_quantity) != 1) {
            fprintf(stderr, "Ungültiger Menge an Zutaten.\n");
            return false;
        }
        clear_input_buffer();

        new_recipe->ingredients[i].quantity = duplicatestr(ingredient_quantity);
    }

    printf("Anleitung:\n");
    char instructions[1000];
    if (fgets(instructions, sizeof(instructions), stdin) == NULL) {
        fprintf(stderr, "Ungültige Eingabe.\n");
        return false;
    }
    instructions[strcspn(instructions, "\n")] = '\0'; // https://stackoverflow.com/a/28462221
    new_recipe->instructions = duplicatestr(instructions);

    addrecipe(new_recipe, recipe_count, recipe_file);
    free(new_recipe);

    return true;
}
