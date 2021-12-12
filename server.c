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
#define DEFAULT_HOSTNAME "localhost"
#define DEFAULT_PORT 6900
#define DEFAULT_SHARED_MEMORY_SIZE 1024
#define BIGGEST_BUFFER 10000
#define MAX_CLIENTS 10
#define PORT 12348
#define SHARED_MEMORY_SIZE 10000
#define GAME_STATE_SIZE 10000
#define MAX_INCOMING_PACKET_SIZE 300
#define ARRAY_INDEX(array, string_idx) ((array) + (string_idx) * MAX_INCOMING_PACKET_SIZE*2)
/* Global variables */

void update_game_state();
int get_writeable_packet_in_buffer( int id);
void print_Bytes(void* packet, int count);




char* block = NULL;
char* game_state_block = NULL;
char* shared_memory = NULL;
int client_count = 0;
int* shared_data = NULL;
int* total_bytes_used = 0;
char* game_state = NULL;
char *array_of_client_buffers = NULL;

/* Gets shared memory */
void get_shared_memory(){
    /* Packet shared memory  */
   block = attach_memory_block(FILENAME, MAX_INCOMING_PACKET_SIZE*2*10);
   if(block == NULL){
       printf("Memory could not be allocated\n");
   }
    array_of_client_buffers = (char*)calloc(MAX_CLIENTS, MAX_INCOMING_PACKET_SIZE*2);

     int i;
    for( i=0; i<MAX_CLIENTS; i++){
     ARRAY_INDEX(array_of_client_buffers, i)[0] = '0';
     ARRAY_INDEX(array_of_client_buffers, i)[MAX_INCOMING_PACKET_SIZE] = '0';
      } 
      strncpy(block, array_of_client_buffers, MAX_INCOMING_PACKET_SIZE*2*10);
    /* Game state shared memory*/
    /* MAX clients = 10 therefore memory will be requested for five teams */
    int team_cnt = MAX_CLIENTS/2;
    int player_cnt = MAX_CLIENTS;
    /* for now ball count will be 1 */
    int ball_cnt = 1;
    /* for now powerup count will be 1 */
    int powerup_cnt = 1;
    int total_gamestate_size = 5+ 21*team_cnt +1+ player_cnt*18 + 1 + ball_cnt*13 + 1 + powerup_cnt*17;
     game_state_block = attach_memory_block(FILENAME, total_gamestate_size);
   
    /*    GAMESTATE STRUKTŪRA
     
            *game_state_block  - window wit=dth
            *(game_state_block + 4) -  window height
            *(game_state_block + 8) -  team count 
            *(game_state_block + 9 + 21*team_id) -  komandas id
            *(game_state_block + 9 + 21*team_id + 1) - komandas score
            *(game_state_block + 9 + 21*team_id + 5) - savas komandas vartu lokacija X
            *(game_state_block + 9 + 21*team_id + 9) - savas komandas vartu lokacija Y
            *(game_state_block + 9 + 21*team_id + 13) - pretinieku komandas vartu lokacija X
            *(game_state_block + 9 + 21*team_id + 13) - pretinieku komandas vartu lokacija Y
            *(game_state_block + 9 + 21*team_cnt) - speletaju counts
            *(game_state_block + 9 + 21*team_cnt + 1 + 18*player_id) - speletaja id
            *(game_state_block + 9 + 21*team_cnt + 1 + 18*player_id + 1) - speletaja komandas id
            *(game_state_block + 9 + 21*team_cnt + 1 + 18*player_id + 2) - speletaja X coordinates
            *(game_state_block + 9 + 21*team_cnt + 1 + 18*player_id + 6) - speletaja Y coordinates
            *(game_state_block + 9 + 21*team_cnt + 1 + 18*player_id + 10) - speletaja width
            *(game_state_block + 9 + 21*team_cnt + 1 + 18*player_id + 14) - speletaja height
            *(game_state_block + 9 + 21*team_cnt + 1 + 18*player_cnt ) - bumbu count
            *(game_state_block + 9 + 21*team_cnt + 1 + 18*player_cnt + 1 + ball_id*13) - bumbas X coordinate
            *(game_state_block + 9 + 21*team_cnt + 1 + 18*player_cnt + 1 + ball_id*13 +4) - bumbas Y coordinate
            *(game_state_block + 9 + 21*team_cnt + 1 + 18*player_cnt + 1 + ball_id*13 +8) - ball radius
            *(game_state_block + 9 + 21*team_cnt + 1 + 18*player_cnt + 1 + ball_id*13 +12) - ball Type/color
            *(game_state_block + 9 + 21*team_cnt + 1 + 18*player_cnt + 1 + ball_cnt*13) - powerup up count
            *(game_state_block + 9 + 21*team_cnt + 1 + 18*player_cnt + 1 + ball_cnt*13 + 1 + powerup_id*17) - powerup type
            *(game_state_block + 9 + 21*team_cnt + 1 + 18*player_cnt + 1 + ball_cnt*13 + 1 + powerup_id*17 + 1) - powerup X
            *(game_state_block + 9 + 21*team_cnt + 1 + 18*player_cnt + 1 + ball_cnt*13 + 1 + powerup_id*17 + 5) - powerup Y
            *(game_state_block + 9 + 21*team_cnt + 1 + 18*player_cnt + 1 + ball_cnt*13 + 1 + powerup_id*17 + 9) - powerup width
            *(game_state_block + 9 + 21*team_cnt + 1 + 18*player_cnt + 1 + ball_cnt*13 + 1 + powerup_id*17 + 13) - powerup height
                           
          */
}

