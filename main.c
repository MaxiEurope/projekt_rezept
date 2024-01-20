#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ncurses.h>

#include "src/actions/actions.h"
#include "src/util/ext/cJSON.h"
#include "src/util/str/strfunctions.h"
#include "src/util/str/getargs.h"
#include "src/util/recipe.h"
#include "src/util/recipeutil.h"

int recipe_count = 0;
char *recipe_file = NULL;

int main(int argc, char *argv[]) {
    initscr();
    echo();
    nocbreak();
    clear();
    refresh();

    if (!getargs(argc, argv, &recipe_file)) {
        return 1;
    }

    getrecipecount(&recipe_count, recipe_file);
    printw("Es wurden %d Rezepte geladen.\n", recipe_count);
    refresh();

    while (1) {
        printw("Was möchtest du tun?\n");
        printw("1. Rezept hinzufügen\n");
        printw("2. Rezept anzeigen\n");
        printw("3. Rezept löschen\n");
        printw("4. Rezept bearbeiten\n");
        printw("5. Rezept suchen\n");
        printw("6. Beenden\n");
        printw("Auswahl: ");
        refresh();

        int choice;
        if(scanw("%1d", &choice) != 1) {
            clear();
            printw("Ungültige Eingabe. Bitte nur eine Zahl (1-6) eingeben.\n");
            refresh();
            continue;
        } else if(choice < 1 || choice > 6) {
            clear();
            printw("Bitte nur eine Zahl zwischen 1 und 6 eingeben.\n");
            refresh();
            continue;
        }

        switch (choice) {
            case 1: {
                clear();
                bool success = add(&recipe_count, recipe_file);
                if (!success) {
                    printw("Bitte von vorne beginnen.\n");
                    refresh();
                }
                break;
            }
            case 2: {
                clear();
                if (recipe_count == 0) {
                    printw("Es gibt keine Rezepte zum Lesen.\n");
                    refresh();
                    break;
                }

                bool success = list(&recipe_count, recipe_file);
                if (!success) {
                    printw("Bitte erneut versuchen.\n");
                    refresh();
                }
                break;
            }
            case 3: {
                clear();
                if (recipe_count == 0) {
                    printw("Es gibt keine Rezepte zum Löschen.\n");
                    refresh();
                    break;
                }

                bool success = del(&recipe_count, recipe_file);
                if (!success) {
                    printw("Bitte erneut versuchen.\n");
                    refresh();
                }
                break;
            }
            case 4: {
                clear();
                if (recipe_count == 0) {
                    printw("Es gibt keine Rezepte zum Editieren.\n");
                    refresh();
                    break;
                }

                bool success = edit(&recipe_count, recipe_file);
                if (!success) {
                    printw("Bitte von vorne beginnen.\n");
                    refresh();
                }
                break;
            }
            case 5: {
                clear();
                if (recipe_count == 0) {
                    printw("Es gibt keine Rezepte zum Durchsuchen.\n");
                    refresh();
                    break;
                }

                bool success = search(&recipe_count, recipe_file);
                if (!success) {
                    printw("Bitte von vorne beginnen.\n");
                    refresh();
                }
                break;
            }
            case 6: {
                clear();
                printw("Programm wird beendet, bye.\n");
                refresh();
                napms(1000);
                refresh();
                endwin();
                return 0;
            }
            default: {
                printw("Ungültige Eingabe.\n");
                refresh();
                endwin();
                return 1;
            }
        }
    }

    return 0;
}
