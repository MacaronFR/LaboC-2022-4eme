#ifndef SOCKET_SHARED_H
#define SOCKET_SHARED_H

#include <stdio.h>
#include <stdlib.h>

void sigPipeHandle();
void sendAll(int *clients, int sender, int n, char *buffer, ssize_t size);

#endif //SOCKET_SHARED_H
