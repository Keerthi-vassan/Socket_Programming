#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

int main(){
    int client_sock;
    struct sockaddr_in server_addr;
    char Buffer[4096];

    client_sock = socket(AF_INET , SOCK_STREAM , 0);
    if(client_sock<0){ perror("socket error"); exit(1);}

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("10.0.13.61");

    if(connect(client_sock,(struct sockaddr*)&server_addr, sizeof(server_addr) )<0){
        perror("connect error"); exit(1);
    }

    while(1){
        printf("Enter message to send (type 'exit' to quit): ");
        fgets(Buffer, sizeof(Buffer), stdin);

        // Remove newline character from the input
        size_t len = strlen(Buffer);
        if (len > 0 && Buffer[len - 1] == '\n') {
            Buffer[len - 1] = '\0';
        }

        if (strcmp(Buffer, "exit") == 0) {
            break;
        }

        send(client_sock, Buffer, strlen(Buffer), 0);
    }


    close(client_sock);
    return 0;


}