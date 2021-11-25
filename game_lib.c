/* PBM765 allowed library */
#include<stdio.h>

/*
 * Space-pong library segment that
 *      creates a game
 *      contains pong logic methods
 */

/* Equivalent to the main function for the game session */
void gameloop(){
    printf("Starting gameloop! Runs forever.\n");
    int seconds = 0;
    int refresh_rate = 3;
    while(1){
        printf("Game Timer: %d seconds.\n", seconds);
        seconds += 3;
        sleep(refresh_rate);
    }
}

void game_structure(){
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
}