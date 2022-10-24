#include <simple.h>

int simple(){
    int sock, client, port;
    ssize_t n;
    unsigned int clilen;
    char buffer[256];
    struct sockaddr_in srv_addr, cli_addr;
    signal(SIGPIPE, sigPipeHandle);
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0){
        perror("socket");
        return 0;
    }
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));
    memset(&srv_addr, 0, sizeof(srv_addr));
    port = 2022;
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr.s_addr = INADDR_ANY;
    srv_addr.sin_port = htons(port);
    if(bind(sock, (struct sockaddr*)&srv_addr, sizeof(srv_addr)) < 0){
        perror("bind");
        return 0;
    }
    listen(sock, 1);
    clilen = sizeof(cli_addr);
    client = accept(sock, (struct sockaddr*)&cli_addr, &clilen);
    printf("Client connected\n");
    memset(buffer, 0, 256);
    while(strncmp("quit", buffer, 4) != 0){
        n = read(client, buffer, 255);
        buffer[n] = 0;
        if(strncmp("quit", buffer, 4) == 0){
            write(client, "quit", 5);
            printf("Client ask for disconnection\n");
        }else {
            printf("%s", buffer);
            write(client, "Message reçu\n", 14);
        }
    }
    printf("Client disconnected\n");
    close(sock);
    return 1;
}