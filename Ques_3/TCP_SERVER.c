#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

int main() {
    int sockfd, newsock;
    struct sockaddr_in serverAddr, clientAddr;
    char buffer[1024];
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) { perror("Socket error"); exit(1); }
    
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    
    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind error"); exit(1);
    }
    
    listen(sockfd, 10);
    printf("Server listening for clients...\n");
    
    socklen_t addr_size;
    pid_t childpid;
    
    while (1) {
        addr_size = sizeof(clientAddr);
        newsock = accept(sockfd, (struct sockaddr*)&clientAddr, &addr_size);
        if (newsock < 0) { perror("Accept error"); continue; }

        printf("Client connected from %s:%d\n", inet_ntoa(clientAddr.sin_addr),
        ntohs(clientAddr.sin_port));


        if ((childpid = fork()) == 0) {
            close(sockfd);
            while (1) {
                memset(buffer, 0, sizeof(buffer));
                int n = recv(newsock, buffer, sizeof(buffer), 0);
                if (n <= 0) break;
                printf("Client [%s:%d]: %s\n",
                        inet_ntoa(clientAddr.sin_addr),
                        ntohs(clientAddr.sin_port),
                        buffer);
                send(newsock, buffer, strlen(buffer), 0); // echo back
            }
            close(newsock);
            exit(0);
        }
        close(newsock);
    }
    return 0;
}
