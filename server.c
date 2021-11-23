#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** argv) {
    int i;
    int it;
    char* param;
    char num_str[8];
    /* Server connection info */
    char server_hostname[64];
    int server_port;

    printf("   Running space-pong server...\n");
    /* Reads server connection info */
    for (i=0; i<argc; i++){
        param = argv[i];
        /* Reads hostname */
        if (param[0] == '-' && param[1] == 'h' && param[2] == '='){
            /* Fills the hostname char-by-char */
            it = 3;
            while (1){
                server_hostname[it-3] = param[it];
                if (param[it] == '\0'){
                    break;
                } else {
                    it++;
                }
            }
        }
        /* Reads port */
        else if (param[0] == '-' && param[1] == 'p' && param[2] == '='){
            /* Fills the port char-by-char */
            it = 3;
            while (1){
                num_str[it-3] = param[it];
                if (param[it] == '\0'){
                    break;
                } else {
                    it++;
                }
            }
            /* Translates the port string to integer */
            server_port = atoi(num_str);
        }
    }
    printf("   Server connection info: hostname=\"%s\"; port=%d;\n", server_hostname, server_port);

    return 0;
}
