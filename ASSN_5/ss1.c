#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include  <poll.h>
#include <fcntl.h>

// to be run on 8081
// ./ss1 8081

int main(int argc,char**argv){
    int listenfd,connfd;
    socklen_t client;
    struct sockaddr_in server_addr,client_addr;

    // Creating socket file descriptor
    listenfd=socket(AF_INET,SOCK_STREAM,0);
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    // to be run at 8081
    server_addr.sin_port=htons(atoi(argv[1]));
    bind(listenfd,(struct sockaddr*)&server_addr,sizeof(server_addr));
    listen(listenfd,5);
    client=sizeof(client_addr);
    connfd=accept(listenfd,(struct sockaddr*)&client_addr,&client);
    char buffer[10000];
    read(connfd,buffer,10000);

    // capitalise the buffer
    int i;
    for(i=0;i<10000;i++){
        if(buffer[i]>='a' && buffer[i]<='z'){
            buffer[i] = buffer[i] - 32;
        }
    }

    printf("Sending %s",buffer);
    write(connfd,buffer,10000);
}