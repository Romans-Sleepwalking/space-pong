#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#define MAX_PACKET_SIZE 100
void createPacket_1(int packet_number, char* player_name, char* packet);
char get_checksum(char* data, int size_of_data );
void direct_copy_data_as_bytes(void* packet, void* data, int size);
int main(){
  
    /*ja vajadzēs vairāk atmiņu packetos vai gamestate tad varēs palielināt memory size no 1024 */
  
  int* memory_block = malloc(sizeof(char)*MAX_PACKET_SIZE);
      int packet_count = 0;
      char* packet[MAX_PACKET_SIZE];
      createPacket_1(packet_count, "PlayerName", *packet);
      printf("Packet: %s", *packet);

           /*  */
        /* ///////////////////  */
        
    return 0; 
}
void createPacket_1(int packet_number, char* player_name, char* packet){
  /* tracking where i am in the packet */
  int count=0;
  int packet_id = 1;
  long packet_data_size = 20;

  /* 20 \0 */
 char data[20]="\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
/* adding packet number to the packet */
direct_copy_data_as_bytes(packet, &packet_number, 4);
count+=4;
direct_copy_data_as_bytes(packet + sizeof(char)*count, &packet_id, 4);
count+=4;
direct_copy_data_as_bytes(packet + sizeof(char)*count, &packet_data_size, 8);
count+=8;

/* inserting player name into data */
int player_name_length = strlen(player_name);
direct_copy_data_as_bytes(data, player_name, player_name_length); 

char checksum = get_checksum(data, 20); 
direct_copy_data_as_bytes(packet + sizeof(char)*count, &checksum, 1);
count+=1;

 /*
Nav nejusmas kpc vins man neļāva addot data ja es nepieliku +31 pie packet atmiņas
*/
  for(int m =0; m<20; m++){
         char* letter = &data[m];
     packet[m+31+count] = *letter;
         /* packet[i+count]='\0'; */
  
 }
 count += 20;
}
void direct_copy_data_as_bytes(void* packet, void* data, int size){
  int i;
  char* p = packet;
  char* d = data;
  for(i=0; i<size;i++){
    p[i] = d[i];
  }
}
char get_checksum(char* data, int size_of_data ){
  char check = 0;
  for(int i =0; i<size_of_data; i++){
    check ^= data[i];
  }
  return check;
}