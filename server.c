/* Custom space-pong library's header file */
#include "pong_lib.h"
/* PBM765 allowed libraries */
#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include <netdb.h>
/* Global default constants */
#define DEFAULT_MAX_CLIENTS 4
#define DEFAULT_HOSTNAME "127.0.0.1"
#define DEFAULT_PORT 12369
#define DEFAULT_SHARED_MEMORY_SIZE 1024
/* Global variables */
char* shared_memory = NULL;
int* client_count = NULL;
int* shared_data = NULL;
int* total_bytes_used = 0;

/* Gets shared memory */
void get_shared_memory(){
    int single_client_memory;
    /* ... */
    shared_memory = mmap(NULL, DEFAULT_SHARED_MEMORY_SIZE,PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);
    /* ... */
    client_count = (int*) shared_memory;
    /* ... */
    single_client_memory = DEFAULT_SHARED_MEMORY_SIZE / 4;
    /* ... */
    total_bytes_used = (int*)(shared_memory +sizeof(int));
    shared_data = (int*)(shared_memory +sizeof(int) + sizeof(int));
    /* shared_data = (int*)(shared_memory +sizeof(int)); */
}

/* Sends and process client's packets */
void process_client(int id,int socket){
    int i;
    /* ... */
    char in[1];
    char out[100];
    /* ... */
    int k = 0;

    printf("Processing client id=%d, socket=%d\n", id, socket);
    printf("CLIENT count %d\n", *client_count);
    /* ... */
    while(1){
        read(socket,in,1);
        /*    if(in[0]>-1 && in[0]<126 && in[0] != 10 && in[0] != 3){*/
        if ((in[0]>47 && in[0]<58) || (in[0]>64 && in[0]<126) || in[0] == 0 || in[0] == 10){
            /* ... */
            shared_data[DEFAULT_MAX_CLIENTS + id] += 1;
            /* ... */
            if (in[0] == 10){
                printf("Character received:  New line\n");
            } else {
                printf("Character received: %c \n", in[0]);
            }
            /* ... */
            for (i = 0; i < shared_data[DEFAULT_MAX_CLIENTS + id]; i++){
                sprintf(out,"%c",in[0]);
                write(socket,out,1);
            }
            /* ... */
            k = (int)in[0]-48;
            shared_data[id] = k;
            /*sprintf(out,"CLIENT %d Total bytes used by client: %d\n",id,shared_data[DEFAULT_MAX_CLIENTS+id]);*/
            printf("CLIENT %d total bytes used %d\n", id, shared_data[DEFAULT_MAX_CLIENTS + id]);
        }
    }
}

/* Starts network which polls new socket connections */
int start_polling(char* hostname, int port){
    /* Server info */
    int server_socket;
    struct sockaddr_in server_address;
    /* For every new connection client's info  */
    int client_socket;
    struct sockaddr_in client_address;
    int client_address_size = sizeof(client_address);
    int new_client_id;
    /* For every new connection process forking */
    int is_parent_proc;
    int is_child_proc;

    /* ========== CREATES SOCKET OBJECT ========== */


    /* Creates a new socket */
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket >= 0){
        printf("\tServer socket created... ");
    }
    /* If creation failed, throws an error  */
    else {
        printf("Error: server socket creation failed!\n");
        return -1;
    }

    /* Assigns remote socket values */
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port);

    /* Binds the socket to accept incoming connections */
    if (bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address)) >= 0){
        printf("Binded... ");
    }
    /* If binding failed, throws an error  */
    else {
        printf("Error: server socket binding failed!\n");
        return -1;
    }

    /* ========== STARTS SOCKET LISTENING ========== */

    /* Sets the socket to listen for incoming connections */
    if ((listen(server_socket, DEFAULT_MAX_CLIENTS)) >= 0){
        printf("Is listening!\n");
    }
    /* If binding failed, throws an error  */
    else {
        printf("Error: server socket setting to listening failed!\n");
        return -1;
    }
    /* Infinitely loops server to listen and react to new clients */
    while(1) {
        /* Refreshes parameters */
        client_socket = 0;
        new_client_id = 0;
        is_parent_proc = 0;
        is_child_proc = 0;
        /* Accepts a new client connection */
        if ((client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_size)) >= 0){
            /* Saves the client's info */
            new_client_id = *client_count;
            /* Records stats */
            *client_count += 1;
            /* Forks child process */
            is_parent_proc = fork();
            /* Child process closes the client's socket */
            if (!is_parent_proc){
                close(client_socket);
            }
            /* Parent process... */
            else {
                /* Closes server socket */
                close(server_socket);
                /* Forks grandchild process */
                is_child_proc = fork();
                /* TODO: what does the grandchild process? */
                if (!is_child_proc){
                    wait(NULL);
                    printf("Succesfully orphaned client %d\n", new_client_id);
                    exit(0);
                }
                /* Child process calls func process_client()  */
                else {
                    process_client(new_client_id,client_socket);
                    exit(0);
                }
            }
        }
        /* If error during accepting new client connection occured, throws an error and continues */
        else {
            printf("Error: accepting client connection failed! ERRNO=%d\n",errno);
            continue;
        }
    }
}


