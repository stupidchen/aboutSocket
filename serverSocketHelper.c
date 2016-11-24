#include "serverSocketHelper.h"

int initSocket(int port, int queue, int bufferSize) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(port);
	sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sockfd, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) == -1) {
		perror("bind");
		return -1;
	}

	if (listen(sockfd, queue) == -1) {
		perror("listen");
		return -1;
	}

	return sockfd;
}

int sendString(int conn, char *string) {
	return -1;
} 

int closeSocket(int socketfd) {
	close(socketfd);
}

int closeConnection(int conn) {
	close(conn);
}
