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

    /* GUI */
    void initGrid(int x, int y);
    void drawGrid();
    void drawBall(float cx, float cy);
    void drawPaddle(double cy, char side);

#endif
