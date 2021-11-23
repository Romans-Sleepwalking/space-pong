/* header file */
#include "stuffchiks_lib.h"
/* PMB allowed libraries */
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** argv) {
    int i;
    /* Server connection info */
    char* server_hostname;
    int server_port;

    printf("   Running space-pong server...\n");
    /* Reads server connection info */
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
    printf("   Server connection info: hostname=\"%s\"; port=%d;\n", server_hostname, server_port);

    /* TODO */

    return 0;
}
