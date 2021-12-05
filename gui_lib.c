/* PBM765 allowed libraries */
#include <GL/gl.h>
#include <math.h>

int grid_x, grid_y;

void initGrid(int x, int y){
    grid_x = x;
    grid_y = y;
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

void drawPaddle(double cy, char side){
    double paddle_width = 1.0;
    double paddle_height = 6.0;
    double cx;
    if (side == 'L'){
        cx = 4.5;
    } else if (side =='R'){
        cx = grid_x - 4.5;
    }
    glLineWidth(1.0);
    glColor3f(0.996, 0.458, 0.996);
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx - paddle_width/2, cy - paddle_height/2);
        glVertex2f(cx + paddle_width/2, cy - paddle_height/2);
        glVertex2f(cx + paddle_width/2, cy + paddle_height/2);
        glVertex2f(cx - paddle_width/2, cy + paddle_height/2);
    glEnd();
}

void drawBall(float cx, float cy){
    int i;
    int poligons = 69;
    double radius = 1.0;
    double doublePi = 2.0 * 3.142;
    glColor3f(1, 0.070, 0.309);
    glBegin(GL_TRIANGLE_FAN);
        /* From the center of the circle, and around */
        glVertex2f(cx, cy); /*  center of circle */
        for (i = 0; i <= poligons; i++)   {
            glVertex2f (
                    (cx + (radius * cos(i * doublePi / 20))), (cy + (radius * sin(i * doublePi / 20)))
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
        glVertex2f (
                (cx + (radius * cos(i * doublePi / 20))), (cy + (radius * sin(i * doublePi / 20)))
        );
    }
    glEnd();
}







