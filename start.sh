gcc -std=c99 -Wall -Wextra -pedantic -Wno-unused-parameter main.c -lncurses src/actions/add.c src/actions/del.c src/actions/edit.c src/actions/list.c src/actions/search.c src/util/ext/cJSON.c src/util/str/strfunctions.c src/util/str/getargs.c src/util/addrecipe.c src/util/freerecipes.c src/util/getrecipecount.c src/util/parserecipe.c src/util/printrecipe.c src/util/readfile.c src/util/searchrecipe.c -o recipe ; ./recipe -f a.json