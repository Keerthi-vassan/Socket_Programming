#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sockfd;
    struct sockaddr_in serverAddr, clientAddr;
    
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) { perror("Socket error"); exit(1); }
    
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    
    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind error"); exit(1);
    }
    
    printf("UDP server waiting for multiple clients...\n");
    
    char buffer[1024];
    socklen_t addr_size;
    addr_size = sizeof(clientAddr);
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&clientAddr, &addr_size);
        printf("Client [%s:%d]: %s\n",
               inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), buffer);

        // Echo back to the same client
        sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&clientAddr, addr_size);
    }

    close(sockfd);
    return 0;
}
