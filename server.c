/* Custom space-pong library's header file */
#include "pong_lib.h"
/* PBM765 allowed libraries */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
/* Global default values */
#define DEFAULT_MAX_CLIENTS 10
#define DEFAULT_HOSTNAME "127.0.0.1"
#define DEFAULT_PORT 12348
#define DEFAULT_SHARED_MEMORY_SIZE 1024
/* Global variables */
char* shared_memory = NULL;
int* client_count = NULL;
int* shared_data = NULL;
int* total_bytes_used = 0;

/* Gets shared memory */
void get_shared_memory(){
    shared_memory = mmap(NULL,SHARED_MEMORY_SIZE,PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);
    client_count = (int*) shared_memory;
    int single_client_memory = SHARED_MEMORY_SIZE/4;

    total_bytes_used = (int*)(shared_memory +sizeof(int));
    shared_data = (int*)(shared_memory +sizeof(int) + sizeof(int));
     /*shared_data = (int*)(shared_memory +sizeof(int)); */
}

/* Starts network */
void start_network(int port){
    int main_socket;
    struct sockaddr_in server_adress;

    int client_socket;
    struct sockaddr_in client_address;
    int client_adress_size = sizeof(client_address);

    main_socket = socket(AF_INET, SOCK_STREAM, 0);

    if(main_socket<0) {
        printf("Error opening main server socket!\n");
        exit(1);
    }

    printf("Main socket created\n");

    server_adress.sin_family = AF_INET;
    server_adress.sin_addr.s_addr = INADDR_ANY;
    server_adress.sin_port = htons(port);

    if(bind(main_socket, (struct sockaddr*) &server_adress, sizeof(server_adress))<0) {
        printf("Error binding the main server socket!\n");
        exit(1);
    }
    printf("Server socket binded\n");

    if(listen(main_socket,MAX_CLIENTS)<0) {
        printf("Error listening to socket");
        exit(1);
    }
    printf("Main socket is listening\n");

    while(1) {
        int new_client_id = 0;
        int cpid = 0;
        client_socket = accept(main_socket, (struct sockaddr*)&client_address,&client_adress_size);
        if(client_socket<0){
            printf("Error accepting client connection! ERRNO=%d\n",errno);
            continue;
        }
        new_client_id = *client_count;
        *client_count+=1;
        cpid = fork();

        if(cpid == 0){
            close(main_socket);
            cpid = fork();
            if(cpid == 0){
                process_client(new_client_id,client_socket);
                exit(0);
            }
            else{
                wait(NULL);
                printf("Succesfully orphaned client %d\n",new_client_id);
                exit(0);
            }
        }
        else close(client_socket);
    }
}

/* This is where we have to send and process packets */
void process_client(int id,int socket){
    int i=0;
    char in[1];
    char out[100];
    
    printf("Processing client id=%d, socket=%d\n",id,socket);
    printf("CLIENT count %d\n",*client_count);
 
    while(1){
        read(socket,in,1);
    /*    if(in[0]>-1 && in[0]<126 && in[0] != 10 && in[0] != 3){*/
        if((in[0]>47 && in[0]<58) || (in[0]>64 && in[0]<126) || in[0] == 0 || in[0] == 10){
         shared_data[MAX_CLIENTS+id] += 1;
         if(in[0] == 10){
           printf("Character received:  New line\n");
         }else{
           printf("Character received: %c \n", in[0]);
         }
       
          int m;
          for(m =0; m<shared_data[MAX_CLIENTS+id]; m++){
            sprintf(out,"%c",in[0]);
            write(socket,out,1);
          }
            i = (int)in[0]-48;
            shared_data[id] = i;
           /*sprintf(out,"CLIENT %d Total bytes used by client: %d\n",id,shared_data[MAX_CLIENTS+id]);*/
          printf("CLIENT %d total bytes used %d\n",id,shared_data[MAX_CLIENTS+id]);
        }
    }
}


int main(int argc, char** argv){
    int i;
    /* Assigns default values to server connection info */
    char* server_hostname = DEFAULT_HOSTNAME;
    int server_port = DEFAULT_PORT;
    /* Process forking status */
    int is_child_proc = 0;

    /* ========== READS SERVER'S CONNECTION INFO ========== */

    printf("\tReading connection parameters... ");
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
    printf("hostname=\"%s\", port=%d\n", server_hostname, server_port);


    /* ========== ALLOCATES MEMORY FOR SERVER'S NEEDS ========== */

    int requested_mem_size = SHARED_MEMORY_SIZE;
    /*ja vajadzēs vairāk atmiņu packetos vai gamestate tad varēs palielināt memory size no 1024 */
    int* memory_block = malloc(requested_mem_size * sizeof(int));
    /*pagaidam game state buus 512 baitus liels memory */
    int* game_state = (int*)(memory_block +sizeof(int)*requested_mem_size/2);

    /* memory block struktūra:
    first_client input(128 baiti) ->  second_client input(128 baiti) -> third_client input(128 baiti) -> fourth_client input(128 baiti) -> gamestate_client input(512 baiti)
    /*pagaidam katram clientam bus 128 baiti */
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

    printf("\tRunning space-pong server... ");

    /* Forks child process */
    is_child_proc = fork();
    /* If child process, starts the game */
    if (is_child_proc){
        gameloop();
    } else {
        /* Parent process starts the server */
        start_network(server_port);
    }
    printf("Bytes received: %d", *total_bytes_used);
    return 0;
}