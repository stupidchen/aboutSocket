#ifndef CLIENT_SOCKET_HELPER_H
#define CLIENT_SOCKET_HELPER_H

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h> 

#include <sys/shm.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define DEFAULT_BUFFER_SIZE 1024

struct SocketWrapperStruct {
    int fd; 
    struct sockaddr_in address;

    char sendbuf[DEFAULT_BUFFER_SIZE];
    char recvbuf[DEFAULT_BUFFER_SIZE];
};

typedef struct SocketWrapperStruct SocketWrapper;

extern SocketWrapper *initSocket(char *address, int port);

extern long sendString(SocketWrapper *socket, char *str);

extern int aSendString(SocketWrapper *socket, char *str, void *callback); 

extern char *recvString(SocketWrapper *socket, size_t len);

extern char *aRecvString(SocketWrapper *socket, size_t len, void *callback);

extern void closeSocket(SocketWrapper *socket);

#endif


