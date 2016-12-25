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
        if (sw->fs == NULL) {
            perror("Select malloc");
        }
        else {
            FD_ZERO(sw->fs);
        }
    }

    if (EPOLL_STATUS) {
        int tmpfd0 = epoll_create(EPOLL_DEFAULT_FDSIZE); int tmpfd1 = epoll_create(EPOLL_DEFAULT_FDSIZE);
        
        if (tmpfd0 == -1 || tmpfd1 == -1) {
            perror("Epoll create");
        }
        else {
            sw->efdGroupNum = 2;
            sw->efdMaxGroupNum = EPOLL_DEFAULT_GROUP_NUMBER;
            sw->efds = (int *)malloc(sizeof(int) * EPOLL_DEFAULT_GROUP_NUMBER);
            
            sw->efds[EPOLL_LISTEN_GROUP] = tmpfd0;
            sw->efds[EPOLL_BOARDCAST_GROUP] = tmpfd1;
            
            addEpollEventByFd(sw, EPOLL_LISTEN_GROUP, sockfd, createEpollEvent(sw, EPOLLIN | EPOLLET));
        }
    }

    sw->fd = sockfd;
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
    newConnection->last = NULL;
    newConnection->socket = socket;
    if (socket->connectionHead == NULL) {
        socket->connectionHead = newConnection;
        socket->connectionTail = newConnection;
    }
    else {
        newConnection->last = socket->connectionTail;
        socket->connectionTail->next = newConnection;
        socket->connectionTail = newConnection;
    }
    socket->connectionNumber++;

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
    str[succLen + 1] = '\0';
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
    int i;

	close(socket->fd);
    if (EPOLL_STATUS) {
        for (i = 0; i < socket->fdGroupNumber; i++) {
            close(socket->efds[i]);
        }
    }

    free(socket);
}

//TODO Check, the deletion will affect the link struct
void closeConnection(ConnectionWrapper *connection) {
	close(connection->fd);

    connection->socket->connectionNumber--;

    if (connection->last == NULL) {
        connection->socket->connectionHead = connection->next;
    }
    else {
        connection->last->next = connection->next;
    }

    if (connection->next == NULL) {
        connection->socket->connectionTail = connection->last;
    }
    else {
        connection->next->last = connection->last;
    }

    free(connection);
}

SelectResult *selectReadyConnections(SocketWrapper *socket, struct timeval *timeout) {
    ConnectionWrapper *tmp, *last;
    int maxfd = -1;

    FD_ZERO(socket->fs);
    tmp = socket->connectionHead;
    while (tmp != NULL) {
        FD_SET(tmp->fd, socket->fs);
        if (tmp->fd > maxfd) maxfd = tmp->fd;
        tmp = tmp->next;
    }

    int fd = select(maxfd + 1, socket->fs, NULL, 0, timeout);

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

extern EpollEvent *createEpollEvent(void *connection, __uint32_t status) {
    EpollEvent *event = (EpollEvent *)malloc(sizeof(EpollEvent));
    event->data.ptr = connection;
    event->events = status;
    return event;
}

extern EpollEvent *createEpollEventByFd(int fd,  __uint32_t status) {
    EpollEvent *event = (EpollEvent *)malloc(sizeof(EpollEvent));
    event->data.fd = fd;
    event->events = status;
    return event;
}

extern EpollEvents *createEpollEvents(unsigned long size) {
    EpollEvents *newEvents = (EpollEvents *)malloc(sizeof(EpollEvents));
    newEvents->events = (EpollEvent *)malloc(sizeof(EpollEvent) * size);
    newEvents->num = 0;
    return newEvents;
}

extern void addEpollEvent(SocketWrapper *socket, int group, ConnectionWrapper *connection, EpollEvent *event) {
    epoll_ctl(socket->efds[group], EPOLL_CTL_ADD, connection->fd, event);
}

extern void addEpollEventByFd(SocketWrapper *socket, int group, int fd, EpollEvent *event) {
    epoll_ctl(socket->efds[group], EPOLL_CTL_ADD, fd, event);
}

extern void deleteEpollEvent(SocketWrapper *socket, int group, ConnectionWrapper *connection, EpollEvent *event) {
    epoll_ctl(socket->efds[group], EPOLL_CTL_DEL, connection->fd, event);
}

extern void deleteEpollEventByFd(SocketWrapper *socket, int group, int fd, EpollEvent *event) {
    epoll_ctl(socket->efds[group], EPOLL_CTL_DEL, fd, event);
}

extern void modifyEpollEvent(SocketWrapper *socket, int group, ConnectionWrapper *connection, EpollEvent *event) {
    epoll_ctl(socket->efds[group], EPOLL_CTL_MOD, connection->fd, event);
}

extern void modifyEpollEventByFd(SocketWrapper *socket, int group, int fd, EpollEvent *event) {
    epoll_ctl(socket->efds[group], EPOLL_CTL_MOD, fd, event);
}

extern void epollWait(SocketWrapper *socket, int group, EpollEvents *result, int maxevents, int timeout) {
    result->num = epoll_wait(socket->efds[group], result->events, maxevents, timeout);
}

extern int getFdFromEvent(EpollEvent event) {
    return event.data.fd;
}

extern void *getPtrFromEvent(EpollEvent event) {
    return event.data.ptr;
}
