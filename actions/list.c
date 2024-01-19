#include <stdlib.h>

#include "../util/recipeutil.h"

/**
 * @brief Lists all recipes
 * 
 * @param recipe_count The number of recipes
 * @param recipe_file Path to the recipe file
 */
void list(int *recipe_count, char *recipe_file) {
    char *json_data = readfile(recipe_file);

    Recipe *recipes = parserecipe(json_data, *recipe_count);

    printrecipes(recipes, *recipe_count);
    freerecipes(recipes, *recipe_count);

    free(json_data);
}
