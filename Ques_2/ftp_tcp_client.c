#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sockfd;
    struct sockaddr_in serverAddr;
    char buffer[4096];
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) { perror("Socket error"); exit(1); }
    
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // change to friend's IP
    
    if (connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connect error"); exit(1);
    }
    
    FILE *fp;
    fp = fopen("video.mp4", "rb");
    if (!fp) { perror("File open error"); exit(1); }
    
    printf("Sending file...\n");
    int n;
    while ((n = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
        if (send(sockfd, buffer, n, 0) == -1) {
            perror("Send error");
            exit(1);
        }
    }

    printf("File sent successfully!\n");

    fclose(fp);
    close(sockfd);
    return 0;
}
