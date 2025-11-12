#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define SIZE 1024

int main() {
    int sockfd, newsock;
    char buffer[SIZE];
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addr_size;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(sockfd, 5);
    printf("Server waiting for connection...\n");

    addr_size = sizeof(clientAddr);
    newsock = accept(sockfd, (struct sockaddr*)&clientAddr, &addr_size);
    printf("Client connected.\n");

    if (fork() == 0) {
        // Child process: Sending
        while (1) {
            printf("Server: ");
            fgets(buffer, SIZE, stdin);
            send(newsock, buffer, strlen(buffer), 0);
        }
    } else {
        // Parent process: Receiving
        while (1) {
            memset(buffer, 0, SIZE);
            int n = recv(newsock, buffer, SIZE, 0);
            if (n <= 0) break;
            printf("Client: %s", buffer);
        }
    }

    close(newsock);
    close(sockfd);
    return 0;
}