int main(int argc, char** argv){
    int i;
    /* Assigns default values to server connection info */
    char* server_hostname = DEFAULT_HOSTNAME;
    int server_port = DEFAULT_PORT;
    /* Process forking status */
    int is_parent_proc = 0;

    /* ========== READS SERVER'S CONNECTION INFO ========== */

    printf("\tReading connection parameters...");
    for (i=0; i<argc; i++){
        /* Reads hostname */
        if (argv[i][0] == '-' && argv[i][1] == 'h' && argv[i][2] == '='){
            /* Calls parameter reading function from library */
            server_hostname = read_parameter_value(argv[i]);
        }
            /* Reads port */
        else if (argv[i][0] == '-' && argv[i][1] == 'p' && argv[i][2] == '='){
            /* Translates the port string to integer */
            server_port = atoi(read_parameter_value(argv[i]));
        }
    }
    printf("hostname=\"%s\"; port=%d\n", server_hostname, server_port);

    /* ========== ALLOCATES MEMORY FOR SERVER'S NEEDS ========== */

    int requested_mem_size = DEFAULT_SHARED_MEMORY_SIZE;
    /*ja vajadzēs vairāk atmiņu packetos vai gamestate tad varēs palielināt memory size no 1024 */
    int* memory_block = malloc(requested_mem_size * sizeof(int));
    /*pagaidam game state buus 512 baitus liels memory */
    int* game_state_partition = (int*)(memory_block +sizeof(int)*requested_mem_size/2);

    /* memory block struktūra:
    first_client input(128 baiti) ->  second_client input(128 baiti) -> third_client input(128 baiti) -> fourth_client input(128 baiti) -> gamestate_client input(512 baiti)
     */
    /* pagaidam katram clientam bus 128 baiti */
    int* first_client_input = (int*)memory_block;
    int* second_client_input = (int*)(first_client_input +sizeof(int)*requested_mem_size/8);
    int* third_client_input = (int*)(second_client_input +sizeof(int)*requested_mem_size/8);
    int* fourth_client_input = (int*)(third_client_input +sizeof(int)*requested_mem_size/8);

    /* Katram clientam inputam pirmais baits noradis 0 vai 1 , 0 nozime ka packetā var rakstīt, 1 nozīmēs ka packetā nevar rakstīt, jo to sākumā ir jānolasa gameupdeitotajam */
    *first_client_input = 0;
    *second_client_input = 0;
    *third_client_input = 0;
    *fourth_client_input = 0;
    /* talak lai atrastu packeta info vajadzes first_client_packet = (*int)(first_client +sizeof(int) )
    vai ari vares izmantot client id un tad vares accessot katru:  client_packet = (*int)(memory_block +sizeof(int)*requested_mem_size/8*id)
    */
    get_shared_memory();

    /* ========== RUNS THE SERVER ========== */

    printf("\tRunning space-pong server...\n");

    /* Forks child process */
    is_parent_proc = fork();
    /* Parent process starts the game */
    if (is_parent_proc){
        launch_game(game_state_partition);
    } else {
        /* Child process calls the start server polling function */
        start_polling(server_hostname, server_port);
    }
    printf("\tBytes received: %d\n", *total_bytes_used);
    return 0;
}
