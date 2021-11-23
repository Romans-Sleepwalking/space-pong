#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** argv) {
    int i;
    int it;
    char* param;
    char num_str[8];
    /* Client connection info */
    char client_address[64];
    int client_port;

    printf("   Running space-pong client...\n");
    /* Reads client connection info */
    for (i=0; i<argc; i++){
        param = argv[i];
        /* Reads address */
        if (param[0] == '-' && param[1] == 'a' && param[2] == '='){
            /* Fills the address char-by-char */
            it = 3;
            while (1){
                client_address[it-3] = param[it];
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
            client_port = atoi(num_str);
        }
    }
    printf("   Client connection info: address=\"%s\"; port=%d;\n", client_address, client_port);

    return 0;
}
