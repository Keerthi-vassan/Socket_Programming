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
    struct sockaddr_in serverAddr;
    socklen_t addr_size = sizeof(serverAddr);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Send initial message to let server know this client
    strcpy(buffer, "Hello from client!");
    sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&serverAddr, addr_size);

    if (fork() == 0) {
        // Child: Sending
        while (1) {
            printf("Client: ");
            fgets(buffer, SIZE, stdin);
            sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&serverAddr, addr_size);
        }
    } else {
        // Parent: Receiving
        while (1) {
            memset(buffer, 0, SIZE);
            recvfrom(sockfd, buffer, SIZE, 0, (struct sockaddr*)&serverAddr, &addr_size);
            printf("Server: %s", buffer);
        }
    }

    close(sockfd);
    return 0;
}
