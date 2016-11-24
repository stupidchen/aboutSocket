#ifndef SERVER_SOCKET_HELPER
#define SERVER_SOCKET_HELPER

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>

#define DEFAULT_PORT 8887
#define DEFAULT_QUEUE 20
#define DEFAULT_BUFFER_SIZE 1024

//init:Bind, listen
int initSocket(int port, int queue, int bufferSize);

int sendString(int conn, char *string);

int closeSocket(int socketfd); 

int closeConnection(int conn);

struct socketWrapperStruct {
	int fd;
	char *buffer;
	int *connections;
};

typedef socketWrapperStruct socketWrapper;

#endif 
