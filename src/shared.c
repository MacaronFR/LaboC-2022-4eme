#include <shared.h>
#include <arpa/inet.h>

void sigPipeHandle() {
	perror("Client abort connection");
	exit(-1);
}

void sendAll(int *clients, int sender, int n, char *buffer, ssize_t size) {
	for (int i = 0; i < n; ++i) {
		if (clients[i] != sender) {
			write(clients[i], buffer, size);
		}
	}
}

sock initSock() {
	sock sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("Socket");
		exit(-1);
	}
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &(int) {1}, sizeof(int));
	return sock;
}

struct sockaddr_in *initSrvAddr(int port) {
	struct sockaddr_in *srv = malloc(sizeof(struct sockaddr_in));
	memset(srv, 0, sizeof(struct sockaddr_in));
	srv->sin_family = AF_INET;
	srv->sin_addr.s_addr = INADDR_ANY;
	srv->sin_port = htons(port);
	return srv;
}

struct sockaddr_in *initSrvAddrClient(const char *address, int port) {
	struct sockaddr_in *srv = malloc(sizeof(struct sockaddr_in));
	memset(srv, 0, sizeof(struct sockaddr_in));
	srv->sin_family = AF_INET;
	srv->sin_addr.s_addr = inet_addr(address);
	srv->sin_port = htons(port);
	return srv;
}

sock initSrv(int port, int maxClient) {
	sock socket = initSock();
	struct sockaddr_in *addr = initSrvAddr(port);
	if (bind(socket, (struct sockaddr *) addr, sizeof(struct sockaddr_in)) < 0) {
		perror("Bind");
		free(addr);
		exit(-1);
	}
	free(addr);
	if (listen(socket, maxClient) < 0) {
		perror("Listen");
		exit(-1);
	}
	return socket;
}

sock initClient(const char *address, int port) {
	sock socket = initSock();
	struct sockaddr_in *addr = initSrvAddr(port);
	if (connect(socket, (struct sockaddr *) addr, sizeof(struct sockaddr_in)) < 0) {
		perror("Connect");
		free(addr);
		exit(-1);
	}
	free(addr);
	return socket;
}