/**
 * @brief Duplicates a string
 * 
 * @param s Input string
 * @return char* duplicated string
 */
char *duplicate(const char *s) {
    char *new_str = malloc(strlen(s) + 1);
    if (new_str) {
        strcpy(new_str, s);
    }
    return new_str;
}
