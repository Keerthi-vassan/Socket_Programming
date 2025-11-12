#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sockfd, newsock;
    struct sockaddr_in serverAddr, clientAddr;
    char buffer[4096];
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) { perror("Socket error"); exit(1); }
    
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    
    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind error"); exit(1);
    }
    
    listen(sockfd, 1);
    printf("Waiting for connection...\n");
    
    socklen_t addrlen = sizeof(clientAddr);
    newsock = accept(sockfd, (struct sockaddr*)&clientAddr, &addrlen);
    if (newsock < 0) { perror("Accept error"); exit(1); }
    
    FILE *fp;
    fp = fopen("received_video.mp4", "wb");
    if (!fp) { perror("File open error"); exit(1); }
    
    printf("Receiving file...\n");
    int n;
    while ((n = recv(newsock, buffer, sizeof(buffer), 0)) > 0) {
        fwrite(buffer, 1, n, fp);
    }

    printf("File received successfully!\n");

    fclose(fp);
    close(newsock);
    close(sockfd);
    return 0;
}
