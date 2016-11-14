#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
	/*
	struct sockaddr_in sa;
	struct sockaddr_in6 sa6;

	inet_pton(AF_INET, "10.12.110.57:80", &(sa.sin_addr));
	inet_pton(AF_INET6, "2001:db8:63b3:1::3490", &(sa6.sin6_addr));

	int status;
	struct addrinfo hints;
	struct addrinfo *servinfo;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if ((status = getaddrinfo("www.baidu.com", "80", &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
		return 2;
	}
	*/

	int sockfd;
	struct addrinfo hints, *result;

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(NULL, "3490", &hints, &result);

	sockfd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	printf("%d\n", sockfd);
	/*
	int num = 0;
	while (servinfo != NULL) {
		printf("node %d:\n", ++num);
		printf("ai_flags: %d\n", servinfo->ai_flags);
		printf("ai_family: %d\n", servinfo->ai_family);
		printf("ai_socktype: %d\n", servinfo->ai_socktype);
		printf("ai_protocol: %d\n", servinfo->ai_protocol);
		printf("ai_addrlen: %d\n", servinfo->ai_addrlen);

		char paddr[INET_ADDRSTRLEN];
		struct sockaddr_in *tmp = (struct sockaddr_in *)servinfo->ai_addr;

		inet_ntop(AF_INET, &(tmp->sin_addr), paddr, INET_ADDRSTRLEN);
		printf("%s\n", paddr);
		// printf("%s\n", servinfo->ai_canonname);
		servinfo = servinfo->ai_next;
	}

	freeaddrinfo(servinfo);
	*/

	return 0;
}
