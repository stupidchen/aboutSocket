#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

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
/*    newConnection = acceptOneConnection(socket);*/
    /*char *peername = getPeerName(newConnection);*/
    /*printf("New connection.\n Peername: %s\n", peername);*/
    /*for (;;) {*/
        /*int t = sendString(newConnection, "Hello");*/
        /*int recvLen = recvString(newConnection, buf, RECVSTRLEN);*/
        /*printf("%d\n", recvLen);*/
        /*sleep(10);*/
        /*if (recvLen == 0) break;*/
    /*}*/
    /*closeConnection(newConnection);*/


    char str[RECVSTRLEN * 2];

    pid_t forkPid = fork();
    if (forkPid < 0) {
        perror("Fork error");
        return -1;
    }
    if (forkPid == 0) {
        for (;;) {
            newConnection = acceptOneConnection(socket);
            sprintf(str, "Connection %d built.", socket->connectionNumber);
            sendString(newConnection, str);
        }
    }
    else {
        SelectResult *result = selectReadyConnections(socket, NULL);
        ConnectionWrapper *tmp = result->connectionHead;
        while (tmp != NULL) {
            int recvLen = recvString(tmp, buf, RECVSTRLEN);
            if (strcmp(buf, "exit") == 0) {
                sprintf(str, "Received exit signal.");
                sendString(tmp, str);
                closeConnection(tmp);
            }
            else {
                sprintf(str, "Received %d bytes: %s", recvLen, buf);
                sendString(tmp, str);
            }
        }
    }
    closeSocket(socket);

    return 0;
}
