#include "serverSocketHelper.h"

#define RECVSTRLEN 20 

int main() {
    SocketWrapper *socket;
    ConnectionWrapper *newConnection;
    char *buf = (char *)malloc(sizeof(RECVSTRLEN));

    socket = initSocket(DEFAULT_PORT, DEFAULT_QUEUE);
    newConnection = acceptOneConnection(socket);
    for (;;) {
        int t = sendString(newConnection, "Hello");
        int recvLen = recvString(newConnection, buf, RECVSTRLEN);
        printf("%s\n", buf);

        if (recvLen == 0) break;
    }
    closeConnection(newConnection);
    closeSocket(socket);

    return 0;
}
