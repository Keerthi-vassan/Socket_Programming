#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sockfd;
    char buffer[1024];
    
    struct sockaddr_in servaddr, cliaddr;
    
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }
    
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(8080);
    
    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(1);
    }
    
    printf("UDP Server waiting for message...\n");
    
    // Receive from client
    socklen_t len = sizeof(cliaddr);
    recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&cliaddr, &len);
    printf("Client: %s\n", buffer);

    // Send reply
    char reply[] = "Hello from UDP server!";
    sendto(sockfd, reply, strlen(reply), 0, (struct sockaddr *)&cliaddr, sizeof(servaddr));
    printf("Reply sent to client.\n");

    close(sockfd);
    return 0;
}
