#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HOSTNAME "\n[Hostname]\n"
#define HORA_UPTIME "\n\n[Hora/Up Time]\n"
#define ENDERECO_IPV6 "\n\n[Endereco IPV6]\n"
#define ENDERECO_IPV4 "\n\n[Endereco IPV4]\n"
#define ESPACO_EM_DISCO "\n\n[Espaco em disco]\n"

#define READ "r"

#define CMD_HOSTNAME "hostname"
#define CMD_HORA_UPTIME "w | head -1"
#define CMD_ENDERECO_IPV6 "ifconfig | grep \"inet6 addr\" | sed -e 's/^[ \t]*//'"
#define CMD_ENDERECO_IPV4 "ifconfig | grep \"inet addr\" | sed -e 's/^[ \t]*//'"
#define CMD_ESPACO_EM_DISCO "df -kh"

#define MAX_SIZE 1024

/** Retira os dados invalidos do final do buffer */
void beautifyBuffer(char *buffer) {
	int i = 0;

	for (i = strlen(buffer) - 1; i >= 0; i--) {
		if (buffer[i] == '\n') {
			buffer[i] = '\0';
			break;
		}
	}
}

/** Preenche o buffer */
void fillBuffer(char *buffer, char *cmd, char *output_text) {
	FILE * fp = popen(cmd, READ);
	memcpy(&(buffer[strlen(buffer)]), output_text, strlen(output_text) + 1);
	fread(&(buffer[strlen(buffer)]), sizeof(buffer), 1, fp);
	fgets(&(buffer[strlen(buffer)]), 1024, fp);
	beautifyBuffer(buffer);
}

/** Salva o titulo do comando */
void saveTitle(char *buffer, char *output_text) {
	memcpy(&(buffer[strlen(buffer)]), output_text, strlen(output_text) + 1);
}

/**
 *  Procedimento principal que retorna as informacoes do sistema na seguinte ordem
 *  :1 hostname
 *  :2 hora_uptime
 *  :3 endereco ipv6
 *  :4 endereco ipv4
 *  :5 espaco em disco
 */

char *getInfo() {
	char *buffer = malloc(MAX_SIZE);
	buffer[0] = '\0';

	// Salva as informacoes do hostname no buffer
	FILE * fp = popen(CMD_HOSTNAME, READ);
	memcpy(buffer, HOSTNAME, strlen(HOSTNAME) + 1);
	fread(&buffer[strlen(buffer)], 1024, 1, fp);
	fgets(&buffer[strlen(buffer)], MAX_SIZE, fp);
	beautifyBuffer(buffer);

	// Salva as informacoes de hora_uptime no buffer
	fp = popen(CMD_HORA_UPTIME, READ);
	memcpy(&buffer[strlen(buffer)], HORA_UPTIME, strlen(HORA_UPTIME) + 1);
	fread(&buffer[strlen(buffer)], 1024, 1, fp);
	fgets(&buffer[strlen(buffer)], MAX_SIZE, fp);
	beautifyBuffer(buffer);

	// Salva as informacoes de ipv6 no buffer
	fp = popen(CMD_ENDERECO_IPV6, READ);
	memcpy(&buffer[strlen(buffer)], ENDERECO_IPV6, strlen(ENDERECO_IPV6) + 1);
	fread(&buffer[strlen(buffer)], 1024, 1, fp);
	fgets(&buffer[strlen(buffer)], MAX_SIZE, fp);
	beautifyBuffer(buffer);

	// Salva as informacoes de ipv4 no buffer
	fp = popen(CMD_ENDERECO_IPV4, READ);
	memcpy(&buffer[strlen(buffer)], ENDERECO_IPV4, strlen(ENDERECO_IPV4) + 1);
	fread(&buffer[strlen(buffer)], 1024, 1, fp);
	fgets(&buffer[strlen(buffer)], MAX_SIZE, fp);
	beautifyBuffer(buffer);

	// Salva as informacoes de espaco em disco no buffer
	fp = popen(CMD_ESPACO_EM_DISCO, READ);
	memcpy(&buffer[strlen(buffer)], ESPACO_EM_DISCO, strlen(ESPACO_EM_DISCO) + 1);
	fread(&buffer[strlen(buffer)], 1024, 1, fp);
	fgets(&buffer[strlen(buffer)], MAX_SIZE, fp);
	beautifyBuffer(buffer);

	return buffer;
}
