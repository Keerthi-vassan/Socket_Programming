#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sockfd;
    char buffer[1024];
    
    struct sockaddr_in servaddr;
    
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }
    
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8080);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    // Send message
    char msg[] = "Hello from UDP client!";
    sendto(sockfd, msg, strlen(msg), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
    printf("Message sent to server.\n");
    
    // Receive reply
    socklen_t len = sizeof(servaddr);
    recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&servaddr, &len);
    printf("Server: %s\n", buffer);

    close(sockfd);
    return 0;
}
