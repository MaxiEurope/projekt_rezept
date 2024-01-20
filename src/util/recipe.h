#ifndef RECIPE_H
#define RECIPE_H

typedef struct {
    char* name;
    char* quantity;
} Ingredient;

typedef struct {
    char* name;
    char* instructions;
    int ingredient_count;
    Ingredient* ingredients;
    int valid;
} Recipe;

#endif
