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
}

#endif
