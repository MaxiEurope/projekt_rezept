#ifndef RECIPE_H
#define RECIPE_H

typedef enum {
    GRAM,
    MILLILITER,
    PIECE,
    TABLESPOON,
    TEASPOON,
    CUP
} Unit;

typedef struct {
    char* name;
    unsigned int quantity;
    Unit unit;
} Ingredient;

typedef struct {
    char* name;
    char* instructions;
    int ingredient_count;
    Ingredient* ingredients;
    int valid;
} Recipe;

#endif
