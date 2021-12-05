/* PBM765 allowed libraries */
#include <GL/gl.h>
#include <stdlib.h>

int grid_x, grid_y;

void initGrid(int x, int y){
    grid_x = x;
    grid_y = y;
}

void unit(int x, int y){
    glLineWidth(1.0);
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(x, y);
        glVertex2f(x+1, y);
        glVertex2f(x, y+1);
        glVertex2f(x+1, y+1);
    glEnd();
}

void drawGrid(){
    unit(20,20);
}







