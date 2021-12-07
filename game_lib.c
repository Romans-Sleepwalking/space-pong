/* PBM765 allowed library */
#include<stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/mman.h>
#include <math.h>

/* Grid parameters */
#define GRID_COLUMNS 80
#define GRID_ROWS 40
/* Default teams' score parameters */
#define DEFAULT_SCORE 0.0
#define MAX_SCORE 69
/* Default game mode */
#define GAME_MODE 1.0
#define DEFAULT_READY_PLAYERS_COUNT 0.0
/* Default ball parameters */
#define BALL_R 1.0
#define DEFAULT_BALL_CX 40.0
#define DEFAULT_BALL_CY 20.0
#define DEFAULT_BALL_VX 0.5
#define DEFAULT_BALL_VY 0.5
/* Default paddles' parameters */
#define PADDLE_WIDTH 1.0
#define PADDLE_HEIGHT 6.0
#define DEFAULT_L_PADDLE_CX 5.0
#define DEFAULT_L_PADDLE_CY 15.0
#define DEFAULT_R_PADDLE_CX 75.0
#define DEFAULT_R_PADDLE_CY 25.0
/* Calculation refresh rate */
#define REFRESH_RATE_SECONDS 0.2

/* For collision calculations */
double BALL_TANG = sqrt(2 * pow(BALL_R, 2.0));
double PADDLE_TRIG_LEN_X = PADDLE_WIDTH/2 + BALL_R;
double PADDLE_TRIG_LEN_Y = PADDLE_HEIGHT/2 + BALL_R;

/* Equivalent to the main function for the game session */
int launch_game(int* game_state_memory_ptr){
    /* Timings */
    float seconds = 5;
    float refresh_rate = REFRESH_RATE_SECONDS;
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
void move_ball(double* ballCX, double* ballCY, double* ballVX, double* ballVY, double* lScore, double* rScore,
               double* rPaddleCX, double* rPaddleCY, double* lPaddleCX, double* lPaddleCY){
    /* Changes ball coordinate according to speed */
    *ballCX = (*ballCX + *ballVX);
    *ballCY = (*ballCY + *ballVY);

    /* ? HORIZONTAL UPPER/LOWER BORDER COLLISION */
    if ((*ballCY <= 2 * BALL_R) || (*ballCY >= GRID_ROWS - 2 * BALL_R)){
        /* Reflects the ball to the vertical direction: flips y speed component */
        *ballVY = -(*ballVY);
    }

    /* ? GOAL AT LEFT */
    else if (*ballCX <= 0){
        /* Left border collision -> right team scores */
        *rScore = (*rScore + 1);
        printf("TEAM RIGHT SCORES!");
        /* Checks for game over */
        if (*rScore >= MAX_SCORE){
            /* TODO: Game Over Stuff */
            printf("TEAM RIGHT WON!");
        }
        *ballCX = GRID_COLUMNS - BALL_R;
    }

    /* ? GOAL AT RIGHT */
    else if (*ballCX >= GRID_COLUMNS){
        /* Right border collision -> left team scores */
        *lScore = (*lScore + 1);
        printf("TEAM LEFT SCORES!");
        /* Checks for game over */
        if (*lScore >= MAX_SCORE){
            /* TODO: Game Over Stuff */
            printf("TEAM LEFT WON!");
        }
        *ballCX = BALL_R;
    }

    /* ? LEFT PADDLE COLLISION */
    else if (
            (*lPaddleCX - PADDLE_TRIG_LEN_X <= *ballCX) &&
            (*ballCX <= *lPaddleCX + PADDLE_TRIG_LEN_X) &&
            (*lPaddleCY - PADDLE_TRIG_LEN_Y <= *ballCY) &&
            (*ballCY <= *lPaddleCY + PADDLE_TRIG_LEN_Y)
    ){
        /* HORIZONTAL COLLISION */
        if (fabs(*ballCX - *lPaddleCX) > fabs(*ballCY - *lPaddleCY)){
            /* Reflects the ball to the horizontal direction: flips x speed component */
            *ballVX = -(*ballVX);
        }
        /* VERTICAL COLLISION */
        else if (fabs(*ballCX - *lPaddleCX) < fabs(*ballCY - *lPaddleCY)){
            /* Reflects the ball to the vertical direction: flips y speed component */
            *ballVY = -(*ballVY);
        }
        /* ANGULAR COLLISION */
        else if (fabs(*ballCX - *lPaddleCX) == fabs(*ballCY - *lPaddleCY)){
            /* Reflects the ball to the opposite direction: flips speed components */
            *ballVX = -(*ballVX);
            *ballVY = -(*ballVY);
        }
    }

    /* ? RIGHT PADDLE COLLISION */
    else if (
            (*rPaddleCX - PADDLE_TRIG_LEN_X <= *ballCX) &&
            (*ballCX <= *rPaddleCX + PADDLE_TRIG_LEN_X) &&
            (*rPaddleCY - PADDLE_TRIG_LEN_Y <= *ballCY) &&
            (*ballCY <= *rPaddleCY + PADDLE_TRIG_LEN_Y)
    ){
        /* HORIZONTAL COLLISION */
        if (fabs(*ballCX - *rPaddleCX) > fabs(*ballCY - *rPaddleCY)){
            /* Reflects the ball to the horizontal direction: flips x speed component */
            *ballVX = -(*ballVX);
        }
            /* VERTICAL COLLISION */
        else if (fabs(*ballCX - *rPaddleCX) < fabs(*ballCY - *rPaddleCY)){
            /* Reflects the ball to the vertical direction: flips y speed component */
            *ballVY = -(*ballVY);
        }
            /* ANGULAR COLLISION */
        else if (fabs(*ballCX - *rPaddleCX) == fabs(*ballCY - *rPaddleCY)){
            /* Reflects the ball to the opposite direction: flips speed components */
            *ballVX = -(*ballVX);
            *ballVY = -(*ballVY);
        }
    }
}


