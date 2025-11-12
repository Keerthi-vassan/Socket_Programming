#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

int main(){
    int client_sock;
    char Buffer[1024];
    struct sockaddr_in server_addr;

    client_sock = socket(AF_INET,SOCK_DGRAM,0);
    if(client_sock<0){perror("socket Failed..."); exit(1);}

    server_addr.sin_family=AF_INET;
    server_addr.sin_port= htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    while(1){
        printf("Enter message to send to server (or exit to close the connection): ");
        fgets(Buffer, sizeof(Buffer), stdin);

        if(strcmp(Buffer,"exit")==0) {break;}

        Buffer[strcspn(Buffer, "\n")] = 0; // Remove newline character

        sendto(client_sock, Buffer, strlen(Buffer), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

        memset(Buffer, 0, sizeof(Buffer));
        socklen_t addr_len = sizeof(server_addr);
        recvfrom(client_sock, Buffer, sizeof(Buffer), 0, (struct sockaddr*)&server_addr, &addr_len);
        printf("Server reply: %s\n", Buffer);
    }

    printf("closing the connection...");

    close(client_sock);
    return 0;

}