/* PBM765 allowed library */
#include<stdio.h>
#include <time.h>
/* Global default constants */
#define DEFAULT_REFRESH_RATE 3

/*
 * Space-pong library segment that
 *      creates a game
 *      contains pong logic methods
 */

/* Equivalent to the main function for the game session */
int launch_game(int* game_state){
    /* Timings */
    float seconds = 5;
    float refresh_rate = DEFAULT_REFRESH_RATE;
    clock_t timer;
    printf("\tPreparing the game with %1.1f seconds refresh rate... ", refresh_rate);

    /* ========== LOADING GAME MEMORY ========== */

    int* score_team_1 = game_state;
    int* score_team_2 = (int*)(score_team_1 +sizeof(int));
    /*game type is 1 or 2 (1 - 1v1, 2 - 2v2) */
    int* game_type = (int*)(score_team_2 +sizeof(int));
    int* ready_player_count =  (int*)(game_type + sizeof(int));

    /*ball info */
    float* ball_x = (float*)(ready_player_count + sizeof(int));
    float* ball_y = (float*)(ball_x + sizeof(float));
    int* ball_r = (int*)(ball_y + sizeof(float));
    int* ball_color = (int*)(ball_y + sizeof(int));
    /*/////////*/
    /*powerup info pagaidam neieliku jo isti nesaprotu kas ir domats api dokumenta:
    int* power_up_count = (int*)(ball_color + sizeof(int));
    int* player_id_w_powerup = (int*)(power_up_count + sizeof(int));
     */

    /*info about first player*/
    /* katru playeru vares atrast pec: float* player = (float*)(game type + sizeof(int) + sizeof(int)*20*(id)) */
    float* player1_x = (float*)(ball_color +sizeof(int));
    float* player1_y = (float*)(player1_x +sizeof(float));
    int* player1_height = (int*)(player1_y +sizeof(float));
    int* player1_width = (int*)(player1_height +sizeof(int));
    int* player1_color = (int*)(player1_width +sizeof(int));
    /*info about second player*/
    float* player2_x = (float*)(game_type +sizeof(int) + sizeof(int)*20);
    float* player2_y = (float*)(player2_x +sizeof(float));
    int* player2_height = (int*)(player2_y +sizeof(float));
    int* player2_width = (int*)(player2_height +sizeof(int));
    int* player2_color = (int*)(player2_width +sizeof(int));
    /*info about third player*/
    float* player3_x = (float*)(game_type +sizeof(int) + sizeof(int)*20*2);
    float* player3_y = (float*)(player3_x +sizeof(float));
    int* player3_height = (int*)(player3_y +sizeof(float));
    int* player3_width = (int*)(player3_height +sizeof(int));
    int* player3_color = (int*)(player3_width +sizeof(int));
    /*info about fourth player*/
    float* player4_x = (float*)(game_type +sizeof(int) + sizeof(int)*20*3);
    float* player4_y = (float*)(player4_x +sizeof(float));
    int* player4_height = (int*)(player4_y +sizeof(float));
    int* player4_width = (int*)(player4_height +sizeof(int));
    int* player4_color = (int*)(player4_width +sizeof(int));

    printf("Memory loaded... ");

    /* ========== LOOPS THE GAME PROCESS ========== */

    /* Starts games infinite loop */
    printf("Game Launched!\n");
    timer = clock();
    while (1){
        if ((((double)clock()-timer)/CLOCKS_PER_SEC) >= seconds){
            printf("\t\tTimer: %1.1f seconds... nothing happened\n", seconds);
            seconds += refresh_rate;
        }
        if (seconds > 60){
            break;
        }
    }
    printf("\tGame Over!\n");
    return 0;
}
