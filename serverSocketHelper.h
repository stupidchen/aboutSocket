#ifndef SERVER_SOCKET_HELPER
#define SERVER_SOCKET_HELPER

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/shm.h>

#define DEFAULT_PORT "8887"
#define DEFAULT_QUEUE 20
#define DEFAULT_BUFFER_SIZE 1024

#define DEFAULT_HOSTNAME_MAXLEN 20

#define DISALLOW_RECEIVE 0
#define DISALLOW_SEND 1
#define DISALLOW_RS 2

struct ConnectionWrapperStruct {
    int fd;
    struct sockaddr address;
    socklen_t addrlen;
    struct ConnectionWrapperStruct *next;
};

struct SocketWrapperStruct {
	int fd;
	struct ConnectionWrapperStruct *connectionHead, *connectionTail;
    int connectionNumber;
};

typedef struct ConnectionWrapperStruct ConnectionWrapper;

typedef struct SocketWrapperStruct SocketWrapper;

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

#endif 
