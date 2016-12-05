#ifndef SERVER_SOCKET_HELPER
#define SERVER_SOCKET_HELPER

//select switch
#define USE_SELECT
#define USE_EPOLL

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include <sys/shm.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/epoll.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

#ifdef USE_SELECT
#define SELECT_STATUS 1
#else 
#define SELECT_STATUS 0
#endif

#ifdef USE_EPOLL
#define EPOLL_STATUS 1
#else
#define EPOLL_STATUS 0
#endif

#define DEFAULT_PORT "8887"
#define DEFAULT_QUEUE 20
#define DEFAULT_BUFFER_SIZE 1024
#define DEFAULT_SELECT_SET_SIZE 16

#define DEFAULT_HOSTNAME_MAXLEN 20

#define DISALLOW_RECEIVE 0
#define DISALLOW_SEND 1
#define DISALLOW_RS 2

#define EPOLL_DEFAULT_FDSIZE 10000

struct ConnectionWrapperStruct {
    int fd;
    struct sockaddr address;
    socklen_t addrlen;
    struct ConnectionWrapperStruct *next;
    struct ConnectionWrapperStruct *nextSelect;
};

struct SocketWrapperStruct {
	int fd, maxfd;
    int epollfd;
	struct ConnectionWrapperStruct *connectionHead, *connectionTail;
    int connectionNumber;
    fd_set *fs;
};

struct SelectResultStruct {
    int num;
    struct ConnectionWrapperStruct *connectionHead;
};

struct EpollEventsStruct {
    int num;
    struct epoll_event *events;
}; 

typedef struct ConnectionWrapperStruct ConnectionWrapper;

typedef struct SocketWrapperStruct SocketWrapper;

typedef struct SelectResultStruct SelectResult;

typedef struct EpollEventsStruct EpollEvents;

//init:Get descriptor, bind, listen
extern SocketWrapper *initSocket(char *port, int queue);

extern ConnectionWrapper *acceptOneConnection(SocketWrapper *socket);

extern int acceptConnections(SocketWrapper *socket, int num);

extern int sendString(ConnectionWrapper *connection, char *str);

extern int recvString(ConnectionWrapper *connection, char *str, int len);

extern int sendBinaries(ConnectionWrapper *connection, unsigned char *bytes);

extern char *getPeerName(ConnectionWrapper *connection);

extern char *getHostName(int len);

void shutdownConnection(ConnectionWrapper *connection, int status);

extern void closeSocket(SocketWrapper *socket); 

extern void closeConnection(ConnectionWrapper *connection);

#ifdef USE_SELECT
extern SelectResult *selectReadyConnections(SocketWrapper *socket, struct timeval *timeout);
#endif

#ifdef USE_EPOLL
extern void addEpollEvent(SocketWrapper *socket, ConnectionWrapper *connection, EpollEvents *events);

extern void deleteEpollEvent(SocketWrapper *socket, ConnectionWrapper *connection, EpollEvents *events);

extern void modifyEpollEvent(SocketWrapper *socket, ConnectionWrapper *connection, EpollEvents *events);

extern EpollEvents *epollWait(SocketWrapper *socket, int maxevents, int timeout);
#endif

#endif 
