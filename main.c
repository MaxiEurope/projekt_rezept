#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ncurses.h>

#include "actions/actions.h"
#include "util/ext/cJSON.h"
#include "util/str/strfunctions.h"
#include "util/str/getargs.h"
#include "util/recipe.h"
#include "util/recipeutil.h"

int recipe_count = 0;
char *recipe_file = NULL;

int main(int argc, char *argv[]) {
    printf("Hello, World!\n");

    if (!getargs(argc, argv, &recipe_file)) {
        return 1;
    }

    getrecipecount(&recipe_count, recipe_file);
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
            clear_input_buffer();
            continue;
        } else if(choice < 1 || choice > 6) {
            printf("Bitte nur eine Zahl zwischen 1 und 6 eingeben.\n");
            continue;
        }


        switch (choice) {
            case 1: {
                bool success = add(&recipe_count, recipe_file);
                if (!success) {
                    printf("Bitte von vorne beginnen.\n");
                    clear_input_buffer();
                }
                break;
            }
            case 2: {
                list(&recipe_count, recipe_file);
                break;
            }
            case 3: {
                if (recipe_count == 0) {
                    printf("Es gibt keine Rezepte zum Löschen.\n");
                    break;
                }

                bool success = del(&recipe_count, recipe_file);
                if (!success) {
                    printf("Bitte erneut versuchen.\n");
                    clear_input_buffer();
                }
                break;
            }
            case 4: {
                if (recipe_count == 0) {
                    printf("Es gibt keine Rezepte zum Editieren.\n");
                    break;
                }

                bool success = edit(&recipe_count, recipe_file);
                if (!success) {
                    printf("Bitte von vorne beginnen.\n");
                    clear_input_buffer();
                }
                break;
            }
            case 5: {
                if (recipe_count == 0) {
                    printf("Es gibt keine Rezepte zum Durchsuchen.\n");
                    break;
                }

                bool success = search(&recipe_count, recipe_file);
                if (!success) {
                    printf("Bitte von vorne beginnen.\n");
                    clear_input_buffer();
                }
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
