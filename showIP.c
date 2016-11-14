#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "Invalid argument.\n");
		return 1;
	}

	int status;
	struct addrinfo hints, *res, *t;
	char ipv4str[INET_ADDRSTRLEN], ipv6str[INET6_ADDRSTRLEN];

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if ((status = getaddrinfo(*(argv + 1), NULL, &hints, &res)) != 0) {
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
		return 2;
	}

	for (t = res; t != NULL; t = t->ai_next) {
		void *addr;
		char *ipver;

		if (t->ai_family == AF_INET) {
			struct sockaddr_in *tmp = (struct sockaddr_in *)t->ai_addr;
			addr = &(tmp->sin_addr);
			ipver = "IPv4";
			inet_ntop(AF_INET, addr, ipv4str, INET_ADDRSTRLEN);
			printf("%s: %s\n", ipver, ipv4str);
		}
		else {
			struct sockaddr_in *tmp = (struct sockaddr_in6 *)t->ai_addr;
			addr = &(tmp->sin_addr);
			ipver = "IPv6";
			inet_ntop(AF_INET6, addr, ipv4str, INET6_ADDRSTRLEN);
			printf("%s: %s\n", ipver, ipv6str);
		}
	}

	freeaddrinfo(res);
	return 0;
}
