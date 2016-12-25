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

struct SocketWrapperStruct {
    int fd; 
    struct sockaddr address;
    socklen_t addrlen;
};

typedef SocketWrapperStruct SocketWrapper;

extern SocketWrapper *initSocket(char *address, char *port);

extern int sendString(SocketWrapper *socket, char *str);

extern int aSendString(SocketWrapper *socket, char *str, void *callback); 

extern int recvString(SocketWrapper *socket, char *str, int len);

extern int aRecvString(SocketWrapper *socket, char *str, void *callback);

extern void closeSocket(SocketWrapper *socket);

#endif


