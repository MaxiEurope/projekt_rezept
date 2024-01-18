#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "util/ext/cJSON.h"
#include "util/str/duplicatestr.h"
#include "util/recipe.h"
#include "util/recipeutil.h"

/**
 * @brief Clears the input buffer
 * https://stackoverflow.com/a/7898516
 */
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

int recipe_count = 0;

int main() {
    printf("Hello, World!\n");

    getrecipecount(&recipe_count);
    printf("Es wurden %d Rezepte geladen.\n", recipe_count);
    
    while (1) {
        printf("Was möchtest du tun?\n");
        printf("1. Rezept hinzufügen\n");
        printf("2. Rezept anzeigen\n");
        printf("3. Rezept löschen\n");
        printf("4. Rezept bearbeiten\n");
        printf("5. Rezept suchen\n");
        printf("6. Beenden\n");
        printf("Auswahl: ");
        int choice;
        char term;
        if(scanf("%1d%c", &choice, &term) != 2 || term != '\n') {
            printf("Ungültige Eingabe. Bitte nur eine Zahl (1-6) eingeben.\n");
            while(getchar() != '\n');
            continue;
        } else if(choice < 1 || choice > 6) {
            printf("Bitte nur eine Zahl zwischen 1 und 6 eingeben.\n");
            continue;
        }
        switch (choice) {
            case 1: {
                Recipe *new_recipe = (Recipe *) malloc(sizeof(Recipe));
                printf("Name: ");
                char name[101];
                if (scanf("%100[^\n]", name) != 1) {
                    printf("Ungültiger Rezept Name.\n");
                    return 1;
                }
                name[strcspn(name, "\n")] = '\0'; // https://stackoverflow.com/a/28462221
                new_recipe->name = duplicatestr(name);
                clear_input_buffer();
                printf("Anzahl Zutaten: ");
                int ingredient_count;
                int res = scanf("%d", &ingredient_count);
                if (res != 1) {
                    printf("Invalid Eingabe, die Anzahl der Zutaten soll eine Zahl sein, von vorne beginnen.\n");
                    continue;
                }
                new_recipe->ingredient_count = ingredient_count;
                new_recipe->ingredients = (Ingredient *) malloc(ingredient_count * sizeof(Ingredient));
                for (int i = 0; i < ingredient_count; i++) {
                    printf("Zutat %d:\n", i + 1);
                    printf("Name: ");
                    char ingredient_name[100];
                    scanf("%s", ingredient_name);
                    new_recipe->ingredients[i].name = duplicatestr(ingredient_name);
                    printf("Menge: ");
                    char ingredient_quantity[100];
                    scanf("%s", ingredient_quantity);
                    new_recipe->ingredients[i].quantity = duplicatestr(ingredient_quantity);
                }
                printf("Anleitung:\n");
                clear_input_buffer();
                char instructions[1000];
                if (fgets(instructions, sizeof(instructions), stdin) == NULL) {
                    printf("Ungültige Eingabe.\n");
                    return 1;
                }
                instructions[strcspn(instructions, "\n")] = '\0'; // https://stackoverflow.com/a/28462221
                new_recipe->instructions = duplicatestr(instructions);
                addrecipe(new_recipe, &recipe_count);
                free(new_recipe);
                break;
            }
            case 2: {
                char *json_data = readfile("recipes.json");
                if (json_data == NULL) {
                    printf("Konnte Datei nicht öffnen.\n");
                    return 1;
                }

                Recipe *recipes = parserecipe(json_data, &recipe_count);
                if (recipes == NULL) {
                    printf("Ein Fehler ist beim Laden von der JSON Rezept Datei aufgetreten.\n");
                    free(json_data);
                    return 1;
                }

                printrecipes(recipes, recipe_count);
                freerecipes(recipes, recipe_count);

                free(json_data);
                break;
            }
            case 3: {
                char *json_data = readfile("recipes.json");
                if (json_data == NULL) {
                    printf("Konnte Datei nicht öffnen.\n");
                    return 1;
                }

                Recipe *recipes = parserecipe(json_data, &recipe_count);
                if (recipes == NULL) {
                    printf("Ein Fehler ist beim Laden von der JSON Rezept Datei aufgetreten.\n");
                    free(json_data);
                    return 1;
                }

                printf("Welches Rezept möchtest du löschen?\n");
                int recipe_index;
                scanf("%d", &recipe_index);
                if (recipe_index < 1 || recipe_index > recipe_count) {
                    printf("Ungültige Eingabe.\n");
                    freerecipes(recipes, recipe_count);
                    free(json_data);
                    return 1;
                }

                cJSON *json_recipes = cJSON_Parse(json_data);
                if (json_recipes == NULL) {
                    printf("Ein Fehler ist beim Laden von der JSON Rezept Datei aufgetreten.\n");
                    freerecipes(recipes, recipe_count);
                    free(json_data);
                    return 1;
                }

                cJSON_DeleteItemFromArray(json_recipes, recipe_index - 1);

                char *updated_json_data = cJSON_Print(json_recipes);
                if (updated_json_data == NULL) {
                    printf("Ein Fehler ist beim Speichern aufgetreten.\n");
                    cJSON_Delete(json_recipes);
                    freerecipes(recipes, recipe_count);
                    free(json_data);
                    return 1;
                }
                FILE *file = fopen("recipes.json", "w");
                if (file == NULL) {
                    printf("Konnte Datei nicht öffnen.\n");
                    free(updated_json_data);
                    cJSON_Delete(json_recipes);
                    freerecipes(recipes, recipe_count);
                    free(json_data);
                    return 1;
                }

                printf("Rezept gelöscht.\n");
                printf("Rezeptanzahl: %d\n", recipe_count - 1);

                fprintf(file, "%s\n", updated_json_data);
                fclose(file);

                free(updated_json_data);
                cJSON_Delete(json_recipes);
                freerecipes(recipes, recipe_count);
                free(json_data);
                break;
            }
            case 4: {
                char *json_data = readfile("recipes.json");
                if (json_data == NULL) {
                    printf("Konnte Datei nicht öffnen.\n");
                    return 1;
                }

                Recipe *recipes = parserecipe(json_data, &recipe_count);
                if (recipes == NULL) {
                    printf("Ein Fehler ist beim Laden von der JSON Rezept Datei aufgetreten.\n");
                    free(json_data);
                    return 1;
                }

                printf("Welches Rezept möchtest du bearbeiten?\n");
                int recipe_index;
                scanf("%d", &recipe_index);
                if (recipe_index < 1 || recipe_index > recipe_count) {
                    printf("Ungültige Eingabe.\n");
                    freerecipes(recipes, recipe_count);
                    free(json_data);
                    return 1;
                }

                printf("Was möchtest du bearbeiten?\n");
                printf("1. Name\n");
                printf("2. Zutaten\n");
                printf("3. Anleitung\n");
                printf("Auswahl: ");
                int edit_choice;
                char edit_term;
                if(scanf("%1d%c", &edit_choice, &edit_term) != 2 || edit_term != '\n') {
                    printf("Ungültige Eingabe. Bitte nur eine Zahl (1-3) eingeben.\n");
                    return 1;
                } else if(edit_choice < 1 || edit_choice > 3) {
                    printf("Bitte nur eine Zahl zwischen 1 und 3 eingeben.\n");
                    return 1;
                }
                switch (edit_choice) {
                    case 1: {
                        printf("Neuer Name: ");
                        char name[100];
                        scanf("%s", name);
                        recipes[recipe_index - 1].name = duplicatestr(name);
                        break;
                    }
                    case 2: {
                        printf("Anzahl Zutaten: ");
                        int ingredient_count;
                        scanf("%d", &ingredient_count);
                        recipes[recipe_index - 1].ingredient_count = ingredient_count;
                        recipes[recipe_index - 1].ingredients = (Ingredient *) malloc(ingredient_count * sizeof(Ingredient));
                        for (int i = 0; i < ingredient_count; i++) {
                            printf("Zutat %d:\n", i + 1);
                            printf("Name: ");
                            char ingredient_name[100];
                            scanf("%s", ingredient_name);
                            recipes[recipe_index - 1].ingredients[i].name = duplicatestr(ingredient_name);
                            printf("Menge: ");
                            char ingredient_quantity[100];
                            scanf("%s", ingredient_quantity);
                            recipes[recipe_index - 1].ingredients[i].quantity = duplicatestr(ingredient_quantity);
                        }
                        break;
                    }
                    case 3: {
                        printf("Neue Anleitung:\n");
                        char instructions[1000];
                        scanf("%s", instructions);
                        recipes[recipe_index - 1].instructions = duplicatestr(instructions);
                        break;
                    }
                    default: {
                        printf("Ungültige Eingabe.\n");
                        freerecipes(recipes, recipe_count);
                        free(json_data);
                        return 1;
                    }
                }


                cJSON *json_recipes = cJSON_Parse(json_data);
                if (json_recipes == NULL) {
                    printf("Ein Fehler ist beim Laden von der JSON Rezept Datei aufgetreten.\n");
                    freerecipes(recipes, recipe_count);
                    free(json_data);
                    return 1;
                }

                cJSON *json_recipe = cJSON_GetArrayItem(json_recipes, recipe_index - 1);
                if (json_recipe == NULL || !cJSON_IsObject(json_recipe)) {
                    printf("Ein Fehler ist beim Laden von Rezept %d aufgetreten.\n", recipe_index);
                    freerecipes(recipes, recipe_count);
                    free(json_data);
                    return 1;
                }

                cJSON *json_name = cJSON_GetObjectItem(json_recipe, "name");
                cJSON *json_ingredients = cJSON_GetObjectItem(json_recipe, "ingredients");
                cJSON *json_instructions = cJSON_GetObjectItem(json_recipe, "instructions");

                if (json_name == NULL || !cJSON_IsString(json_name) || json_ingredients == NULL || !cJSON_IsArray(json_ingredients) || json_instructions == NULL || !cJSON_IsString(json_instructions)) {
                    freerecipes(recipes, recipe_count);
                    free(json_data);
                    return 1;
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
                    printf("Ein Fehler ist beim Speichern aufgetreten.\n");
                    cJSON_Delete(json_recipes);
                    freerecipes(recipes, recipe_count);
                    free(json_data);
                    return 1;
                }
                FILE *file = fopen("recipes.json", "w");
                if (file == NULL) {
                    printf("Konnte Datei nicht öffnen.\n");
                    free(updated_json_data);
                    cJSON_Delete(json_recipes);
                    freerecipes(recipes, recipe_count);
                    free(json_data);
                    return 1;
                }

                printf("Rezept bearbeitet.\n");

                fprintf(file, "%s\n", updated_json_data);
                fclose(file);

                free(updated_json_data);

                cJSON_Delete(json_recipes);
                freerecipes(recipes, recipe_count);
                free(json_data);
                break;
            }
            case 5: {
                char *json_data = readfile("recipes.json");
                if (json_data == NULL) {
                    printf("Konnte Datei nicht öffnen.\n");
                    return 1;
                }

                Recipe *recipes = parserecipe(json_data, &recipe_count);
                if (recipes == NULL) {
                    printf("Ein Fehler ist beim Laden von der JSON Rezept Datei aufgetreten.\n");
                    free(json_data);
                    return 1;
                }

                printf("Wie viele Zutaten möchtest du eingeben?\n");
                int ingredient_count;
                scanf("%d", &ingredient_count);
                char **ingredients = (char **) malloc(ingredient_count * sizeof(char *));
                for (int i = 0; i < ingredient_count; i++) {
                    printf("Zutat %d: ", i + 1);
                    char ingredient[100];
                    scanf("%s", ingredient);
                    ingredients[i] = duplicatestr(ingredient);
                }
                printf("Rezepte:\n");
                searchrecipe(recipes, &recipe_count, ingredients, ingredient_count);

                freerecipes(recipes, recipe_count);
                free(json_data);
                break;
            }
            case 6: {
                printf("Programm wird beendet.\n");
                return 0;
            }
            default: {
                printf("Ungültige Eingabe.\n");
                return 1;
            }
        }
    }

    return 0;
}
