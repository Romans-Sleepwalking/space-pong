#ifndef _PONG_LIB_H
#define _PONG_LIB_H
  #include <stdbool.h>
    /* Direction constants */
    #define UP 1
    #define DOWN -1
    #define FILENAME "server.c"
    
    /* All related to shared memory block   */
    
    char * attach_memory_block(char *filename, int size);
    bool detach_memory_block(char *block);
    bool destroy_memory_block(char *filename);
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

    /* FOR TESTING */
    void start_round(double* ball_cx, double* ball_cy, double* ball_vx, double* ball_vy);
    void move_ball(double* ballCX, double* ballCY, double* ballVX, double* ballVY, int* lScore, int* rScore,
                   double* rPaddleCX, double* rPaddleCY, double* lPaddleCX, double* lPaddleCY);
    void move_paddle(char direction, double* coord, double* speed);

    /* GUI */
    void initGrid(char* l_team_name_ptr, char* r_team_name_ptr, int* l_team_score_ptr, int* r_team_score_ptr,
                  int* grid_columns_ptr, int* grid_rows_ptr, double* paddle_width_ptr, double* paddle_height_ptr,
                  double* ball_r_ptr, double* ball_cx_ptr, double* ball_cy_ptr,  double* ball_vx_ptr, double* ball_vy_ptr,
                  double* r_paddle_cx_ptr, double* r_paddle_cy_ptr, double* l_paddle_cx_ptr, double* l_paddle_cy_ptr);
    void drawGame();

#endif
