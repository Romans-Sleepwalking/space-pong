/* PBM765 allowed library */
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdbool.h>
#include <ctype.h>
#include <arpa/inet.h>
/* error's magic value */
#define IPC_RESULT_ERROR (-1)

/*
   ================================

     SPACE-PONG UTILITY FUNCTIONS

   ================================
*/

/*
   ================================
         Parameter Operations
   ================================
*/

/* Finds and picks a parameter value substring from a parameter array */
char* read_param_value(int argc, char** argv, char magic_char){
    int i, j;
    char* substring = malloc(32);
    /* Finds the required parameter */
    for (i=0; i<argc; i++){
        if (argv[i][0] == '-' && argv[i][1] == magic_char && argv[i][2] == '='){
            j = 3;
            /* Loops until meets EOF character */
            while (1){
                /* Fills the substring character array */
                substring[j-3] = argv[i][j];
                if (argv[i][j] == '\0'){
                    break;
                } else j++;
            }
            return substring;
        }
    }
    return "Error: Parameter not found!";
}

/*
   ================================
          String Operations
   ================================
*/

/* Counts string length */
int str_length(char* string){
    /* Loops string characters until EOF char, counting length */
    int length;
    for (length = 0; string[length] != '\0'; length++);
    return length;
}

/* Copies string content inside the another string by pointers */
void str_copy(char* source, char* destination){
    while ((*destination++ = *source++));
}

/* Tries to find a substring */
int str_find(char* needle, char* haystack){
    int len_haystack = str_length(haystack);
    int len_needle = str_length(needle);
    int needle_char_match_count = 0;
    int i;
    /* Loops haystack's length where neddle is possible */
    for (i = 0; i < len_haystack - len_needle; i++) {
        /* Checks every next character for matching the substring */
        while (haystack[i + needle_char_match_count] == needle[needle_char_match_count]){
            needle_char_match_count++;
            /* If match, returns the substring first index */
            if (needle_char_match_count == len_needle){
                return i + 1;
            }
        }
        /* Moves to next haystack character character */
        needle_char_match_count = 0;
    }
    /* If no substring found, returns the -1 magic number  */
    return -1;
}

/* Formats... TODO */
int unescape(char* char_ptr){
    switch (*char_ptr) {
        case '-':
            *char_ptr = '-';
            return 0;
        case '*':
            *char_ptr = '?';
            return 0;
        default:
            return 1;
    }
}

/*
   ================================
          Packet Operations
   ================================
*/

/* Replaces unprintable characters with number sign */
char printable_char(char c){
    if ( isprint(c) ){
        return c;
    } else {
        return '#';
    }
}

/* Prints packet's content in human-readable format */
void print_packet_bytes(void* packet, int size){
    int i;
    unsigned char* p = (unsigned char*) packet;
    /* If packet size is less than a thousand bytes  */
    if (size < 999) {
        printf("Printing %d bytes...\n", size);
        printf("[NPK] [C] [HEX] [DEC] [ BINARY ]\n");
        printf("================================\n");
        for (i = 0; i < size; i++) {
            printf(" %3d | %c | %02X | %3d | %c%c%c%c%c%c%c%c\n", i, printable_char(p[i]), p[i], p[i],
                   p[i] & 0x80 ? '1' : '0',
                   p[i] & 0x40 ? '1' : '0',
                   p[i] & 0x20 ? '1' : '0',
                   p[i] & 0x10 ? '1' : '0',
                   p[i] & 0x08 ? '1' : '0',
                   p[i] & 0x04 ? '1' : '0',
                   p[i] & 0x02 ? '1' : '0',
                   p[i] & 0x01 ? '1' : '0'
            );
        }
    } else {
        printf("Error: packet of %d bytes is too large for printing! (999 bytes max)\n", size);
        return;
    }
}

/* Calculates checksum */
char calculate_checksum(char* buffer, int n){
    int i;
    char res = 0;
    for(i = 0; i < n; i++){
        res ^= buffer[i];
    }
    return res;
}

/* Reads network-formatted 4-bit integer from the right place in memory */
int get_4_bit_integer(void * addr){
    return ntohl(*((int*)addr) );
}

/* Translates integer to network 4-bit format, and puts it into the right place in memory */
void put_4_bit_integer(int data, void* addr){
    int* destination = addr;
    *destination = htonl(data);
}

/* Verifies the received packet by order, length and checksum */
int is_packet(char* buffer, int n, int previous_packet_id){
    int this_id;
    int length;
    unsigned char required_checksum = (unsigned char) buffer[n-1];
    unsigned char this_checksum;
    /* New packet ID */
    this_id = get_4_bit_integer(buffer);
    /* Checks if packet arrived too late during the process */
    if (0 < this_id && this_id <= previous_packet_id){
        /* Returns false */
        printf("Error: Received packet ID=%d is out of order! (previous ID: %d)\n", previous_packet_id, this_id);
        return 0;
    }
    /* TODO: checks data length */
    length = get_4_bit_integer(buffer+5);
    /* Checks checksum */
    this_checksum = (unsigned char)calculate_checksum(buffer, n-1);
    if (this_checksum != required_checksum){
        printf("Error: Packet checksum=%d failed! (calculated: %d)\n", required_checksum, this_checksum);
        return 0;
    }
    return 1;
}

/*
   ================================
        Memory block operations
   ================================
*/

/* TODO */
static int get_shared_block(char *filename, int size) {
    key_t key;
    key = ftok(filename, 0);
    if (key == IPC_RESULT_ERROR){
        printf("KEY was -1 \n");
        return IPC_RESULT_ERROR;
    }
    return shmget(key, size, 0644 | IPC_CREAT);
}

/* TODO */
char* attach_memory_block(char* filename, int size){
    int shared_block_id = get_shared_block(filename, size);
    char *result;

    if(shared_block_id == IPC_RESULT_ERROR){\
        return NULL;
    }
    result = shmat(shared_block_id, NULL, 0);
    if(result == (char*)IPC_RESULT_ERROR){
        return NULL;
    }
    return result;
}

/* TODO */
bool detach_memory_block(char *block){
    return (shmdt(block) != IPC_RESULT_ERROR);
}

/* TODO */
bool destroy_memory_block(char* filename){
    int shared_block_id = get_shared_block(filename,0);
    if(shared_block_id == IPC_RESULT_ERROR){
        return NULL;
    }
    return (shmctl(shared_block_id, IPC_RMID, NULL) != IPC_RESULT_ERROR);
}