#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#include "include/serverSocketHelper.h"

#define RECVSTRLEN 40 

struct argStruct {
    SocketWrapper *socket;
    pthread_mutex_t *mutex;
};

typedef struct argStruct arg;

void *acceptConnections(void *args) {
    int i;

    arg *tmpArg = (arg *)args;
    SocketWrapper *socket = tmpArg->socket;
    ConnectionWrapper *newConnection;
    unsigned long evnum = EPOLL_DEFAULT_MAX_CONCURRENT_CONNECTIONS;
    EpollEvents *connev = createEpollEvents(evnum); 

    int timeout = 1000;
    for (;;) {
        epollWait(socket, EPOLL_LISTEN_GROUP, connev, evnum, timeout);
        //        pthread_mutex_lock(tmpArg->mutex);
        //        printf("Start to listen %s\n", str);
        for (i = 0; i < connev->num; i++) {
            newConnection = acceptOneConnection(socket);
            addEpollEvent(socket, EPOLL_BOARDCAST_GROUP, newConnection, createEpollEvent(newConnection, EPOLLIN));
            printf("Connection %d built.\n", socket->connectionNumber);
        }
        //        pthread_mutex_unlock(tmpArg->mutex);
        //sendString(newConnection, str);
    }
}

int main() {
    int i;

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
    int timeout = 1000;
    EpollEvents *result = createEpollEvents(EPOLL_DEFAULT_FDSIZE);
    
    for (;;) {
//        pthread_mutex_lock(tmpArg->mutex);
//        pthread_mutex_unlock(tmpArg->mutex);
//        
//        
        epollWait(socket, EPOLL_BOARDCAST_GROUP , result, EPOLL_DEFAULT_FDSIZE, timeout);   
        printf("%d connection ready.\n", result->num);
        for (i = 0; i < result->num; i++) {
            EpollEvent tmp = result->events[i];
            if (tmp.events & EPOLLIN) {
                ConnectionWrapper *readyConn = (ConnectionWrapper *)getPtrFromEvent(tmp); 
                int recvLen = recvString(readyConn, buf, RECVSTRLEN);
                if (strcmp(buf, "exit") == 0 || recvLen == 0) {
                    printf("Received exit signal.");
                    sprintf(str, "Bye.");
                    sendString(readyConn, str);
                    closeConnection(readyConn);
                }
                else {
                    sprintf(str, "Received %d bytes: %s", recvLen, buf);
                    sendString(readyConn, str);
                }
            }
        }
    }
}
