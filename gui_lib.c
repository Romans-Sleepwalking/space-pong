/* Custom space-pong library's header file */
#include "pong_lib.h"
/* PBM765 allowed libraries */
#include <GL/gl.h>
#include <math.h>


/* Drawing quality */
#define CIRCLE_POLIGONS 69
/* Served game parameters pointed to main function processes */
char* l_team_name_ptr;
char* r_team_name_ptr;
int* l_team_score_ptr;
int* r_team_score_ptr;
int* grid_columns_ptr;
int* grid_rows_ptr;
double* paddle_width_ptr;
double* paddle_height_ptr;
double* ball_r_ptr;
double* ball_cx_ptr;
double* ball_cy_ptr;
double* ball_vx_ptr;
double* ball_vy_ptr;
double* r_paddle_cx_ptr;
double* r_paddle_cy_ptr;
double* l_paddle_cx_ptr;
double* l_paddle_cy_ptr;
/* For calculations */
double DOUBLE_PI = 2.0 * 3.142;


/* Initializes grid and pointers */
void initGrid(char* p_l_team_name, char* p_r_team_name, int* p_l_team_score, int* p_r_team_score,
              int* p_grid_columns, int* p_grid_rows, double* p_paddle_width, double* p_paddle_height,
              double* p_ball_r, double* p_ball_cx, double* p_ball_cy,  double* p_ball_vx, double* p_ball_vy,
              double* p_r_paddle_cx, double* p_r_paddle_cy, double* p_l_paddle_cx, double* p_l_paddle_cy){
    l_team_name_ptr = p_l_team_name;
    r_team_name_ptr = p_r_team_name;
    l_team_score_ptr = p_l_team_score;
    r_team_score_ptr = p_r_team_score;
    grid_columns_ptr = p_grid_columns;
    grid_rows_ptr = p_grid_rows;
    paddle_width_ptr = p_paddle_width;
    paddle_height_ptr = p_paddle_height;
    ball_r_ptr = p_ball_r;
    ball_cx_ptr = p_ball_cx;
    ball_cy_ptr = p_ball_cy;
    ball_vx_ptr = p_ball_vx;
    ball_vy_ptr = p_ball_vy;
    r_paddle_cx_ptr = p_r_paddle_cx;
    r_paddle_cy_ptr = p_r_paddle_cy;
    l_paddle_cx_ptr = p_l_paddle_cx;
    l_paddle_cy_ptr = p_l_paddle_cy;
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
    for (x = 0; x < *grid_columns_ptr; x++){
        for (y = 0; y < *grid_rows_ptr; y++){
            unit(x, y);
        }
    }
    glColor3f(1, 0, 0.627);
    /* Draws upper border */
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0, *grid_rows_ptr - *ball_r_ptr);
        glVertex2f(*grid_columns_ptr, *grid_rows_ptr - *ball_r_ptr);
        glVertex2f(*grid_columns_ptr, *grid_rows_ptr);
        glVertex2f(0, *grid_rows_ptr);
    glEnd();
    /* Draws lower border */
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0, 0);
        glVertex2f(*grid_columns_ptr, 0);
        glVertex2f(*grid_columns_ptr, *ball_r_ptr);
        glVertex2f(0, *ball_r_ptr);
    glEnd();
}

void drawText() {
    /* TODO */
}

void drawPaddles(){
    glLineWidth(1.0);
    glColor3f(0.996, 0.458, 0.996);
    /* Right paddle */
    glRectd(*r_paddle_cx_ptr - *paddle_width_ptr/2, *r_paddle_cy_ptr - *paddle_height_ptr/2,
            *r_paddle_cx_ptr + *paddle_width_ptr/2, *r_paddle_cy_ptr + *paddle_height_ptr/2);
    /* Left paddle */
    glRectd(*l_paddle_cx_ptr - *paddle_width_ptr/2, *l_paddle_cy_ptr - *paddle_height_ptr/2,
            *l_paddle_cx_ptr + *paddle_width_ptr/2, *l_paddle_cy_ptr + *paddle_height_ptr/2);
}

void drawBall(){
    int i;
    /* Makes a ball move */
    move_ball(ball_cx_ptr, ball_cy_ptr, ball_vx_ptr, ball_vy_ptr, l_team_score_ptr, r_team_score_ptr,
              r_paddle_cx_ptr, r_paddle_cy_ptr, l_paddle_cx_ptr, l_paddle_cy_ptr);
    /* Draws a ball moved */
    glColor3f(1, 0.070, 0.309);
    glBegin(GL_TRIANGLE_FAN);
        /* From the center of the circle, and around */
        glVertex2f(*ball_cx_ptr, *ball_cy_ptr); /*  center of circle */
        for (i = 0; i <= CIRCLE_POLIGONS; i++){
            glVertex2f ((*ball_cx_ptr + (*ball_r_ptr * cos(i * DOUBLE_PI / 20))),
                        (*ball_cy_ptr + (*ball_r_ptr * sin(i * DOUBLE_PI / 20))));
        }
    glEnd();
}

void drawBorders(){
    glColor3f(1, 0, 0.627);
    glRectd(0, 0 , *grid_columns_ptr, *ball_r_ptr);
    glRectd(0, *grid_rows_ptr - *ball_r_ptr, *grid_columns_ptr, *grid_rows_ptr);
    glEnd();
}

void drawGame(){
    drawGrid();
    drawBorders();
    drawBall();
    drawPaddles();
    drawText();
}
