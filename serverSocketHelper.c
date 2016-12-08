#include "serverSocketHelper.h"

SocketWrapper *initSocket(char *port, int queue) {
    SocketWrapper *sw = (SocketWrapper *)malloc(sizeof(SocketWrapper));

    struct addrinfo hints, *result;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(NULL, port, &hints, &result);

    //TODO Result might have several address, need to bind it all
    int sockfd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (bind(sockfd, result->ai_addr, result->ai_addrlen) == -1) {
	    perror("bind");
        return NULL;
	}

	if (listen(sockfd, queue) == -1) {
	    perror("listen");
        return NULL;
	}

    if (SELECT_STATUS) {
        sw->fs = (fd_set *)malloc(sizeof(fd_set));
        FD_ZERO(sw->fs);
    }

    if (EPOLL_STATUS) {
        sw->epollfd = epoll_create(EPOLL_DEFAULT_FDSIZE);
    }

    sw->fd = sockfd;
    sw->maxfd = -1;
    sw->connectionNumber = 0;
    sw->connectionHead = NULL;
    sw->connectionTail = NULL;
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
        socket->connectionHead = newConnection;
        socket->connectionTail = newConnection;
    }
    else {
        socket->connectionTail->next = newConnection;
        socket->connectionTail = newConnection;
    }
    socket->connectionNumber++;
    if (tmp > socket->maxfd) socket->maxfd = tmp;

    return newConnection;
}

int sendString(ConnectionWrapper *connection, char *str) {
    ssize_t succLen;
    succLen = send(connection->fd, str, strlen(str), 0);
	return (int)succLen;
} 

int recvString(ConnectionWrapper *connection, char *str, int len) {
    ssize_t succLen;
    succLen = recv(connection->fd, str, (size_t)len, 0);
    return (int)succLen;
}

int sendBinaries(ConnectionWrapper *connection, unsigned char *bytes) {
    //TODO
    return 0;
}

//TODO The address is host byte order, but we want network byte order 
char *getPeerName(ConnectionWrapper *connection) {
    struct sockaddr *thisAddr;
    int addrlen = sizeof(struct sockaddr);
    getpeername(connection->fd, thisAddr, &addrlen);
    if (thisAddr->sa_family == AF_INET) {
        char *ipv4addr = (char *)malloc(sizeof(char) * INET_ADDRSTRLEN);
        inet_ntop(AF_INET, thisAddr, ipv4addr, INET_ADDRSTRLEN);
        return ipv4addr;
    }
    else {
        char *ipv6addr = (char *)malloc(sizeof(char) * INET6_ADDRSTRLEN);
        inet_ntop(AF_INET6, thisAddr, ipv6addr, INET6_ADDRSTRLEN);
        return ipv6addr;
    }
}

char *getHostName(int len) {
    char *hostname = (char *)malloc(sizeof(char) * len);
    gethostname(hostname, (size_t)len); 
    return hostname;
}

void shutdownConnection(ConnectionWrapper *connection, int status) {
    shutdown(connection->fd, status);
}

void closeSocket(SocketWrapper *socket) {
	close(socket->fd);
    free(socket);
}

void closeConnection(ConnectionWrapper *connection) {
    //TODO Delete it in the list
	close(connection->fd);
    free(connection);
}

SelectResult *selectReadyConnections(SocketWrapper *socket, struct timeval *timeout) {
    ConnectionWrapper *tmp, *last;

    FD_ZERO(socket->fs);
    tmp = socket->connectionHead;
    while (tmp != NULL) {
        FD_SET(tmp->fd, socket->fs);
        tmp = tmp->next;
    }

    int fd = select(socket->maxfd + 1, socket->fs, socket->fs, 0, timeout);

    if (fd == -1) {
        return NULL;
    }

    SelectResult *result = (SelectResult *)malloc(sizeof(SelectResult));
    result->num = 0;
    result->connectionHead = NULL;

    if (fd == 0) {
        return result;
    }

    tmp = socket->connectionHead;
    while (tmp != NULL) {
        if (FD_ISSET(tmp->fd, socket->fs)) {
            result->num++;
            
            tmp->nextSelect = NULL;
            if (result->connectionHead == NULL) {
                result->connectionHead = tmp;
            }
            else {
                last->nextSelect = tmp;
            }
            last = tmp;
        }
        tmp = tmp->next;
    }
    
    return result;
}

extern void addEpollEvent(SocketWrapper *socket, ConnectionWrapper *connection, EpollEvents *events) {
    epoll_ctl(socket->epollfd, EPOLL_CTL_ADD, connection->fd, events->events);
}

extern void deleteEpollEvent(SocketWrapper *socket, ConnectionWrapper *connection, EpollEvents *events) {
    epoll_ctl(socket->epollfd, EPOLL_CTL_DEL, connection->fd, events->events);
}

extern void modifyEpollEvent(SocketWrapper *socket, ConnectionWrapper *connection, EpollEvents *events) {
    epoll_ctl(socket->epollfd, EPOLL_CTL_DEL, connection->fd, events->events);
}

extern EpollEvents *epollWait(SocketWrapper *socket, int maxevents, int timeout) {
    EpollEvents *result = (EpollEvents *)malloc(sizeof(EpollEvents));
    
    result->num = epoll_wait(socket->epollfd, result->events, maxevents, timeout);
    return result;
}

