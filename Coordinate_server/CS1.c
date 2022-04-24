#include "networks.h"
#define PORT 8080

int main(){
    // create TCP connection for S1
    int sfd1, sfd2;
	struct sockaddr_in serv_addr,cli_addr;
	socklen_t cli_len;
	int port_no = PORT;

	if((sfd1 = socket(AF_INET,SOCK_STREAM,0))==-1)
	perror("\n socket ");
	else printf("\n socket created successfully");

	bzero(&serv_addr,sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port_no);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	
	int opt=1;

	if(bind(sfd1,(struct sockaddr *) &serv_addr,sizeof(serv_addr))==-1)
	perror("\n bind : ");
	else printf("\n bind successful ");
    
    listen(sfd1,10);
    int nsfd;
	if((nsfd = accept(sfd1 , (struct sockaddr *)&cli_addr , &cli_len))==-1)
	perror("\n accept ");
	else printf("\n accept sfd1 successful");

	cli_len=sizeof(cli_addr);

    // create TCP connection for S2
    struct sockaddr_in serv_addr2,cli_addr2;
	if((sfd2 = socket(AF_INET,SOCK_STREAM,0))==-1)
	perror("\n socket ");
	else printf("\n socket created successfully");

	bzero(&serv_addr2,sizeof(serv_addr2));

	serv_addr2.sin_family = AF_INET;
	serv_addr2.sin_port = htons(port_no + 1);
	serv_addr2.sin_addr.s_addr = INADDR_ANY;
	
	if(bind(sfd2,(struct sockaddr *) &serv_addr2,sizeof(serv_addr2))==-1)
	perror("\n bind : ");
	else printf("\n bind successful ");

	cli_len=sizeof(cli_addr2);

    listen(sfd2,10);
	int nsfd2 ;
	if((nsfd2 = accept(sfd2 , (struct sockaddr *)&cli_addr2 , &cli_len))==-1)
	perror("\n accept ");
	else printf("\n accept sfd2 successful");

    int sfd_c1;
	if((sfd_c1 = accept(sfd1 , (struct sockaddr *)&cli_addr , &cli_len))==-1)
	perror("\n accept ");
	else printf("\n accept sfd_c1 successful");
    // recv data from C1
    char buffer[1024];
    int n = recv(sfd_c1, buffer, 1024, 0);
    buffer[n] = '\0';
    printf("\n C1: %s", buffer);
    // send details to C1
    char *details = "2 services available at CS1 , 1 & 2 !!\n";
    send(sfd_c1, details, strlen(details), 0);

    // receive sfd name to connect to 
    bzero(buffer, 1024);
    recv(sfd_c1, buffer, 1024, 0);
    printf("\n C1: %s", buffer);
    if(strcmp(buffer, "1")==0){
        // receive message from C1
        recv(sfd_c1, buffer, 1024, 0);
        printf("\n C1: %s", buffer);
        // send received message to S1
        send(nsfd, buffer, strlen(buffer), 0);
        bzero(buffer, 1024);
        recv(nsfd, buffer, 1024, 0);
        printf("\n S1: %s", buffer);
        // send received message to C1
        send(sfd_c1, buffer, strlen(buffer), 0);
    }
    else if(strcmp(buffer, "2")==0){
        // receive message from C1
        recv(sfd_c1, buffer, 1024, 0);
        printf("\n C1: %s", buffer);
        // send received message to S1
        send(nsfd2, buffer, strlen(buffer), 0);
        bzero(buffer, 1024);
        recv(nsfd2, buffer, 1024, 0);
        printf("\n S2: %s", buffer);
        // send received message to C1
        send(sfd_c1, buffer, strlen(buffer), 0);
    }
    else{
        printf("\n invalid sfd name");
    }
}