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