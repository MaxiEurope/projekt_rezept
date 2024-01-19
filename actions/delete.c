#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../util/ext/cJSON.h"
#include "../util/str/strfunctions.h"
#include "../util/recipe.h"
#include "../util/recipeutil.h"

bool delete(int *recipe_count, char *recipe_file) {
    char *json_data = readfile(recipe_file);

    Recipe *recipes = parserecipe(json_data, *recipe_count);
    if (recipes == NULL) {
        fprintf(stderr, "Ein Fehler ist beim Laden von der JSON Rezept Datei aufgetreten.\n");
        free(json_data);
        exit(1);
        return false;
    }

    printf("Welches Rezept möchtest du löschen?\n");
    int recipe_index;
    scanf("%d", &recipe_index);
    if (recipe_index < 1 || recipe_index > *recipe_count) {
        printf("Invalide Eingabe, die Nummer des Rezepts soll eine Zahl sein.\n");
        freerecipes(recipes, *recipe_count);
        free(json_data);
        return false;
    }

    cJSON *json_recipes = cJSON_Parse(json_data);
    if (json_recipes == NULL) {
        printf("Ein Fehler ist beim Laden von der JSON Rezept Datei aufgetreten.\n");
        freerecipes(recipes, *recipe_count);
        free(json_data);
        exit(1);
        return false;
    }

    cJSON_DeleteItemFromArray(json_recipes, recipe_index - 1);

    char *updated_json_data = cJSON_Print(json_recipes);
    if (updated_json_data == NULL) {
        printf("Ein Fehler ist beim Speichern aufgetreten.\n");
        cJSON_Delete(json_recipes);
        freerecipes(recipes, *recipe_count);
        free(json_data);
        exit(1);
        return false;
    }
    FILE *file = fopen(recipe_file, "w");
    if (file == NULL) {
        printf("Konnte Datei nicht öffnen.\n");
        free(updated_json_data);
        cJSON_Delete(json_recipes);
        freerecipes(recipes, *recipe_count);
        free(json_data);
        exit(1);
        return false;
    }

    (*recipe_count)--;
    printf("Rezept erfolgreich gelöscht.\n");

    fprintf(file, "%s\n", updated_json_data);
    fclose(file);

    free(updated_json_data);
    cJSON_Delete(json_recipes);
    freerecipes(recipes, *recipe_count);
    free(json_data);

    return true;
}
