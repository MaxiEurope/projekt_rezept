#ifndef RECIPEUTILS_H
#define RECIPEUTILS_H

#include "recipe.h"

void addrecipe(Recipe *new_recipe, int *recipe_count, char *recipe_file);
void freerecipes(Recipe* recipes, int count);
void getrecipecount(int *recipe_count, char *recipe_file);
Recipe* parserecipe(const char* json_data, int recipe_count);
void printrecipes(Recipe* recipes, int count);
char* readfile(const char* filepath);
void searchrecipe(Recipe* recipes, int *recipe_count, char** ingredients, int ingredient_count);

#endif
