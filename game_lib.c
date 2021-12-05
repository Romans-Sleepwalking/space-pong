/* PBM765 allowed library */
#include<stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/mman.h>

/* Default player's score */
#define DEFAULT_PLAYER_SCORE 0.0
/* Default game mode */
#define DEFAULT_GAME_MODE 1.0
#define DEFAULT_READY_PLAYER_COUNT 0.0
/* Default ball parameters */
#define DEFAULT_CX_BALL 40.0
#define DEFAULT_CY_BALL 20.0
#define DEFAULT_VX_BALL 0.5
#define DEFAULT_VY_BALL 0.5
/* Default paddles' coordinates */
#define DEFAULT_CX_L_PADDLE 5.0
#define DEFAULT_CY_L_PADDLE 15.0
#define DEFAULT_CX_R_PADDLE 75.0
#define DEFAULT_CY_R_PADDLE 25.0
/* Calculation refresh rate */
#define DEFAULT_REFRESH_RATE_SECONDS 0.2

/* Equivalent to the main function for the game session */
int launch_game(int* game_state_memory_ptr){
    /* Timings */
    float seconds = 5;
    float refresh_rate = DEFAULT_REFRESH_RATE_SECONDS;
    clock_t timer;
    printf("\tPreparing the game with %1.1f seconds refresh rate... ", refresh_rate);

    /* ========== LOADING GAME MEMORY ========== */

    int* score_team_1 = game_state_memory_ptr;
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

    printf("Memory reserved... ");

    /* ========== LOOPS THE GAME PROCESS ========== */

    /* Starts games infinite loop */
    printf("Game Launched!\n");
    timer = clock();
    while (1){
        if ((((double)clock()-timer)/CLOCKS_PER_SEC) >= seconds){
            printf("\t\tTimer: %1.1f seconds... nothing happened\n", seconds);
            seconds += refresh_rate;
        }
        if (seconds > 2500){
            break;
        }
    }
    printf("\tGame Over!\n");
    return 0;
}



/* Equivalent to the main function for the game session */
void local_test_game(double* game_state_memory_ptr){
    float wait_timer = 0;
    float game_timer = 0;

    printf("\tPreparing the game with %1.1f seconds refresh rate... ", DEFAULT_REFRESH_RATE_SECONDS);

    /* ========== LOADING GAME MEMORY ========== */

    /* P1, P2 scores */
    game_state_memory_ptr[0] = DEFAULT_PLAYER_SCORE;
    game_state_memory_ptr[1] = DEFAULT_PLAYER_SCORE;
    /* Game mode: 1 (1v1) or 2 (2v2) */
    game_state_memory_ptr[2] = DEFAULT_GAME_MODE;
    /* Ready player count */
    game_state_memory_ptr[3] = DEFAULT_READY_PLAYER_COUNT;
    /* Ball parameters */
    double* ball_cx = (double*)(game_state_memory_ptr + 4 * sizeof(double));
    *ball_cx = DEFAULT_CX_BALL;
    game_state_memory_ptr[5] = DEFAULT_CY_BALL;
    game_state_memory_ptr[6] = DEFAULT_VX_BALL;
    game_state_memory_ptr[7] = DEFAULT_VY_BALL;
    /* Paddles' parameters */
    game_state_memory_ptr[8] = DEFAULT_CX_L_PADDLE;
    game_state_memory_ptr[9] = DEFAULT_CY_L_PADDLE;
    game_state_memory_ptr[10] = DEFAULT_CX_R_PADDLE;
    game_state_memory_ptr[11] = DEFAULT_CY_R_PADDLE;

    printf("Game memory is ready... ");

    /* Gives 5 seconds for players to prepare */
    sleep(5);
    printf("Game Launched!\n");

    /* ========== LOOPS THE GAME PROCESS ========== */

    /* Starts counting time */
    game_timer = clock();
    /* Infinite loop intil Game Over break */
    while (1){
        /* ====== BALL MOVEMENT ====== */

        /* x = x + dx... y = y + dy */
        game_state_memory_ptr[4] += game_state_memory_ptr[6];
        game_state_memory_ptr[5] += game_state_memory_ptr[7];

        if (game_state_memory_ptr[4] >= 80){
            game_state_memory_ptr[4] = 0;
        }
        if (game_state_memory_ptr[5] >= 40){
            game_state_memory_ptr[5] = 0;
        }

        if (game_timer > 2500000){
            printf("\tGame Over!\n");
            break;
        }
        sleep(DEFAULT_REFRESH_RATE_SECONDS);
    }
    return 0;
}


