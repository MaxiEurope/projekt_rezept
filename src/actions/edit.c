#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <limits.h>
#include <ncurses.h>

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

    Recipe *recipes = parserecipe(json_data, *recipe_count);

    printw("Welches Rezept möchtest du bearbeiten?\n");

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
        printw("Ungültige Eingabe.\n");
        refresh();
        freerecipes(recipes, *recipe_count);
        free(json_data);
        return false;
    }

    clear();
    refresh();

    printw("Was möchtest du bearbeiten?\n");
    printw("1. Name\n");
    printw("2. Zutaten\n");
    printw("3. Anleitung\n");
    printw("Auswahl: ");
    refresh();

    int choice;
    if (scanw("%1d%c", &choice) != 1) {
        clear();
        printw("Ungültige Eingabe. Bitte nur eine Zahl (1-3) eingeben.\n");
        refresh();
        return false;
    } else if (choice < 1 || choice > 3) {
        clear();
        printw("Bitte nur eine Zahl zwischen 1 und 3 eingeben.\n");
        refresh();
        return false;
    }

    clear();
    refresh();

    switch (choice) {
        case 1: {
            printw("Neuer Name: ");
            refresh();
            char name[101];
            if (scanw("%100[^\n]", name) != 1) {
                clear();
                printw("Ungültiger Rezept Name.\n");
                refresh();
                return false;
            }

            recipes[recipe_index - 1].name = duplicatestr(name);
            break;
        }
        case 2: {
            printw("Neue Anzahl an Zutaten: ");
            refresh();
            int ingredient_count;
            if (scanw("%d", &ingredient_count) != 1) {
                clear();
                printw("Invalide Eingabe, die Anzahl der Zutaten soll eine Zahl sein.\n");
                refresh();
                return false;
            }

            recipes[recipe_index - 1].ingredient_count = ingredient_count;

            recipes[recipe_index - 1].ingredients = (Ingredient *)malloc(ingredient_count * sizeof(Ingredient));
            for (int i = 0; i < ingredient_count; i++) {
                printw("Zutat %d:\n", i + 1);
                refresh();
                printw("Name: ");
                refresh();
                char ingredient_name[101];
                if (scanw("%100[^\n]", ingredient_name) != 1) {
                    clear();
                    printw("Ungültiger Name für eine Zutat.\n");
                    refresh();
                    return false;
                }

                recipes[recipe_index - 1].ingredients[i].name = lowercase(duplicatestr(ingredient_name));

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

                recipes[recipe_index - 1].ingredients[i].quantity = (unsigned int)ingredient_quantity;

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

                recipes[recipe_index - 1].ingredients[i].unit = unit_choice;
            }
            break;
        }
        case 3: {
            printw("Neue Anleitung: ");
            refresh();
            char instructions[1001];
            if (scanw("%1000[^\n]", instructions) != 1) {
                clear();
                printw("Ungültige Eingabe.\n");
                refresh();
                return false;
            }

            recipes[recipe_index - 1].instructions = duplicatestr(instructions);
            break;
        }
        default: {
            clear();
            printw("Ungültige Eingabe.\n");
            refresh();
            freerecipes(recipes, *recipe_count);
            free(json_data);
            return false;
        }
    }

    cJSON *json_recipes = cJSON_Parse(json_data);
    if (json_recipes == NULL) {
        clear();
        printw("Ein Fehler ist beim Laden von der JSON Rezept Datei aufgetreten.\n");
        refresh();
        freerecipes(recipes, *recipe_count);
        free(json_data);
        return false;
    }

    cJSON *json_recipe = cJSON_GetArrayItem(json_recipes, recipe_index - 1);
    if (json_recipe == NULL || !cJSON_IsObject(json_recipe)) {
        clear();
        printw("Ein Fehler ist beim Laden von Rezept %d aufgetreten.\n", recipe_index);
        refresh();
        freerecipes(recipes, *recipe_count);
        free(json_data);
        return false;
    }

    cJSON *json_name = cJSON_GetObjectItem(json_recipe, "name");
    cJSON *json_ingredients = cJSON_GetObjectItem(json_recipe, "ingredients");
    cJSON *json_instructions = cJSON_GetObjectItem(json_recipe, "instructions");

    if (json_name == NULL || !cJSON_IsString(json_name) || json_ingredients == NULL || !cJSON_IsArray(json_ingredients) || json_instructions == NULL || !cJSON_IsString(json_instructions)) {
        clear();
        printw("Invalider JSON.\n");
        refresh();
        freerecipes(recipes, *recipe_count);
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
        clear();
        printw("Ein Fehler ist beim Speichern aufgetreten.\n");
        refresh();
        cJSON_Delete(json_recipes);
        freerecipes(recipes, *recipe_count);
        free(json_data);
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
        return false;
    }

    clear();
    printw("Rezept mit der Nummer %d erfolgreich bearbeitet.\n", recipe_index);
    refresh();

    fprintf(file, "%s\n", updated_json_data);
    fclose(file);

    free(updated_json_data);

    cJSON_Delete(json_recipes);
    freerecipes(recipes, *recipe_count);
    free(json_data);

    return true;
}
