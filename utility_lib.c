/* PBM765 allowed library */
#include <stdlib.h>

/*
 * Space-pong library segment that
 *      helps establish the successful connection
 *      contains basic functional
 */

/* Finds and picks a parameter value substring from a parameter array */
char* read_param_value(int argc, char** argv, char magic_char){
    int i, j;
    char* substring = malloc(32);
    /* Finds the required parameter */
    for (i=0; i<argc; i++){
        if (argv[i][0] == '-' && argv[i][1] == magic_char && argv[i][2] == '='){
            j = 3;
            /* Loops until meets EOF character */
            while (1){
                /* Fills the substring character array */
                substring[j-3] = argv[i][j];
                if (argv[i][j] == '\0'){
                    break;
                } else j++;
            }
            return substring;
        }
    }
    return "404";
}

/* Counts string length */
int str_length(char* string){
    /* Loops string characters until EOF char, counting length */
    int length;
    for (length = 0; string[length] != '\0'; length++);
    return length;
}

/* Copies string content inside the another string by pointers */
void str_copy(char* source, char* destination){
    while ((*destination++ = *source++));
}

/* Tries to find a substring */
int str_find(char* needle, char* haystack){
    int len_haystack = str_length(haystack);
    int len_needle = str_length(needle);
    int needle_char_match_count = 0;
    int i;
    /* Loops haystack's length where neddle is possible */
    for (i = 0; i < len_haystack - len_needle; i++) {
        /* Checks every next character for matching the substring */
        while (haystack[i + needle_char_match_count] == needle[needle_char_match_count]){
            needle_char_match_count++;
            /* If match, returns the substring first index */
            if (needle_char_match_count == len_needle){
                return i + 1;
            }
        }
        /* Moves to next haystack character character */
        needle_char_match_count = 0;
    }
    /* If no substring found, returns the -1 magic number  */
    return -1;
}
