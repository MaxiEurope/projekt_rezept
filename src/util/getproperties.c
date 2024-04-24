#include <stdio.h>
#include "recipe.h"
#include "recipeutil.h"
#include <ncurses.h>

MealCategory get_mealcategory(char *recipe_name) {
    int choice;
    print_categories();
    printw("Kategorie: ");
    scanw("%d", &choice);

    switch (choice) {
        case 1: return VORSPEISEN;
        case 2: return HAUPTGERICHTE;
        case 3: return DESSERTS;
        case 4: return VEGETARISCH;
        case 5: return VEGAN;
        default:
            clear();
            printw("Ungültige Auswahl/Kategorie für dieses Rezept: %s\n", recipe_name);
            refresh();
            return get_mealcategory(recipe_name);
    }
}

Unit get_unit(char *ingredient_name) {
    int choice;
    print_units();
    printw("Einheit: ");
    scanw("%d", &choice);

    switch (choice) {
        case 1: return GRAM;
        case 2: return MILLILITER;
        case 3: return PIECE;
        case 4: return TABLESPOON;
        case 5: return TEASPOON;
        case 6: return CUP;
        default:
            clear();
            printw("Ungültige Auswahl/Einheit für diese Zutat: %s\n", ingredient_name);
            refresh();
            return get_unit(ingredient_name);
    }
}
