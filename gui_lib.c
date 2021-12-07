/* Custom space-pong library's header file */
#include "pong_lib.h"
/* PBM765 allowed libraries */
#include <GL/gl.h>
#include <math.h>


/* Drawing quality */
#define CIRCLE_POLIGONS 69
/* Drawing parameters */
#define BALL_R 1.0
#define PADDLE_WIDTH 1.0
#define PADDLE_HEIGHT 6.0

/* TEMPORAL TESTING VALUES */
double l_team_score = 0;
double r_team_score = 0;
double ball_cx = 30.0;
double ball_cy = 25.0;
double ball_vx = 0.2;
double ball_vy = 0.4;
double r_paddle_cx = 4.5;
double r_paddle_cy = 25.0;
double l_paddle_cx = 74.5;
double l_paddle_cy = 25.0;
/* ====================== */

/* Game state for drawing */
double* game_state_memory;
/* Grid parameters for drawing */
int grid_x, grid_y;
/* For calculations */
double DOUBLE_PI = 2.0 * 3.142;



void initGrid(int x, int y, double* game_state_memory_ptr){
    grid_x = x;
    grid_y = y;
    game_state_memory = game_state_memory_ptr;
}

void unit(int x, int y){
    glLineWidth(1.0);
    glColor3f(0.478, 0.015, 0.921);
    glBegin(GL_LINE_LOOP);
        glVertex2f(x, y);
        glVertex2f(x+1, y);
        glVertex2f(x+1, y+1);
        glVertex2f(x, y+1);
    glEnd();
}

void drawGrid(){
    int x, y;
    for (x = 0; x < grid_x; x++){
        for (y = 0; y < grid_y; y++){
            unit(x, y);
        }
    }
    glColor3f(1, 0, 0.627);
    /* upper border */
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0, grid_y - BALL_R);
        glVertex2f(grid_x, grid_y - BALL_R);
        glVertex2f(grid_x, grid_y);
        glVertex2f(0, grid_y);
    glEnd();
    /* lower border */
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0, 0);
        glVertex2f(grid_x, 0);
        glVertex2f(grid_x, BALL_R);
        glVertex2f(0, BALL_R);
    glEnd();
}

void drawPaddle(char side){
    double cx, cy;
    if (side == 'L'){
        cx = r_paddle_cx;
        cy = r_paddle_cy;
    } else if (side =='R'){
        cx = l_paddle_cx;
        cy = l_paddle_cy;
    }
    glLineWidth(1.0);
    glColor3f(0.996, 0.458, 0.996);
    glRectd(cx - PADDLE_WIDTH/2, cy - PADDLE_HEIGHT/2, cx + PADDLE_WIDTH/2, cy + PADDLE_HEIGHT/2);
}

void drawBall(){
    int i;
    move_ball(&ball_cx, &ball_cy, &ball_vx, &ball_vy, &l_team_score, &r_team_score,
              &r_paddle_cx, &r_paddle_cy, &l_paddle_cx, &l_paddle_cy);
    glColor3f(1, 0.070, 0.309);
    glBegin(GL_TRIANGLE_FAN);
        /* From the center of the circle, and around */
        glVertex2f(ball_cx, ball_cy); /*  center of circle */
        for (i = 0; i <= CIRCLE_POLIGONS; i++)   {
            glVertex2f (
                    (ball_cx + (BALL_R * cos(i * DOUBLE_PI / 20))), (ball_cy + (BALL_R * sin(i * DOUBLE_PI / 20)))
            );
        }
    glEnd();
}

void drawBorders(){
    glColor3f(1, 0, 0.627);
    glRectd(0, 0 , grid_x, BALL_R);
    glRectd(0, grid_y - 1, grid_x, grid_y);
    glEnd();
}

void drawGame(){
    drawGrid();
    drawBorders();
    drawBall();
    drawPaddle('L');
    drawPaddle('R');
}
