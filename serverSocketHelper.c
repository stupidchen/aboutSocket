#include "serverSocketHelper.h"

SocketWrapper *initSocket(char *port, int queue) {
    SocketWrapper *sw = (SocketWrapper *)malloc(sizeof(SocketWrapper));

    struct addrinfo hints, *result;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(NULL, port, &hints, &result);

    int sockfd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (bind(sockfd, result->ai_addr, result->ai_addrlen) == -1) {
		perror("bind");
        return NULL;
	}

	if (listen(sockfd, queue) == -1) {
		perror("listen");
        return NULL;
	}

    sw->fd = sockfd;
    sw->connectionNumber = 0;
	return sw;
}

ConnectionWrapper *acceptOneConnection(SocketWrapper *socket) {
    ConnectionWrapper *newConnection = (ConnectionWrapper *)malloc(sizeof(ConnectionWrapper));
    newConnection->addrlen = sizeof(newConnection->address);

    int tmp = accept(socket->fd, &(newConnection->address), &(newConnection->addrlen));
    if (tmp == -1) {
        return NULL;
    }

    newConnection->fd = tmp;
    newConnection->next = NULL;
    if (socket->connectionHead == NULL) {
        socket->connectionHead = (ConnectionWrapper *)malloc(sizeof(ConnectionWrapper *));
        socket->connectionHead = newConnection;
        socket->connectionTail = newConnection;
    }
    else {
        socket->connectionTail->next = (ConnectionWrapper *)malloc(sizeof(ConnectionWrapper *));
        socket->connectionTail = newConnection;
    }

    return newConnection;
}

int acceptConnections(SocketWrapper *socket, int num) {
    return num;
}

int sendString(ConnectionWrapper *connection, char *str) {
    int succLen;
    succLen = send(connection->fd, str, strlen(str), 0);
	return succLen;
} 

int sendBinaries(ConnectionWrapper *connection, unsigned char *bytes) {
    return 0;
}

int closeSocket(SocketWrapper *socket) {
	close(socket->fd);
    free(socket);
}

int closeConnection(ConnectionWrapper *connection) {
	close(connection->fd);
    free(connection);
}
