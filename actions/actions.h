#ifndef ACTIONS_H
#define ACTIONS_H

bool add(int *recipe_count, char *recipe_file);
bool delete(int *recipe_count, char *recipe_file);
bool edit(int *recipe_count, char *recipe_file);
void list(int *recipe_count, char *recipe_file);
bool search(int *recipe_count, char *recipe_file);

#endif
