#include "networks.h"
#define PORTCLIENT 8080

int main(){
    // connect to CS1
    int sfd;
    struct sockaddr_in serv_addr;
    int port_no = PORTCLIENT;

    bzero(&serv_addr,sizeof(serv_addr));

    if((sfd = socket(AF_INET , SOCK_STREAM , 0))==-1)
    perror("\n socket");
    else printf("\n socket created successfully\n");

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port_no);
    //serv_addr.sin_addr.s_addr = INADDR_ANY;
    inet_pton(AF_INET,"127.0.0.1", &serv_addr.sin_addr);
    if(connect(sfd , (struct sockaddr *)&serv_addr , sizeof(serv_addr))==-1)
	perror("\n connect : ");
	else printf("connect succesful\n");


    // sends a signal to CS1 to send details by connecting 
    send(sfd, "send details !", 14, 0);
    // receives the details from CS1
    char buffer[1024];
    recv(sfd, buffer, 1024, 0);
    printf("%s\n", buffer);

}