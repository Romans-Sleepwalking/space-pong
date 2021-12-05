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

/* ... */
void initGUI(){
    glClearColor(1, 0, 0, 0);
    initGrid(grid_columns, grid_rows);
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
    drawGrid();
    glutSwapBuffers();
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
    int is_child_proc = 0;
    int is_grandchild_proc = 0;
    /* Server */
    char *server_name;
    struct sockaddr_in remote_address;
    /* Interaction */
    char inputs[100];
    int ch;
    /* Messages */
    char server_reply[6000];


    /* ========== GUI LAUNCH ========== */
    is_child_proc = fork();
    if (is_child_proc){
        glutInit(&argc, argv);
        find_screen_center();
        /* Main window settings */
        glutInitWindowSize(window_width,window_height);
        glutInitWindowPosition(window_x, window_y);
        glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
        glutCreateWindow("Space Pong");
        glutDisplayFunc(display_callback);
        glutReshapeFunc(reshape_callback);
        initGUI();
        glutMainLoop();
    }

    /* ========== READS CLIENT'S CONNECTION INFO ========== */

    printf("\tRunning space-pong client... ");
    /* Reads client connection info */
    for (i=0; i<argc; i++){
        /* Reads address */
        if (argv[i][0] == '-' && argv[i][1] == 'a' && argv[i][2] == '='){
            /* Calls parameter reading function from library */
            client_address = read_parameter_value(argv[i]);
        }
        /* Reads port */
        else if (argv[i][0] == '-' && argv[i][1] == 'p' && argv[i][2] == '='){
            /* Calls parameter reading function from library and translates the substring to integer */
            client_port = atoi(read_parameter_value(argv[i]));
        }
    }
    printf("address=\"%s\", port=%d\n", client_address, client_port);


    /* ========== CREATES SOCKET OBJECT ========== */

    /* Creates a new socket */
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) >= 0){
        printf("\tClient socket created... ");
    } else {
        /* If failed, throws an error  */
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
    is_child_proc = fork();
    /* Child process connects to the server and interact with it */
    if (is_child_proc) {
        inet_pton(AF_INET, server_name, &remote_address.sin_addr);
        /* Connects to the server */
        if (connect(client_socket, (struct sockadrr *) &remote_address, sizeof(remote_address)) == 0) {
            printf("Connected!\n");
            /* Forks grandchild process */
            is_grandchild_proc = fork();
            /* Child process connects to the server and interact with it */
            if (is_grandchild_proc) {

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