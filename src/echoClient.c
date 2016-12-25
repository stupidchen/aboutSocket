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


#define MYPORT 8887
#define BUFFER_SIZE 1024
#define SERVER_ADDR "112.74.33.156"

int main(void) {
	int client_sockfd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in client_sockaddr;
	client_sockaddr.sin_family = AF_INET;
	client_sockaddr.sin_port = htons(MYPORT);
	client_sockaddr.sin_addr.s_addr = inet_addr(SERVER_ADDR);

	if (connect(client_sockfd, (struct sockaddr *)&client_sockaddr, sizeof(client_sockaddr)) == -1) {
		perror("connect");
		exit(1);
	}

	char sendbuf[BUFFER_SIZE];
	char recvbuf[BUFFER_SIZE];
	while (fgets(sendbuf, sizeof(sendbuf), stdin) != NULL) {
		send(client_sockfd, sendbuf, strlen(sendbuf), 0);
		if (strcmp(sendbuf, "exit\n") == 0) {
			break;
		}
		long long len = recv(client_sockfd, recvbuf, sizeof(recvbuf), 0);
        recvbuf[len + 1] = '\0';
		fputs(recvbuf, stdout);

		memset(sendbuf, 0, sizeof(sendbuf));
		memset(recvbuf, 0, sizeof(recvbuf));
	}

	close(client_sockfd);
	return 0;
}
