/* Custom space-pong library's header file */
#include "pong_lib.h"
/* PBM765 allowed libraries */
#include <sys/socket.h>
#include <arpa/inet.h>
#include<stdio.h>
#include<string.h>
#include <unistd.h>
#include <netdb.h>
#include <stdlib.h>
/* Global default values */
#define DEFAULT_HOSTNAME "127.0.0.1"
#define DEFAULT_PORT 12345


int main(int argc, char** argv){
    int i;
    /* Assigns default values to client connection info */
    char* client_address = DEFAULT_HOSTNAME;
    int client_port = DEFAULT_PORT;
    /* Socket */
    int my_socket = 0;
    /* Process forking status */
    int is_child_proc = 0;
    /* Server */
    char *servername;
    struct sockaddr_in remote_address;
    char inputs[100];
    char server_reply[6000];

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

    /* Assigns remote socket values */
    remote_address.sin_family = AF_INET;
    remote_address.sin_port = htons(client_port);
    /* shi rindina lkm citadak jo ir cipari nevis localhost */
    servername = gethostbyname(client_address);
    /* Creates a new socket */
    if((my_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        printf("Success: socket created!")
    } else {
        /* If failed, throws an error  */
        printf("Error: socket failed!")
        return -1;
    }

    /* ========== CONNECTS TO THE SERVER ========== */

    /* Forks child process */
    is_child_proc = fork();
    /* Child process connects to the server and interact with it */
    if (is_child_proc) {
        inet_pton(AF_INET, servername, &remote_address.sin_addr);
        /* Connects to the server */
        if (connect(my_socket, (struct sockadrr *) &remote_address, sizeof(remote_address)) == 0) {
            printf("Connected\n");
            /* Infinite loop sends client's Terminal messages if inputted */
            while (1){
                scanf("%s", inputs);
                send(my_socket, inputs, strlen(inputs), 0);
            }
        } else {
            printf("Error connecting\n");
            return -1;
        }
    }
    /* Parent process listens for server messages */
    else {
        /* Infinite loop puts server messages if received */
        while(1){
            if (recv(my_socket, server_reply , 6000 , 0) > 0){
                puts(server_reply);
            }
        }
    }
}
