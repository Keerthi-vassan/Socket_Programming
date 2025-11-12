#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

int main(){
    int server_sock , curr_client;
    struct sockaddr_in Server_addr, Client_addr;
    char Buffer[4096];

    server_sock = socket(AF_INET,SOCK_STREAM,0);
    if(server_sock<0){perror("socket failed.."); exit(1);}

    Server_addr.sin_family = AF_INET;
    Server_addr.sin_port = htons(8080);
    Server_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(server_sock, (struct sockaddr*)& Server_addr , sizeof(Server_addr))<0){
        perror("bind Failed");
        exit(1);
    }

    listen(server_sock,5);
    printf("Wainting for Conection....");

    socklen_t addrlen = sizeof(Client_addr);
    curr_client = accept(server_sock,(struct sockaddr*)& Client_addr , &addrlen);
    if(curr_client < 0){ perror("Accept error"); exit(1);}

    FILE *fp;
    fp = fopen("recieved_video.mp4","wb");
    if(!fp) {perror("File open error"); exit(1);}

    printf("Recieving File.....");
    int n;
    while( ( n = recv(curr_client,Buffer,sizeof(Buffer),0)) > 0){
        fwrite(Buffer,1,n,fp);
    }

    printf("File recieved succesfully.....");

    fclose(fp);
    close(curr_client);
    close(server_sock);

    return 0;
    

}