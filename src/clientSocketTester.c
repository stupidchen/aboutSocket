#include "include/clientSocketHelper.h"

#define SERVER_ADDRESS "112.74.33.156" 
#define SERVER_PORT 8887 
#define BUFFER_SIZE 30

int main(void) {
    SocketWrapper *sw = initSocket(SERVER_ADDRESS, SERVER_PORT);

    char sendbuf[BUFFER_SIZE];
    char *recvbuf;
    memset(sendbuf, 0, sizeof(sendbuf));

    int len;
    while (fgets(sendbuf, BUFFER_SIZE, stdin) != NULL) {
        len = sendString(sw, sendbuf);
        if (strcmp(sendbuf, "exit\n") == 0) {
            break;
        }
        recvbuf = recvString(sw, 0); 
        printf("%s", recvbuf);
         
        memset(sendbuf, 0, sizeof(sendbuf));
        free(recvbuf);
    }

    closeSocket(sw);
    return 0;
}
