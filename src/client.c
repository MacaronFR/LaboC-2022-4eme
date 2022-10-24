#include <client.h>

void client() {
	char buffer[256];
	ssize_t n;
	int port = 2022;
	fd_set rdfs;
	sock srvSock = initClient("127.0.0.1", port);
	while (1) {
		memset(buffer, 0, 256);
		FD_ZERO(&rdfs);
		FD_SET(STDIN_FILENO, &rdfs);
		FD_SET(srvSock, &rdfs);
		if (select(srvSock + 1, &rdfs, NULL, NULL, NULL) < 0) {
			perror("select");
			return;
		}
		if (FD_ISSET(STDIN_FILENO, &rdfs)) {
			fgets(buffer, 255, stdin);
			buffer[255] = 0;
			write(srvSock, buffer, strlen(buffer));
		} else if (FD_ISSET(srvSock, &rdfs)) {
			n = read(srvSock, buffer, 255);
			if (n == 0) {
				printf("Server disconnected\n");
				close(srvSock);
				return;
			}
			puts(buffer);
		}
	}
}