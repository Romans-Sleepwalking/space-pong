#ifndef _PONG_LIB_H
#define _PONG_LIB_H

    /* Picks substring from a parameter character array */
    char* read_param_value(int argc, char** argv, char magic_char);
    /* Counts string length */
    int str_length(char *string);
    /* Copies string content inside the another string by pointers */
    void str_copy(char* source, char* destination);
    /* Tries to find a substring */
    int str_find(char* needle, char* haystack);

    /* Equivalent to the main function for the game session */
    void launch_game(int* game_state);
    void local_test_game(double* game_state_memory_ptr);

    /* GUI */
    void initGrid(int x, int y, double* game_state_memory_ptr);
    void drawGame();

    /* Directions */
    #define UP 1
    #define DOWN -1

#endif