/* Sends and process client's packets */
void process_client(int id,int socket){
   /* int i;
    char in[1];
    char out[100];
    int k = 0; */
    int proccess;
   char this_packet[MAX_INCOMING_PACKET_SIZE]="ABC";
  char tmp[BIGGEST_BUFFER]="ABC";
  int n = 0;
  int i = 0;
  char in[1];
  char prev_was_divider = 0;
  char in_packet = 0;
  char escape_mode = 0;
  int last_packet_id = 0;
  int mode = 0;
   printf("Process client works");
    /* ... */
    while(1){
        read(socket,in,1);
        if(in[0] == '-' && !escape_mode){
      /* packet ends */
      if(n<3) {
        /* Discard silently */
      } else {
        if(packet_is_ok(tmp, n, last_packet_id))
        {
               mode = get_writeable_packet_in_buffer(id);
               if(mode == 1)
               {
                    /*
                    printf("\n Address at package receival %p ",&ARRAY_INDEX(array_of_client_buffers, 0)[MAX_INCOMING_PACKET_SIZE]  );
                    printf("\n Character:  %c \n",ARRAY_INDEX(array_of_client_buffers, 0)[MAX_INCOMING_PACKET_SIZE]  );
                    */
                    for(i=0; i<n;i++) 
                     { 
                      /* ARRAY_INDEX(array_of_client_buffers, id)[i+1] = tmp[i]; */
                      ARRAY_INDEX(block, id)[i+1] = tmp[i];
                     }
                     /* just in case put 0 after packet */
                     /*
                     ARRAY_INDEX(array_of_client_buffers, id)[n+1]=0; */
                     ARRAY_INDEX(block, id)[n+1]=0;
                     last_packet_id = get_4_bit_integer(tmp);
                     printf("Received packet of %d bytes with npk=%d\n",n, last_packet_id);
                     /*
                     print_Bytes(&(ARRAY_INDEX(array_of_client_buffers, id)[1]), n);
                     printf("%c\n",ARRAY_INDEX(array_of_client_buffers, id)[0] ); */
                      print_Bytes(&(ARRAY_INDEX(block, id)[1]), n);
                     printf("%c\n",ARRAY_INDEX(block, id)[0] );

                }else if(mode == 2)
                   {
                       for( i=0; i<n;i++)
                         {   
                             /*
                               ARRAY_INDEX(array_of_client_buffers, id)[i+MAX_INCOMING_PACKET_SIZE+1] = tmp[i]; */
                               ARRAY_INDEX(block, id)[i+MAX_INCOMING_PACKET_SIZE+1] = tmp[i];
                         }
                     /* just in case put 0 after packet */
                       /* ARRAY_INDEX(array_of_client_buffers, id)[n+MAX_INCOMING_PACKET_SIZE+1]=0; */
                       ARRAY_INDEX(block, id)[n+MAX_INCOMING_PACKET_SIZE+1]=0;
                        last_packet_id = get_4_bit_integer(tmp);
                         printf("Received packet of %d bytes with npk=%d\n",n, last_packet_id);
                         /*
                         print_Bytes(&(ARRAY_INDEX(array_of_client_buffers, id)[MAX_INCOMING_PACKET_SIZE+1]), n); */
                       print_Bytes(&(ARRAY_INDEX(block, id)[MAX_INCOMING_PACKET_SIZE+1]), n);
                    }
                   proccess =  fork();  
                   if(proccess == 0){
                       update_game_state();
                        exit(0);
                   }
                 /*  printf("Char should be 0: %c", ARRAY_INDEX(array_of_client_buffers, id)[0]); */
        }
     }  
           n = 0;
          in_packet = 0;
      if(prev_was_divider){
        in_packet = 1;
      } else {
        prev_was_divider = 1;
      }
    } else {
      if(in_packet){
        prev_was_divider = 0;
        /* un-escape if needed */
        if(in[0] == '?'){
          escape_mode = 1;
          continue;
        }
        if(escape_mode){          
          escape_mode = 0;
          if(unescape(in) == 1){
            n = 0;
            in_packet = 0;
            continue;
          }         
        }

        tmp[n] = in[0];
        n++;
      }
    }

            /* ... 
            for (i = 0; i < shared_data[DEFAULT_MAX_CLIENTS + id]; i++){
                sprintf(out,"%c",in[0]);
                write(socket,out,1);
            }*/
            /* ... */
          
        
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
            new_client_id = client_count;
            /* Records stats */
            client_count += 1;
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
    server_hostname = read_param_value(argc, argv, 'h');
    server_port = atoi(read_param_value(argc, argv, 'p'));
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
    if (is_parent_proc == 0){
        launch_game(game_state_partition);
    } else {
            /* Child process calls the start server polling function */
           start_polling(server_hostname, server_port);
        }
    
    return 0;
}


/* ------------------------------------------------------------------ */


char calculate_checksum(char* buffer, int n){
  int i;
  char res=0;
  for(i = 0; i<n; i++){
    res ^= buffer[i];
  }
  return res;
}

int unescape(char* ch){
  /* printf("Un-escaping!\n"); */
  if(ch[0] == '-') ch[0] = '-';
  else if(ch[0] == '*') ch[0] = '?';
  else return 1;
  return 0;
}

int get_4_bit_integer(void * addr){
  return ntohl(*((int*)addr));
}

void put_4_bit_integer(int data, void* addr){
  int* destination = addr;
  *destination = htonl(data);
}

int packet_is_ok(char* buffer, int n, int last_id){
  int new_id = 0;
  int length = 0;
  unsigned char goal_checksum = (unsigned char) buffer[n-1];
  unsigned char current_checksum = 0;
  /* ID either 0 or bigger than previous? */
  new_id = get_4_bit_integer(buffer);
  if(new_id>0 && new_id<=last_id){
    printf("Received packet out of order! Last ID = %d, received %d\n", last_id, new_id);
    return 0;
  }
  /* Data length ok? */
  length = get_4_bit_integer(buffer+5);


  /* checksum ok? */
  current_checksum = (unsigned char) calculate_checksum(buffer, n-1);
  if(current_checksum != goal_checksum){
    printf("Packet chekchsum failed! received = %d, calculated %d\n", goal_checksum, current_checksum);
    return 0;
  }
  return 1;
}


char printable_char(char c){
    if(isprint(c)) return c;
    return '#';
}

void print_Bytes(void* packet, int count){
    int i;
    unsigned char* p = (unsigned char*) packet;
    if(count>999){
        printf("Cannot print more than 999 bytes! You asked for %d\n",count);
        return;
    }
    printf("Printing %d bytes...\n",count);
    printf("[NPK] [C] [HEX] [DEC] [ BINARY ]\n");
    printf("================================\n");
    for(i=0;i<count;i++){
        printf(" %3d | %c | %02X | %3d | %c%c%c%c%c%c%c%c\n",i,printable_char(p[i]),p[i],p[i],
        p[i] & 0x80 ? '1':'0',
        p[i] & 0x40 ? '1':'0',
        p[i] & 0x20 ? '1':'0',
        p[i] & 0x10 ? '1':'0',
        p[i] & 0x08 ? '1':'0',
        p[i] & 0x04 ? '1':'0',
        p[i] & 0x02 ? '1':'0',
        p[i] & 0x01 ? '1':'0'
        );
    }
}
int get_writeable_packet_in_buffer(int id){
char available = ARRAY_INDEX(block, id)[0];
char available2 = ARRAY_INDEX(block, id)[MAX_INCOMING_PACKET_SIZE];

    if(available == '0'){
   ARRAY_INDEX(block, id)[0] = '1';
   return 1;
 }else if(available2 == '0'){

  ARRAY_INDEX(block, id)[MAX_INCOMING_PACKET_SIZE] = '1';
      return 2;
 }
     return -1; 
}
void update_game_state(){
      if(ARRAY_INDEX(block, 0)[0] == '1'){
          /* Janolasa packets un jaupdateo gamestate  */
             ARRAY_INDEX(block, 0)[0] = '0';
      }
}



