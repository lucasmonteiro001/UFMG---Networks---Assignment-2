#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "utilities.h"

int main(int argc, char *argv[]) {

	if (argc < 2) {
		fprintf(stderr, "uso %s <porta>\n", argv[0]);
		exit(0);
	}

	int socket_monitor;
	socklen_t clilen;
	struct sockaddr_in6 server_addr, client_addr;
	char addrbuf[INET6_ADDRSTRLEN];

	// cria o socket do tipo ivp6/udp
	socket_monitor = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);

	if (socket_monitor < 0) {
		perror("erro ao criar o socket");
		exit(1);
	}

	memset(&server_addr, 0, sizeof(server_addr));

	server_addr.sin6_family = AF_INET6;
	server_addr.sin6_addr = in6addr_any;
	server_addr.sin6_port = htons(atoi(argv[1]));

	if (bind(socket_monitor, (struct sockaddr *) &server_addr,
			sizeof(server_addr)) < 0) {
		perror("bind falhou");
		exit(2);
	}

	while (1) {
		char buffer[1024];

		clilen = sizeof(client_addr);
		if (recvfrom(socket_monitor, buffer, 1024, 0,
				(struct sockaddr *) &client_addr, &clilen) < 0) {
			perror("recvfrom falhou");
			exit(4);
		}

		// So responde a request se ela for do formato
		// 'request' e tiver tamanho 7
		if (buffer[0] == 'r' && buffer[1] == 'e' && buffer[2] == 'q'
				&& buffer[3] == 'u' && buffer[4] == 'e' && buffer[5] == 's'
				&& buffer[6] == 't' && strlen(buffer) == 7) {

			printf(
					"mensagem recebida '%s' do socket %s\n",
					buffer,
					inet_ntop(AF_INET6, &client_addr.sin6_addr, addrbuf,
							INET6_ADDRSTRLEN));

			printf("enviando a resposta...\n");

			// Constroi a mensagem que sera enviada ao cliente,
			// tal mensagem contem informacoes do sistema
			char *s = getInfo();

			if (sendto(socket_monitor, s, 1024, 0,
					(struct sockaddr *) &client_addr, sizeof(client_addr)) < 0) {
				perror("sendto falhou");
				exit(5);
			}

			s = NULL;
			printf("resposta enviada\n");
		}


	}

	return 0;
}
