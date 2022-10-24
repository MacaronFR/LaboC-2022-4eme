#include <complexe.h>

int complexe(){
    int sock, port, max, actual = 0;
    int client[5];
    fd_set rdfs;
    ssize_t n;
    char buffer[256];
    struct sockaddr_in srv_addr;
    signal(SIGPIPE, sigPipeHandle);
    max = sock = socket(AF_INET, SOCK_STREAM, 0);
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
    if(listen(sock, 5) < 0){
        perror("listen");
        return -1;
    }
    while(1) {
        FD_ZERO(&rdfs);
        FD_SET(STDIN_FILENO, &rdfs);
        FD_SET(sock, &rdfs);
        for (int i = 0; i < actual; ++i) {
            FD_SET(client[i], &rdfs);
        }
        if (select(max + 1, &rdfs, NULL, NULL, NULL) == -1) {
            perror("select");
            return -1;
        }
        if (FD_ISSET(STDIN_FILENO, &rdfs)) {
            break;
        } else if (FD_ISSET(sock, &rdfs)) {
            struct sockaddr_in csin = {0};
            unsigned int sinsize = sizeof(csin);
            int csock = accept(sock, (struct sockaddr *) &csin, &sinsize);
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
                        for(int j = i; j < actual - 1; ++j){
                            client[j] = client[j+1];
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
    for(int i = 0; i < actual; ++i){
        close(client[i]);
    }
    close(sock);
    return 1;
}