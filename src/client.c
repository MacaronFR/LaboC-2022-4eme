#include <client.h>

void client(){
    char buffer[256];
    struct sockaddr_in srv_addr;
    ssize_t n;
    int port = 2022;
    fd_set rdfs;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0){
        perror("socket");
        return;
    }
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(port);
    srv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if(connect(sock, (struct sockaddr *)&srv_addr, sizeof(srv_addr)) < 0){
        perror("connect");
        return;
    }
    while (1){
        memset(buffer, 0, 256);
        FD_ZERO(&rdfs);
        FD_SET(STDIN_FILENO, &rdfs);
        FD_SET(sock, &rdfs);
        if(select(sock + 1, &rdfs, NULL, NULL, NULL) < 0){
            perror("select");
            return;
        }
        if(FD_ISSET(STDIN_FILENO, &rdfs)){
            fgets(buffer, 255, stdin);
            buffer[255] = 0;
            write(sock, buffer, strlen(buffer));
        }else if(FD_ISSET(sock, &rdfs)){
            n = read(sock, buffer, 255);
            if(n == 0){
                printf("Server disconnected\n");
                close(sock);
                return;
            }
            puts(buffer);
        }
    }
}