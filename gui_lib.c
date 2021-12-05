/* PBM765 allowed libraries */
#include <GL/gl.h>
#include <math.h>

double* game_state_memory;

int grid_x, grid_y;
double ball_cx = 20.0;
double ball_cy = 25.0;
double r_paddle_cy = 15.0;
double l_paddle_cy = 25.0;
double vx = 0.5;
double vy = 0.5;

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
    double border_height = 1.0;
    for (x = 0; x < grid_x; x++){
        for (y = 0; y < grid_y; y++){
            unit(x, y);
        }
    }
    glColor3f(1, 0, 0.627);
    /* upper border */
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0, grid_y - border_height);
        glVertex2f(grid_x, grid_y - border_height);
        glVertex2f(grid_x, grid_y);
        glVertex2f(0, grid_y);
    glEnd();
    /* lower border */
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0, 0);
        glVertex2f(grid_x, 0);
        glVertex2f(grid_x, border_height);
        glVertex2f(0, border_height);
    glEnd();
}

void drawPaddle(char side){
    double paddle_width = 1.0;
    double paddle_height = 6.0;
    double cx, cy;
    if (side == 'L'){
        cx = 4.5;
        cy = l_paddle_cy;
    } else if (side =='R'){
        cx = grid_x - 4.5;
        cy = r_paddle_cy;
    }
    glLineWidth(1.0);
    glColor3f(0.996, 0.458, 0.996);
    glRectd(cx - paddle_width/2, cy - paddle_height/2, cx + paddle_width/2, cy + paddle_height/2);
}

void drawBall(double cx, double cy){
    int i;
    int poligons = 69;
    double radius = 1.0;
    double doublePi = 2.0 * 3.142;
    ball_cx += vx;
    ball_cy += vy;
    if (ball_cx > 80) {ball_cx = 0;}
    if (ball_cy > 40) {ball_cy = 0;}
    glColor3f(1, 0.070, 0.309);
    glBegin(GL_TRIANGLE_FAN);
        /* From the center of the circle, and around */
        glVertex2f(ball_cx, ball_cy); /*  center of circle */
        for (i = 0; i <= poligons; i++)   {
            glVertex2f (
                    (ball_cx + (radius * cos(i * doublePi / 20))), (ball_cy + (radius * sin(i * doublePi / 20)))
            );
        }
    glEnd();
}

void drawBorder(float cx, float cy){
    int i;
    int poligons = 69;
    double radius = 1.0;
    double doublePi = 2.0 * 3.142;
    glColor3f(1, 0.070, 0.309);
    glBegin(GL_TRIANGLE_FAN);
    /* From the center of the circle, and around */
    glVertex2f(cx, cy); /*  center of circle */
    for (i = 0; i <= poligons; i++)   {
        glVertex2f ((cx + (radius * cos(i * doublePi / 20))), (cy + (radius * sin(i * doublePi / 20))));
    }
    glEnd();
}

void drawGame(){
    drawGrid();
    drawBall(game_state_memory[4], game_state_memory[5]);
    drawPaddle('L');
    drawPaddle('R');
}





