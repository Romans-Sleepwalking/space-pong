/* Custom space-pong library's header file */
#include "pong_lib.h"
/* PBM765 allowed libraries */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <stdlib.h>
#include <GL/glut.h>

/* Default connection parameters */
#define DEFAULT_HOSTNAME "localhost"
#define DEFAULT_PORT 6900
/* GUI window parameters */
#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 800
/* Drawing frames per second */
#define FPS 60

/* Predefined example packets for tests (integers in BIG endian (Network byte order)) */
/* P1  JOIN        DIV     NPK      P  SIZE      DATA                                             CS   DIV      */
char pack1[34] = {'-','-', 0,0,0,0, 1, 0,0,0,20, 'n',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 26, '-','-'};
/* P3  MESSAGE     DIV      NPK      P  SIZE      DATA                                             CS   DIV      */
char pack3[272] = {'-','-', 0,0,0,1, 3, 0,0,1,2, /* TA  S  MESSAGE */
                                                   -1, 0, 'T','h','i','s',' ','i','s',' ','m','e','s','s','a','g','e','!',0,0,0,0,0,0,0,0,0,
                                                          0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                                          0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                                          0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                                          0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                                          0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                                          0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                                          0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                                          0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,         /*  CS   DIV       */
                                                          0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 136, '-','-'};
/* P6  PLAYER READY  DIV     NPK      P  SIZE      DATA  CS   DIV      */
char pack6[15] =   {'-','-', 0,0,0,2, 6, 0,0,0,1,  1,    4, '-','-'};
/* P8  INPUT       DIV     NPK      P  SIZE      DATA  CS   DIV      */
char pack8[15] = {'-','-', 0,0,0,3, 8, 0,0,0,1,  4,    14, '-','-'};
/* P9  STATUS(Optional)      DIV     NPK      P  SIZE      DATA  CS   DIV      
char pack9[14] = {'-','-', 0,0,0,4, 9, 0,0,0,0,        13, '-','-'};*/
/* Mutable variables required for drawing */
char* l_team_name = "Left Player";
char* r_team_name = "Right Player";
int l_team_score = 0;
int r_team_score = 0;
int grid_columns = 80;
int grid_rows = 40;
double paddle_width = 2.0;
double paddle_height = 6.0;
double ball_r = 1.0;
double ball_cx = 40.0;
double ball_cy = 20.0;
double ball_vx = 0.2;
double ball_vy = 0.15;
double paddle_speed = 1.5;
double r_paddle_cx = 74.5;
double r_paddle_cy = 25.0;
double l_paddle_cx = 4.5;
double l_paddle_cy = 15.0;

char* createPackage1(int id, char* data_segment, int package_number);
char* createPackage3(char* data_segment, int package_number);
char* createPackage6(char client_id, int package_number);
char* createPackage8(char key_press, int package_number);


