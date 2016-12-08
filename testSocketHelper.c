#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#include "serverSocketHelper.h"

#define RECVSTRLEN 40 

struct argStruct {
    SocketWrapper *socket;
    pthread_mutex_t *mutex;
};

typedef struct argStruct arg;

void *acceptConnections(void *args) {
    arg *tmpArg = (arg *)args;
    SocketWrapper *socket = tmpArg->socket;
    ConnectionWrapper *newConnection;
    char str[RECVSTRLEN * 2];


    for (;;) {
//        pthread_mutex_lock(tmpArg->mutex);
        printf("Start to listen %s\n", str);
        newConnection = acceptOneConnection(socket);
//        pthread_mutex_unlock(tmpArg->mutex);
        sprintf(str, "Connection %d built.", socket->connectionNumber);
        sendString(newConnection, str);
    }
}

int main() {
    SocketWrapper *socket;
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
    pthread_t listenThread;

    arg *tmpArg = (arg *)malloc(sizeof(arg));
    tmpArg->mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(tmpArg->mutex, NULL);
    tmpArg->socket = socket;

    pthread_create(&listenThread, NULL, acceptConnections, tmpArg);

    char str[RECVSTRLEN * 2];
    struct timeval timeout;
    
    for (;;) {
        timeout.tv_sec = 3;
        timeout.tv_usec = 0;
//        pthread_mutex_lock(tmpArg->mutex);
        SelectResult *result = selectReadyConnections(socket, &timeout);
//        pthread_mutex_unlock(tmpArg->mutex);
        printf("%d connection ready.\n", result->num);
        ConnectionWrapper *tmp = result->connectionHead;
        while (tmp != NULL) {
            int recvLen = recvString(tmp, buf, RECVSTRLEN);
            if (strcmp(buf, "exit") == 0 || recvLen == 0) {
                sprintf(str, "Received exit signal.");
                sendString(tmp, str);
                closeConnection(tmp);
            }
            else {
                sprintf(str, "Received %d bytes: %s", recvLen, buf);
                sendString(tmp, str);
            }
            tmp = tmp->nextSelect;
        }
    }
}
