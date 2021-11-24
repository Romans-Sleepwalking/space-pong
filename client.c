/* header file */
#include "stuffchiks_lib.h"
/* PMB allowed libraries */
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** argv) {
    int i;
    /* Client connection info */
    char* client_address;
    int client_port;

    printf("\tRunning space-pong client...\n");
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
    printf("\tClient connection info: address=\"%s\"; port=%d;\n", client_address, client_port);

    return 0;
}
