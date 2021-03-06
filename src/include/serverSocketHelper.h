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
#define EPOLL_DEFAULT_MAX_CONCURRENT_CONNECTIONS 100

#define EPOLL_LISTEN_GROUP 0
#define EPOLL_BOARDCAST_GROUP 1
#define EPOLL_DEFAULT_GROUP_NUMBER 10

struct ConnectionWrapperStruct {
    int fd;
    struct sockaddr address;
    socklen_t addrlen;
    struct ConnectionWrapperStruct *next, *last;
    struct ConnectionWrapperStruct *nextSelect;
    struct SocketWrapperStruct *socket;
};

struct SocketWrapperStruct {
	int fd;
    fd_set *fs;
    int fdGroupNumber;
    
    //For connection wrapper
	struct ConnectionWrapperStruct *connectionHead, *connectionTail;
    int connectionNumber;

    //For epoll fd group
    int efdGroupNum;
    int efdMaxGroupNum;
    int *efds;
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

/*struct epoll_event {
    __uint32_t events; //EPOLLIN, EPOLLOUT, EPOLLPRI, EPOLLERR, EPOLLHUB, EPOLLET, EPOLLONESHOT
    epoll_data_t data;
};*/
typedef struct epoll_event EpollEvent;

//init:Get descriptor, bind, listen
extern SocketWrapper *initSocket(char *port, int queue);

extern ConnectionWrapper *acceptOneConnection(SocketWrapper *socket);

extern int sendString(ConnectionWrapper *connection, char *str);

extern int recvString(ConnectionWrapper *connection, char *str, int len);

extern char *getPeerName(ConnectionWrapper *connection);

extern char *getHostName(int len);

void shutdownConnection(ConnectionWrapper *connection, int status);

extern void closeSocket(SocketWrapper *socket); 

extern void closeConnection(ConnectionWrapper *connection);

#ifdef USE_SELECT
extern SelectResult *selectReadyConnections(SocketWrapper *socket, struct timeval *timeout);
#endif

#ifdef USE_EPOLL

//EPOLLIN, EPOLLOUT, EPOLLPRI, EPOLLERR, EPOLLHUB, EPOLLET, EPOLLONESHOT
extern EpollEvent *createEpollEvent(void *connection, __uint32_t status);

extern EpollEvent *createEpollEventByFd(int fd,  __uint32_t status);

extern EpollEvents *createEpollEvents(unsigned long size);

extern void addEpollEvent(SocketWrapper *socket, int group, ConnectionWrapper *connection, EpollEvent *event);

extern void addEpollEventByFd(SocketWrapper *socket, int group, int fd, EpollEvent *event);

extern void deleteEpollEvent(SocketWrapper *socket, int group, ConnectionWrapper *connection, EpollEvent *event);

extern void deleteEpollEventByFd(SocketWrapper *socket, int group, int fd, EpollEvent *event);

extern void modifyEpollEvent(SocketWrapper *socket, int group, ConnectionWrapper *connection, EpollEvent *event);

extern void modifyEpollEventByFd(SocketWrapper *socket, int group, int fd, EpollEvent *event);

extern void epollWait(SocketWrapper *socket, int group, EpollEvents *result, int maxevents, int timeout);

extern int getFdFromEvent(EpollEvent event);

extern void *getPtrFromEvent(EpollEvent event);
#endif

#endif 
