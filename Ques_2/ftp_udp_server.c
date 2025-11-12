 #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sockfd;
    struct sockaddr_in serverAddr, clientAddr;
    char buffer[4096];
    
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) { perror("Socket error"); exit(1); }
    
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    
    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind error"); exit(1);
    }
    
    FILE *fp;
    fp = fopen("received_video_udp.mp4", "wb");
    if (!fp) { perror("File open error"); exit(1); }
    
    printf("Waiting for UDP file...\n");
    
    int n;
    socklen_t addrlen = sizeof(clientAddr);
    while ((n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*) &clientAddr, &addrlen)) > 0) {
        if (strcmp(buffer, "END") == 0) break;  // stop signal
        fwrite(buffer, 1, n, fp);
    }

    printf("File received successfully (UDP)!\n");

    fclose(fp);
    close(sockfd);
    return 0;
}
