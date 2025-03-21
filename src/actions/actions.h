#ifndef ACTIONS_H
#define ACTIONS_H

#include "../util/recipe.h"

bool add(int *recipe_count, char *recipe_file);
bool del(int *recipe_count, char *recipe_file);
bool edit(int *recipe_count, char *recipe_file);
bool list(int *recipe_count, char *recipe_file);
bool search(int *recipe_count, char *recipe_file);

#endif