/* Initializes grid and memory */
void direct_copy_data_as_bytes(void* packet, void* data, int size){
  int i;
  char* p = packet;
  char* d = data;
  for(i=0; i<size;i++){
    p[i] = d[i];
  }
}
void initGUI(){
    glClearColor(0.070, 0.015, 0.345, 1);
    initGrid(l_team_name, r_team_name, &l_team_score, &r_team_score, &grid_columns, &grid_rows,
             &paddle_width, &paddle_height, &ball_r, &ball_cx, &ball_cy, &ball_vx, &ball_vy,
             &r_paddle_cx, &r_paddle_cy, &l_paddle_cx, &l_paddle_cy);
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

void wasd_callback(unsigned char key_pressed, int a, int b){
    /* Left player use WASD: ASCII integers */
    switch (key_pressed) {
        case 'w':  /* up */
            move_paddle('U', &l_paddle_cy, &paddle_speed);
            break;
        case 's':  /* down */
            move_paddle('D', &l_paddle_cy, &paddle_speed);
            break;
        case 'a':  /* left */
            move_paddle('L', &l_paddle_cx, &paddle_speed);
            break;
        case 'd':  /* right */
            move_paddle('R', &l_paddle_cx, &paddle_speed);
            break;
        case 27: /* ESC = exit */
            exit (1);
    }
}

void arrowkeys_callback(int key_pressed, int a, int b){
    /* Right player use arrowkeys: special OpenGL symbols */
    switch (key_pressed) {
        case GLUT_KEY_UP:  /* up */
            move_paddle('U', &r_paddle_cy, &paddle_speed);
            break;
        case GLUT_KEY_DOWN:  /* down */
            move_paddle('D', &r_paddle_cy, &paddle_speed);
            break;
        case GLUT_KEY_LEFT:  /* left */
            move_paddle('L', &r_paddle_cx, &paddle_speed);
            break;
        case GLUT_KEY_RIGHT:  /* right */
            move_paddle('R', &r_paddle_cx, &paddle_speed);
            break;
    }
}


/* ------------------------------------------------------------------------------------------------------*/
int main(int argc, char** argv){
    /*example package 1 */
    char* package1 = createPackage1(1, "Alohaaaaa", 0 );
    char* package3 = createPackage3("TThis is the message", 1 );
    char* package6 = createPackage6(1, 2);
    char* package8 = createPackage8(4, 3);
           /*  printf("Package1: %s\n", package); */
               print_Bytes(package6, 15);
                print_Bytes(package8, 15);
    int i;
    /* Assigns default values to client connection info */
    char* client_address = DEFAULT_HOSTNAME;
    int client_port = DEFAULT_PORT;
    /* Socket */
    int client_socket = 0;
    /* Process forking stat
    us */
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


    /* ========== "SHARED MEMORY" ========== */

    /* TODO */

    /* ========== GUI TEST LAUNCH ========== */

    is_parent_proc = fork();
    if (!is_parent_proc){
        glutInit(&argc, argv);
            /* Main window settings */
            glutInitWindowSize(WINDOW_WIDTH,WINDOW_HEIGHT);
            glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
            glutCreateWindow("Space Pong");
            glutDisplayFunc(display_callback);
            glutReshapeFunc(reshape_callback);
            glutKeyboardFunc(wasd_callback);
            glutSpecialFunc(arrowkeys_callback);
            initGUI();
            glutTimerFunc(0, timer_callback, 0);
        glutMainLoop();
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
                 /*  
                 
                   sleep(1); 
                       send(client_socket, pack9, 14, 0);*/
               
                     while (1) {
                     send(client_socket, package1, 34, 0);
                     sleep(1);
                       send(client_socket, package3, 272, 0);
                       sleep(1);
                     send(client_socket, package6, 15, 0);
                     sleep(1);
                     send(client_socket, package8, 15, 0);
                     sleep(1);
                 /*   scanf("%s", inputs);
                    send(client_socket, inputs, strlen(inputs), 0);
                    sleep(1); */
                }
            }
            else {

        
                while (1) {
                    scanf("%s", inputs);
                    send(client_socket, inputs, strlen(inputs), 0);
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


char* createPackage1(int pack_id, char* data_segment, int package_number){
     unsigned int number2 = htonl(package_number);
     char packageNumberString[4];
     char packageSizeString[4];
     char packageIdString;
     unsigned char checksum = 0;
     memcpy(packageNumberString, &number2, 4);
     int pos = 0;
     int size;
     /*char pack1[34] = {'-','-', 0,0,0,0, 1, 0,0,0,20, 'n','i','c','k',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 26, '-','-'};   */
     /* package 1 */
     char* package =NULL;
            if(pack_id == 1)
            {   
        
             package = pack1;
               int i = 2;
               int posNum = i+4;
                 /* Adding package number */
               for(i = 2; i<posNum; i++){
                   package[i] = packageNumberString[i-2];
               }
                /* Adding nickname to package */
               i= 11;
               while(data_segment[i-11] != '\0'){
                 package[i] = data_segment[i-11];
                 i++;
               }
               /* Adding 0 in the remaining data segment */
               for(i; i<31; i++){
                   package[i] = 0;
               }

              /* Calculating and adding checksum */
               package[i] = 0;
              checksum = (unsigned char) calculate_checksum(package+2, 28);
              package[i] = checksum;
             
           }
           if(pack_id == 3){
              package = pack3;
               int i = 2;
               int posNum = i+4;
                 /* Adding package number */
               for(i = 2; i<posNum; i++){
                   package[i] = packageNumberString[i-2];
               }  
                /* Adding message to package */
               i= 13;
               while(data_segment[i-13] != '\0'){
                 package[i] = data_segment[i-13];
                 i++;
               }
               /* Adding 0 in the remaining data segment */
               for(i; i<31; i++){
                   package[i] = 0;
               }

              /* Calculating and adding checksum */
               package[i] = 0;
              checksum = (unsigned char) calculate_checksum(package+2, 28);
              package[i] = checksum;

           }
     return package;
}
char* createPackage3(char* data_segment, int package_number){
     unsigned int number2 = htonl(package_number);
     char packageNumberString[4];
     char packageSizeString[4];
     char packageIdString;
     unsigned char checksum = 0;
     int pack_id = 3;
     memcpy(packageNumberString, &number2, 4);
     int pos = 0;
     int size;
     /*char pack1[34] = {'-','-', 0,0,0,0, 1, 0,0,0,20, 'n','i','c','k',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 26, '-','-'};   */
     /* package 1 */
     char* package =NULL;    
              package = pack3;
               int i = 2;
               int posNum = i+4;
                 /* Adding package number */
               for(i = 2; i<posNum; i++){
                   package[i] = packageNumberString[i-2];
               }  
                /* Adding message to package */
               i= 13;
               while(data_segment[i-13] != '\0'){
                 package[i] = data_segment[i-13];
                 i++;
               }
               /* Adding 0 in the remaining data segment */
               for(i; i<270; i++){
                   package[i] = 0;
               }

              /* Calculating and adding checksum */
               package[i] = 0;
              checksum = (unsigned char) calculate_checksum(package+2, 266);
              package[i] = checksum;

     return package;
}
char* createPackage6(char client_id, int package_number){
     unsigned int number2 = htonl(package_number);
     char packageNumberString[4];
     unsigned char checksum = 0;
     memcpy(packageNumberString, &number2, 4);
     int pos = 0;
     /*char pack1[34] = {'-','-', 0,0,0,0, 1, 0,0,0,20, 'n','i','c','k',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 26, '-','-'};   */
     /* package 1 */
     char* package =NULL;    
              package = pack6;
               int i = 2;
               int posNum = i+4;
                 /* Adding package number */
               for(i = 2; i<posNum; i++){
                   package[i] = packageNumberString[i-2];
               }  
                
              package[11] = client_id;

              /* Calculating and adding checksum */
              checksum = (unsigned char) calculate_checksum(package+2, 10);
              package[12] = checksum;

     return package;
}
char* createPackage8(char key_press, int package_number){
     unsigned int number2 = htonl(package_number);
     char packageNumberString[4];
     unsigned char checksum = 0;
     memcpy(packageNumberString, &number2, 4);
     int pos = 0;
     /*char pack1[34] = {'-','-', 0,0,0,0, 1, 0,0,0,20, 'n','i','c','k',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 26, '-','-'};   */
     /* package 1 */
     char* package =NULL;    
              package = pack8;
               int i = 2;
               int posNum = i+4;
                 /* Adding package number */
               for(i = 2; i<posNum; i++){
                   package[i] = packageNumberString[i-2];
               }  
                
              package[11] = key_press;

              /* Calculating and adding checksum */
              checksum = (unsigned char) calculate_checksum(package+2, 10);
              package[12] = checksum;

     return package;
}
