#include <complexe.h>

int complexe() {
	sock srvSock;
	int port, max, actual = 0;
	int client[5];
	fd_set rdfs;
	ssize_t n;
	char buffer[256];
	signal(SIGPIPE, sigPipeHandle);
	port = 2022;
	max = srvSock = initSrv(port, 5);
	while (1) {
		FD_ZERO(&rdfs);
		FD_SET(STDIN_FILENO, &rdfs);
		FD_SET(srvSock, &rdfs);
		for (int i = 0; i < actual; ++i) {
			FD_SET(client[i], &rdfs);
		}
		if (select(max + 1, &rdfs, NULL, NULL, NULL) == -1) {
			perror("select");
			return -1;
		}
		if (FD_ISSET(STDIN_FILENO, &rdfs)) {
			break;
		} else if (FD_ISSET(srvSock, &rdfs)) {
			struct sockaddr_in csin = {0};
			unsigned int sinsize = sizeof(csin);
			int csock = accept(srvSock, (struct sockaddr *) &csin, &sinsize);
			if (csock < 0) {
				perror("accept");
				continue;
			}
			printf("Client connected\n");
			max = csock > max ? csock : max;
			FD_SET(csock, &rdfs);
			client[actual] = csock;
			actual++;
			sendAll(client, -1, actual, "Client connected\n", 18);
		} else {
			for (int i = 0; i < actual; ++i) {
				if (FD_ISSET(client[i], &rdfs)) {
					n = read(client[i], buffer, 256);
					if (n == 0) {
						close(client[i]);
						for (int j = i; j < actual - 1; ++j) {
							client[j] = client[j + 1];
						}
						actual--;
						client[actual] = 0;
						sendAll(client, -1, actual, "Un client c'est déconnecté\n", 29);
					} else {
						puts(buffer);
						sendAll(client, client[i], actual, buffer, n);
					}
					break;
				}
			}
		}
	}
	for (int i = 0; i < actual; ++i) {
		close(client[i]);
	}
	close(srvSock);
	return 1;
}