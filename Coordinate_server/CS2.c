#include "networks.h"
#define PORT 1337

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

	if(bind(sfd1,(struct sockaddr *) &serv_addr,sizeof(serv_addr))==-1)
	perror("\n bind : ");
	else printf("\n bind successful ");

	listen(sfd1,10);

	cli_len=sizeof(cli_addr);
	int nsfd;
	if((nsfd = accept(sfd1 , (struct sockaddr *)&cli_addr , &cli_len))==-1)
	perror("\n accept ");
	else printf("\n accept sfd1 successful");

    // create TCP connection for S2
	if((sfd2 = socket(AF_INET,SOCK_STREAM,0))==-1)
	perror("\n socket ");
	else printf("\n socket created successfully");

	bzero(&serv_addr,sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port_no + 1);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	
	if(bind(sfd2,(struct sockaddr *) &serv_addr,sizeof(serv_addr))==-1)
	perror("\n bind : ");
	else printf("\n bind successful ");

	listen(sfd2,10);

	cli_len=sizeof(cli_addr);
	int nsfd2;
	if((nsfd2 = accept(sfd2 , (struct sockaddr *)&cli_addr , &cli_len))==-1)
	perror("\n accept ");
	else printf("\n accept sfd2 successful");

}