#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/**
 * @brief Duplicates a string
 * 
 * @param s Input string
 * @return duplicated string
 */
char *duplicatestr(const char *s) {
    char *new_str = malloc(strlen(s) + 1);
    if (new_str) {
        strcpy(new_str, s);
    }
    return new_str;
}

/**
 * @brief Converts a string to lowercase
 * 
 * @param str The string to convert
 * @return The converted string
 */
char *lowercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
    return str;
}

/**
 * @brief Capitalizes the first letter of a string
 * 
 * @param str The string to modify
 * @return The modified string
 */
char *uppercasefirst(char *str) {
    if (str && *str) {
        str[0] = toupper(str[0]);
    }
    return str;
}

