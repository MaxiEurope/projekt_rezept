#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../util/ext/cJSON.h"
#include "../util/str/strfunctions.h"
#include "../util/recipe.h"
#include "../util/recipeutil.h"

/**
 * @brief Edits part of a recipe
 *
 * @param recipe_count The number of recipes
 * @param recipe_file Path to the recipe file
 * @return True if editing was successful, false otherwise.
 */
bool edit(int *recipe_count, char *recipe_file) {
    char *json_data = readfile(recipe_file);

    Recipe *recipes = parserecipe(json_data, recipe_count);

    printf("Welches Rezept möchtest du bearbeiten?\n");

    for (int i = 0; i < recipe_count; i++) {
        printf("[%d] %s\n", i + 1, recipes[i].name);
    }
    printf("Auswahl: ");

    int recipe_index;
    if (scanf("%d", &recipe_index) != 1) {
        fprintf(stderr, "Invalide Eingabe, die Nummer des Rezepts soll eine Zahl sein.\n");
        return false;
    }
    if (recipe_index < 1 || recipe_index > recipe_count) {
        fprintf(stderr, "Ungültige Eingabe.\n");
        freerecipes(recipes, recipe_count);
        free(json_data);
        return false;
    }

    printf("Was möchtest du bearbeiten?\n");
    printf("1. Name\n");
    printf("2. Zutaten\n");
    printf("3. Anleitung\n");
    printf("Auswahl: ");

    int edit_choice;
    char edit_term;
    if (scanf("%1d%c", &edit_choice, &edit_term) != 2 || edit_term != '\n') {
        fprintf(stderr, "Ungültige Eingabe. Bitte nur eine Zahl (1-3) eingeben.\n");
        return false;
    } else if (edit_choice < 1 || edit_choice > 3) {
        fprintf(stderr, "Bitte nur eine Zahl zwischen 1 und 3 eingeben.\n");
        return false;
    }

    switch (edit_choice) {
        case 1: {
            printf("Neuer Name: ");
            char name[101];
            if (scanf("%100[^\n]", name) != 1) {
                fprintf(stderr, "Ungültiger Rezept Name.\n");
                return false;
            }
            clear_input_buffer();

            recipes[recipe_index - 1].name = duplicatestr(name);
            break;
        }
        case 2: {
            printf("Neue Anzahl an Zutaten: ");
            int ingredient_count;
            if (scanf("%d", &ingredient_count) != 1) {
                fprintf(stderr, "Invalide Eingabe, die Anzahl der Zutaten soll eine Zahl sein.\n");
                return false;
            }
            clear_input_buffer();

            recipes[recipe_index - 1].ingredient_count = ingredient_count;

            recipes[recipe_index - 1].ingredients = (Ingredient *)malloc(ingredient_count * sizeof(Ingredient));
            for (int i = 0; i < ingredient_count; i++) {
                printf("Zutat %d:\n", i + 1);
                printf("Name: ");
                char ingredient_name[101];
                if (scanf("%100[^\n]", ingredient_name) != 1) {
                    fprintf(stderr, "Ungültiger Name für eine Zutat.\n");
                    return false;
                }
                clear_input_buffer();

                recipes[recipe_index - 1].ingredients[i].name = lowercase(duplicatestr(ingredient_name));

                printf("Menge: ");
                char ingredient_quantity[100];
                if (scanf("%100[^\n]", ingredient_quantity) != 1) {
                    fprintf(stderr, "Ungültiger Menge an Zutaten.\n");
                    return false;
                }
                clear_input_buffer();

                recipes[recipe_index - 1].ingredients[i].quantity = duplicatestr(ingredient_quantity);
            }
            break;
        }
        case 3: {
            printf("Neue Anleitung:\n");
            char instructions[1000];
            if (fgets(instructions, sizeof(instructions), stdin) == NULL) {
                fprintf(stderr, "Ungültige Eingabe.\n");
                return false;
            }
            instructions[strcspn(instructions, "\n")] = '\0'; // https://stackoverflow.com/a/28462221
            recipes[recipe_index - 1].instructions = duplicatestr(instructions);
            break;
        }
        default: {
            fprintf(stderr, "Ungültige Eingabe.\n");
            freerecipes(recipes, recipe_count);
            free(json_data);
            return false;
        }
    }

    cJSON *json_recipes = cJSON_Parse(json_data);
    if (json_recipes == NULL) {
        fprintf(stderr, "Ein Fehler ist beim Laden von der JSON Rezept Datei aufgetreten.\n");
        freerecipes(recipes, recipe_count);
        free(json_data);
        return false;
    }

    cJSON *json_recipe = cJSON_GetArrayItem(json_recipes, recipe_index - 1);
    if (json_recipe == NULL || !cJSON_IsObject(json_recipe)) {
        fprintf(stderr, "Ein Fehler ist beim Laden von Rezept %d aufgetreten.\n", recipe_index);
        freerecipes(recipes, recipe_count);
        free(json_data);
        return false;
    }

    cJSON *json_name = cJSON_GetObjectItem(json_recipe, "name");
    cJSON *json_ingredients = cJSON_GetObjectItem(json_recipe, "ingredients");
    cJSON *json_instructions = cJSON_GetObjectItem(json_recipe, "instructions");

    if (json_name == NULL || !cJSON_IsString(json_name) || json_ingredients == NULL || !cJSON_IsArray(json_ingredients) || json_instructions == NULL || !cJSON_IsString(json_instructions)) {
        fprintf(stderr, "Invalider JSON.\n");
        freerecipes(recipes, recipe_count);
        free(json_data);
        return false;
    }

    cJSON_DeleteItemFromObject(json_recipe, "name");
    cJSON_DeleteItemFromObject(json_recipe, "ingredients");
    cJSON_DeleteItemFromObject(json_recipe, "instructions");

    cJSON_AddStringToObject(json_recipe, "name", recipes[recipe_index - 1].name);

    cJSON *json_new_ingredients = cJSON_CreateArray();
    for (int i = 0; i < recipes[recipe_index - 1].ingredient_count; i++) {
        cJSON *json_ingredient = cJSON_CreateObject();
        cJSON_AddStringToObject(json_ingredient, "name", recipes[recipe_index - 1].ingredients[i].name);
        cJSON_AddStringToObject(json_ingredient, "quantity", recipes[recipe_index - 1].ingredients[i].quantity);
        cJSON_AddItemToArray(json_new_ingredients, json_ingredient);
    }
    cJSON_AddItemToObject(json_recipe, "ingredients", json_new_ingredients);

    cJSON_AddStringToObject(json_recipe, "instructions", recipes[recipe_index - 1].instructions);

    char *updated_json_data = cJSON_Print(json_recipes);
    if (updated_json_data == NULL) {
        fprintf(stderr, "Ein Fehler ist beim Speichern aufgetreten.\n");
        cJSON_Delete(json_recipes);
        freerecipes(recipes, recipe_count);
        free(json_data);
        return false;
    }
    FILE *file = fopen(recipe_file, "w");
    if (file == NULL) {
        fprintf(stderr, "Konnte Datei nicht öffnen.\n");
        free(updated_json_data);
        cJSON_Delete(json_recipes);
        freerecipes(recipes, recipe_count);
        free(json_data);
        return false;
    }

    printf("Rezept mit der Nummer %d erfolgreich bearbeitet.\n", recipe_index);

    fprintf(file, "%s\n", updated_json_data);
    fclose(file);

    free(updated_json_data);

    cJSON_Delete(json_recipes);
    freerecipes(recipes, recipe_count);
    free(json_data);

    return true;
}
