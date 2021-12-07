/* Custom space-pong library's header file */
#include "pong_lib.h"
/* PBM765 allowed libraries */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <stdlib.h>
#include <ncurses.h>
#include <GL/glut.h>

/* Default connection parameters */
#define DEFAULT_HOSTNAME "localhost"
#define DEFAULT_PORT 6900
/* Window parameters */
int window_width = 1600;
int window_height = 800;
int window_x;
int window_y;
/* Game grid */
float grid_columns = 80;
float grid_rows = 40;
/* Drawing frames per second */
#define FPS 60


/* Initializes grid and memory */
void initGUI(double* game_state_memory_ptr){
    glClearColor(0.070, 0.015, 0.345, 1);
    initGrid(grid_columns, grid_rows, game_state_memory_ptr);
}

/* Reshapes the view port after user's interactions with window size: 100x100 maintains */
void reshape_callback(int w, int h){
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, grid_columns, 0.0, grid_rows, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

/* Game frame block */
void display_callback(void){
    glClear(GL_COLOR_BUFFER_BIT);
    drawGame();
    glutSwapBuffers();
}

/* Timer-updater */
void timer_callback(int meow){
    glutPostRedisplay();
    glutTimerFunc(1000/FPS, timer_callback, 0);
}

void keyboard_callback(int w, int h){
    /* TODO */
}

/* Finds center of the monitor's screen */
void find_screen_center(){
    window_x = (glutGet (GLUT_SCREEN_WIDTH) - window_width)/2;
    window_y = (glutGet (GLUT_SCREEN_HEIGHT) - window_height)/2;
}


int main(int argc, char** argv){
    int i;
    /* Assigns default values to client connection info */
    char* client_address = DEFAULT_HOSTNAME;
    int client_port = DEFAULT_PORT;
    /* Socket */
    int client_socket = 0;
    /* Process forking status */
    int is_parent_proc = 0;
    int is_child_proc = 0;
    /* Server */
    char *server_name;
    struct sockaddr_in remote_address;
    /* Interaction */
    char inputs[100];
    int ch;
    /* Messages */
    char server_reply[6000];
    /* Game state memory block */
    double* game_state_memory_ptr = (double*)malloc(1024 * sizeof(double));


    /* ========== GUI TEST LAUNCH ========== */

    is_parent_proc = fork();
    if (!is_parent_proc){
        glutInit(&argc, argv);
            find_screen_center();
            /* Main window settings */
            glutInitWindowSize(window_width,window_height);
            glutInitWindowPosition(window_x, window_y);
            glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
            glutCreateWindow("Space Pong");
            glutDisplayFunc(display_callback);
            glutReshapeFunc(reshape_callback);
            initGUI(game_state_memory_ptr);
            glutTimerFunc(0, timer_callback, 0);
        glutMainLoop();
    }
    else {
        sleep(1000000);
    }

    printf("\tRunning space-pong client... ");



    /* ========== READS CLIENT'S CONNECTION INFO ========== */

    printf("\tReading connection parameters...");
    client_address = read_param_value(argc, argv, 'a');
    client_port = atoi(read_param_value(argc, argv, 'p'));
    printf("address=\"%s\", port=%d\n", client_address, client_port);

    /* ========== CREATES SOCKET OBJECT ========== */

    /* Creates a new socket */
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) >= 0){
        printf("\tClient socket created... ");
    } else {
        printf("Error: client socket failed!\n");
        return -1;
    }
    /* Assigns initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();remote socket values */
    /* shi rindina lkm citadak jo ir cipari nevis localhost */
    server_name = gethostbyname(client_address);
    remote_address.sin_family = AF_INET;
    remote_address.sin_port = htons(DEFAULT_PORT);

    /* ========== CONNECTS TO THE SERVER ========== */

    /* Forks child process */
    is_parent_proc = fork();
    /* Child process connects to the server and interact with it */
    if (!is_parent_proc) {
        inet_pton(AF_INET, server_name, &remote_address.sin_addr);
        /* Connects to the server */
        if (connect(client_socket, (struct sockadrr *) &remote_address, sizeof(remote_address)) == 0) {
            printf("Connected!\n");
            /* Forks grandchild process */
            is_child_proc = fork();
            /* Grandchild process connects to the server and interact with it */
            if (!is_child_proc) {

            }
            else {
                while (1) {
                    /*
                    ch = getch();
                    switch (ch) {
                        case KEY_UP:
                            printw("\nUp Arrow");
                            break;
                        case KEY_DOWN:
                            printw("\nDown Arrow");
                            break;
                        default:
                            printw("\nStop, please: %c!", ch);
                    }
                    scanf("%s", inputs);
                    send(client_socket, inputs, strlen(inputs), 0);
                     */
                    sleep(1);
                }
            }
        } else {
            printf("Error: connection failure!\n");
            return -1;
        }
    }
    /* Parent process listens for server messages */
    else {
        /* Infinite loop puts server messages if received */
        while(1){
            if (recv(client_socket, server_reply , 6000 , 0) > 0){
                puts(server_reply);
            }
        }
    }
}