#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sockfd, client1, client2;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addr_size;
    char buffer[1024];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) { perror("Socket error"); exit(1); }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind error"); exit(1);
    }

    listen(sockfd, 2);
    printf("Server waiting for Client 1...\n");
    
    addr_size = sizeof(clientAddr);
    client1 = accept(sockfd, (struct sockaddr*)&clientAddr, &addr_size);
    printf("Client 1 connected.\n");

    printf("Server waiting for Client 2...\n");
    client2 = accept(sockfd, (struct sockaddr*)&clientAddr, &addr_size);
    printf("Client 2 connected.\n");

    printf("Bridge active! Clients can take turns typing.\n");

    while (1) {
        memset(buffer, 0, sizeof(buffer));

        // Receive from Client 1 and send to Client 2
        int n = recv(client1, buffer, sizeof(buffer), 0);
        if (n <= 0) break;
        printf("From Client 1: %s", buffer);
        send(client2, buffer, n, 0);

        memset(buffer, 0, sizeof(buffer));

        // Receive from Client 2 and send to Client 1
        n = recv(client2, buffer, sizeof(buffer), 0);
        if (n <= 0) break;
        printf("From Client 2: %s", buffer);
        send(client1, buffer, n, 0);
    }

    close(client1);
    close(client2);
    close(sockfd);
    return 0;
}
