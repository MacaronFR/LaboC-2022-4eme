#ifndef SOCKET_SHARED_H
#define SOCKET_SHARED_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

typedef int sock;

void sigPipeHandle();

void sendAll(int *clients, int sender, int n, char *buffer, ssize_t size);

sock initSock();

struct sockaddr_in *initSrvAddr(int port);

struct sockaddr_in *initSrvAddrClient(const char *address, int port);

sock initSrv(int port, int maxClient);

sock initClient(const char *address, int port);

#endif //SOCKET_SHARED_H
