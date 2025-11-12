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
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addr_size = sizeof(clientAddr);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    printf("Server ready. Waiting for client...\n");

    // Wait for initial message to know client address
    recvfrom(sockfd, buffer, SIZE, 0, (struct sockaddr*)&clientAddr, &addr_size);
    printf("Client connected.\n");

    if (fork() == 0) {
        // Child: Sending
        while (1) {
            printf("Server: ");
            fgets(buffer, SIZE, stdin);
            sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&clientAddr, addr_size);
        }
    } else {
        // Parent: Receiving
        while (1) {
            memset(buffer, 0, SIZE);
            recvfrom(sockfd, buffer, SIZE, 0, (struct sockaddr*)&clientAddr, &addr_size);
            printf("Client: %s", buffer);
        }
    }

    close(sockfd);
    return 0;
}
