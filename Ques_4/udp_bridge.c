#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sockfd;
    struct sockaddr_in servaddr, addr_recv;
    socklen_t addrlen = sizeof(addr_recv);
    char buffer[1024];

    struct sockaddr_in client1 = {0}, client2 = {0};
    int got1 = 0, got2 = 0;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) { perror("socket"); exit(1); }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(8080);

    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind"); close(sockfd); exit(1);
    }

    printf("UDP Bridge server listening on port 8080...\n");
    printf("Waiting for two clients to register (send any message)...\n");

    // Register first two distinct clients
    while (!(got1 && got2)) {
        int n = recvfrom(sockfd, buffer, sizeof(buffer)-1, 0,
                             (struct sockaddr*)&addr_recv, &addrlen);
        if (n <= 0) continue;
        buffer[n] = '\0';

        if (!got1) {
            client1 = addr_recv;
            got1 = 1;
            printf("Client 1 registered: %s:%d\n",
                   inet_ntoa(client1.sin_addr), ntohs(client1.sin_port));

            // optionally acknowledge
            sendto(sockfd, "Registered as Client1", 21, 0,
                   (struct sockaddr*)&client1, addrlen);
            continue;
        }
        // check different from client1
        if (!got2 &&
            (addr_recv.sin_addr.s_addr != client1.sin_addr.s_addr ||
             addr_recv.sin_port != client1.sin_port)) {
            client2 = addr_recv;
            got2 = 1;
            printf("Client 2 registered: %s:%d\n",
                   inet_ntoa(client2.sin_addr), ntohs(client2.sin_port));
                   
            sendto(sockfd, "Registered as Client2", 21, 0,
                   (struct sockaddr*)&client2, addrlen);
            continue;
        }
    }

    printf("Bridge active. Now relaying messages (turn-based).\n");

    // Main relay loop (turn-based)
    while (1) {
        // Receive from client1 and forward to client2
        int n = recvfrom(sockfd, buffer, sizeof(buffer)-1, 0,
                             (struct sockaddr*)&addr_recv, &addrlen);
        if (n <= 0) break;
        buffer[n] = '\0';
        // Only accept from client1 for this turn
        if (addr_recv.sin_addr.s_addr == client1.sin_addr.s_addr &&
            addr_recv.sin_port == client1.sin_port) {
            printf("From Client1 [%s:%d]: %s\n",
                   inet_ntoa(addr_recv.sin_addr), ntohs(addr_recv.sin_port), buffer);
            sendto(sockfd, buffer, n, 0, (struct sockaddr*)&client2, addrlen);
        } else {
            // If other sender (rare), ignore or optionally notify
            printf("Ignoring message from unknown: %s:%d\n",
                   inet_ntoa(addr_recv.sin_addr), ntohs(addr_recv.sin_port));
        }

        // Receive from client2 and forward to client1
        n = recvfrom(sockfd, buffer, sizeof(buffer)-1, 0,
                     (struct sockaddr*)&addr_recv, &addrlen);
        if (n <= 0) break;
        buffer[n] = '\0';
        if (addr_recv.sin_addr.s_addr == client2.sin_addr.s_addr &&
            addr_recv.sin_port == client2.sin_port) {
            printf("From Client2 [%s:%d]: %s\n",
                   inet_ntoa(addr_recv.sin_addr), ntohs(addr_recv.sin_port), buffer);
            sendto(sockfd, buffer, n, 0, (struct sockaddr*)&client1, addrlen);
        } else {
            printf("Ignoring message from unknown: %s:%d\n",
                   inet_ntoa(addr_recv.sin_addr), ntohs(addr_recv.sin_port));
        }
    }

    close(sockfd);
    return 0;
}
