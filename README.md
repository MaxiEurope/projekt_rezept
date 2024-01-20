# Recipe project

## Compile
```bash
gcc -std=c99 -Wall -Wextra -pedantic -Wno-unused-parameter main.c -lncurses actions/add.c actions/del.c actions/edit.c actions/list.c actions/search.c util/ext/cJSON.c util/str/strfunctions.c util/str/getargs.c util/addrecipe.c util/freerecipes.c util/getrecipecount.c util/parserecipe.c util/printrecipe.c util/readfile.c util/searchrecipe.c -o recipe
```
## Run
```bash
./recipe -f recipes.json
```

## Tasks &#9744;

### &#9745; Task 1 (30 points)
  * Define a human- and computer-readable file format that is able to store named recipes with the required amounts of ingredients.
  * Read that file in and let the user select one of the recipes to display its instructions below a list of the respective ingredients.

### &#9745; Task 2 (10 points)
  * The user can list all recipes requiring a specified list of ingredients.

### &#9744; Task 3 (15 points)
  * Allow the user to specify a list of ingredients with respective amounts and show only those recipes that can be made using them,
    i.e. list all recipes that do not require other types or a higher amount of the ingredients than specified.

### &#9745; Task 4 (15 points)
  * Adding new recipes is possible interactively within the program, which appends the new recipe to the persistent file store.

### &#9745; Task 5 (15 points)
  * Let the user edit existing recipes within the program.

### &#9744; Task 6 (30 points)
  * Implement a nice user interface using the `ncurses` library.
  * Let the user scroll a list of all recipes to select any of them.
  * Allow the user then to take an action on the respective recipe (e.g., edit it).

### &#9744; Task 7 (10 points)
  * Provide a way within the `ncurses` interface to apply filtering of the recipes similar to the search modes described above.

### &#9745; Task 8 (15 points)
  * The program arguments are handled by `getopt(3)`.

## Doxygen
```bash
doxygen Doxyfile
```