#include <stdlib.h>


/* Picks substring from a parameter character array */
char* read_parameter_value(char* string) {
    int i = 3;
    char* substring = malloc(64);;
    /* Loops until meets EOF character */
    while (1){
        substring[i-3] = string[i];
        if (string[i] == '\0'){
            break;
        } else {
            i++;
        }
    }
    return substring;
}
