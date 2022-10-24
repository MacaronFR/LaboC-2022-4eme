#include <shared.h>

void sigPipeHandle(){
    perror("Client abort connection");
    exit(-1);
}

void sendAll(int *clients, int sender, int n, char *buffer, ssize_t size){
    for(int i = 0; i < n; ++i){
        if(clients[i] != sender) {
            write(clients[i], buffer, size);
        }
    }
}