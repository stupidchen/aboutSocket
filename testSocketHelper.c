#include "serverSocketHelper.h"

#define RECVSTRLEN 20 

int main() {
    SocketWrapper *socket;
    ConnectionWrapper *newConnection;
    char *buf = (char *)malloc(sizeof(RECVSTRLEN));

    char *hostname;
    hostname = getHostName(DEFAULT_HOSTNAME_MAXLEN);
    printf("Hostname: %s\n", hostname);

    socket = initSocket(DEFAULT_PORT, DEFAULT_QUEUE);
    newConnection = acceptOneConnection(socket);
    for (;;) {
        char *peername = getPeerName(newConnection);
        printf("New connection.\n Peername: %s\n", peername);
        int t = sendString(newConnection, "Hello");
        int recvLen = recvString(newConnection, buf, RECVSTRLEN);
        printf("Receive: %s\n", buf);

        if (recvLen == 0) break;
    }
    closeConnection(newConnection);
    closeSocket(socket);

    return 0;
}
