#ifndef _PONG_LIB_H
#define _PONG_LIB_H

    #include <stdbool.h>
    /* Direction constants */
    #define UP 1
    #define DOWN -1
    #define FILENAME "server.c"


    /* ===== utility_lib.c ===== */

    /* Parameter Operations */
    char* read_param_value(int argc, char** argv, char magic_char);
    /* String Operations */
    int str_length(char *string);
    void str_copy(char* source, char* destination);
    int str_find(char* needle, char* haystack);
    int unescape(char* char_ptr);
    /* Packet Operations */
    int get_4_bit_integer(void * addr);
    void put_4_bit_integer(int data, void* addr);
    char printable_char(char c);
    void print_Bytes(void* packet, int size);
    char calculate_checksum(char* buffer, int n);
    int is_packet(char* buffer, int n, int last_id);
    /* Memory Block Operations */
    char* attach_memory_block(char *filename, int size);
    bool detach_memory_block(char *block);
    bool destroy_memory_block(char *filename);

    /* ===== game_lib.c ===== */

    /* Game Launch */
    void launch_game(int* game_state);
    void local_test_game(double* game_state_memory_ptr);
    /* Start Round */
    void start_round(double* ball_cx, double* ball_cy, double* ball_vx, double* ball_vy);
    /* Move Objects */
    void move_ball(double* ballCX, double* ballCY, double* ballVX, double* ballVY, int* lScore, int* rScore,
            double* rPaddleCX, double* rPaddleCY, double* lPaddleCX, double* lPaddleCY);
    void move_paddle(char direction, double* coord, double* speed);

    /* ===== gui_lib.c ===== */

    /* Initialize the game */
    void initGrid(char* l_team_name_ptr, char* r_team_name_ptr, int* l_team_score_ptr, int* r_team_score_ptr,
                  int* grid_columns_ptr, int* grid_rows_ptr, double* paddle_width_ptr, double* paddle_height_ptr,
                  double* ball_r_ptr, double* ball_cx_ptr, double* ball_cy_ptr,  double* ball_vx_ptr, double* ball_vy_ptr,
                  double* r_paddle_cx_ptr, double* r_paddle_cy_ptr, double* l_paddle_cx_ptr, double* l_paddle_cy_ptr);
    /* Refresh the displayed information */
    void drawGame();

#endif
