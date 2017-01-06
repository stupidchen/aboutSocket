#include "include/clientSocketHelper.h"

extern SocketWrapper *initSocket(char *address, int port) {
    SocketWrapper *sw = (SocketWrapper *)malloc(sizeof(SocketWrapper));

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in client_sockaddr;
	client_sockaddr.sin_family = AF_INET;
	client_sockaddr.sin_port = htons(port);
	client_sockaddr.sin_addr.s_addr = inet_addr(address);

	if (connect(sockfd, (struct sockaddr *)&client_sockaddr, sizeof(client_sockaddr)) == -1) {
	    perror("connect");
	    exit(1);
	}

    sw->address = client_sockaddr;
    sw->fd = sockfd;

    return sw;
}

//Check: The return value of send() is ssize_t(aka long)
//Block 
extern int sendString(SocketWrapper *socket, char *str) {
    return send(socket->fd, str, strlen(str), 0);
}

extern int aSendString(SocketWrapper *socket, char *str, void *callback) {
    //TODO
    return 0;
}

//TODO Reduce the receive buffer size
//Block 
extern char *recvString(SocketWrapper *socket, size_t len) {
    long long recvlen;
    char *recvbuf;

    if (len == 0) len = DEFAULT_BUFFER_SIZE;

    recvbuf = (char *)malloc(sizeof(char) * len);
    recvlen = recv(socket->fd, recvbuf, len, 0);
    recvbuf[recvlen] = '\0';
    return recvbuf;
}

extern char *aRecvString(SocketWrapper *socket, size_t len, void *callback) {
    //TODO
    return 0;
}

extern void closeSocket(SocketWrapper *socket) {
    close(socket->fd);
    free(socket);
}
