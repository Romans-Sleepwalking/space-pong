/* header file */
#include "stuffchiks_lib.h"
/* PMB allowed libraries */
#include <sys/socket.h>
#include <arpa/inet.h>
#include<stdio.h>
#include<string.h>
#include <unistd.h>
#include <netdb.h>
#include <stdlib.h>
#define HOST "127.0.0.1"
#define PORT 12345
int get_port(int argc, char** argv);
int str_length(char* mystring);

int main(int argc, char** argv){
    char server_reply[6000];
    int port = get_port(argc, argv); 
    char address[15];
    /* gettting the address */
      int i = 0;
  for(i=0;i<argc;i++){
    if(argv[i][0]=='-' && argv[i][1]=='a' && argv[i][2]=='='){
        int k;
        /* char adress_string[str_length(argv[i])-3];*/
        for(k = 3; k < str_length(argv[i]); k++){
          address[k-3] = argv[i][k];       
        }
    }
  }
     if(port == -1){
      port = 12345;
    }
    if(str_length(address) == 0){
      address[0] = "localhost";
      printf("Adress was not included");
    }
     printf("%s\n", address);
    /*#define HOST "localhost" */
    int my_socket = 0;
    char *servername;
    struct sockaddr_in remote_address;
    char inputs[100];
    remote_address.sin_family = AF_INET;
    remote_address.sin_port = htons(port);
   /*shi rindina lkm citadak jo ir cipari nevis localhost*/
    servername=gethostbyname(address);
    if((my_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("SOCKET ERROR\n");
        return -1;
    }
    int pid = 0;
    pid = fork();
    if(pid!=0){
        while(1){
            if( recv(my_socket, server_reply , 6000 , 0) > 0)
	             {
	               puts(server_reply);
	             }
        }
    }
    else{   
    inet_pton(AF_INET, servername, &remote_address.sin_addr);
    if(connect(my_socket, (struct sockadrr*)&remote_address, sizeof(remote_address))<0){
       printf("Error connecting\n");
       return -1;
    }else{
        printf("Connected\n");
        while(1){
            scanf("%s", inputs);
            send(my_socket, inputs, strlen(inputs), 0);
        }
    }
    }
    return 0;
}

int get_port(int argc, char** argv){
  int i = 0;
  int port = -1;
  for(i=0;i<argc;i++){
    printf("%s\n", argv[i]);
    if(argv[i][0]=='-' && argv[i][1]=='p' && argv[i][2]=='='){
        printf("Port is: " );
        int k;

        char port_string[str_length(argv[i])-3];
        for(k = 3; k < str_length(argv[i]); k++){
          port_string[k-3] = argv[i][k];
          
        }
        printf("%s\n", port_string);
        port = atoi(port_string);
    }
  }
  return port;
}
int str_length(char* mystring){
  int len = 0;
  while(mystring[len]!='\0') len++;
  return len;
}