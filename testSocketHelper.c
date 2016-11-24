#include "serverSocketHelper.h"

int main() {
    SocketWrapper *socket;
    ConnectionWrapper *newConnection;

    socket = initSocket(DEFAULT_PORT, DEFAULT_QUEUE);
    newConnection = acceptOneConnection(socket);
    int t = sendString(newConnection, "Hello");
    printf("%d\n", t);
    closeConnection(newConnection);
    closeSocket(socket);

    return 0;
}
