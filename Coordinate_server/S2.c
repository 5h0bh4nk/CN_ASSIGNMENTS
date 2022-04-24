#include "networks.h"
#define PORTA 8080
#define PORTB 1337

int main(){
    // connect to CS1
    int sfd;
	struct sockaddr_in serv_addr;
	int port_no = PORTA + 1;

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

    // connect to CS2
    int sfd2;
	port_no = PORTB + 1;

	bzero(&serv_addr,sizeof(serv_addr));

	if((sfd2 = socket(AF_INET , SOCK_STREAM , 0))==-1)
	perror("\n socket");
	else printf("socket created successfully\n");

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port_no);
	//serv_addr.sin_addr.s_addr = INADDR_ANY;
	inet_pton(AF_INET,"127.0.0.1", &serv_addr.sin_addr);

	if(connect(sfd2 , (struct sockaddr *)&serv_addr , sizeof(serv_addr))==-1)
	perror("\n connect : ");
	else printf("connect succesful\n");

    while(1){
        // receive and send
        char buffer[1024];
        recv(sfd, buffer, 1024, 0);
        printf("%s\n", buffer);
        // capitalise
        for(int i=0;i<strlen(buffer);i++){
            buffer[i] = toupper(buffer[i]);
        }
        send(sfd, buffer, 1024, 0);
    }


}