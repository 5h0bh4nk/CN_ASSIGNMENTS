#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <poll.h>
#include <fcntl.h>

int main(int argc,char**argv)
{
    int sockfd;
    struct sockaddr_in server_addr,client_addr;
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    server_addr.sin_port=htons(atoi(argv[1]));
    inet_pton(AF_INET,argv[1],&server_addr.sin_addr);
    connect(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr));
    char buffer[10000];
    read(0,buffer,sizeof(buffer));
    // reading whole file
    write(sockfd,buffer,10000);
    printf("the message was sent successfully\n");

    // receiving the result
    bzero(buffer, sizeof(buffer));
    read(sockfd, buffer, 10000);
    printf("%s\n", buffer);
}