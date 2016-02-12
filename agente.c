#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc, char *argv[]) {

	if (argc < 3) {
		fprintf(stderr, "uso %s <ipv6> <porta>\n", argv[0]);
		exit(0);
	}

	int socket_agente;
	socklen_t clilen;
	struct sockaddr_in6 server_addr, client_addr;
	char buffer[1024];

	socket_agente = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);

	if (socket_agente < 0) {
		perror("erro ao criar socket");
		exit(1);
	}

	(&server_addr, 0, sizeof(server_addr));

	server_addr.sin6_family = AF_INET6;
	inet_pton(AF_INET6, argv[1], &server_addr.sin6_addr);
	server_addr.sin6_port = htons(atoi(argv[2]));

	char request[7];
	request[0] = 'r';
	request[1] = 'e';
	request[2] = 'q';
	request[3] = 'u';
	request[4] = 'e';
	request[5] = 's';
	request[6] = 't';

	// Envia a mensagem 'REQUEST' para o monitor
	if (sendto(socket_agente, request, sizeof(request), 0,
			(struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
		perror("sendto falhou");
		exit(4);
	}

	clilen = sizeof(client_addr);
	if (recvfrom(socket_agente, buffer, 1024, 0,
			(struct sockaddr *) &client_addr, &clilen) < 0) {
		perror("recvfrom falhou");
		exit(4);
	}

	// Imprima mensagem recebida do monitor
	printf("%s\n\n", buffer);

	close(socket_agente);

	return 0;
}
