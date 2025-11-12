#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define SIZE 1024

int main() {
    int sockfd;
    char buffer[SIZE];
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    printf("Connected to server.\n");

    if (fork() == 0) {
        // Child process: Sending
        while (1) {
            printf("Client: ");
            fgets(buffer, SIZE, stdin);
            send(sockfd, buffer, strlen(buffer), 0);
        }
    } else {
        // Parent process: Receiving
        while (1) {
            memset(buffer, 0, SIZE);
            int n = recv(sockfd, buffer, SIZE, 0);
            if (n <= 0) break;
            printf("Server: %s", buffer);
        }
    }

    close(sockfd);
    return 0;
}
