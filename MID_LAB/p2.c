#include "networks.h"
#include "networks.c"
#define PORT 8080

int main(){
    int sock = 0, valread;
	struct sockaddr_in serv_addr;
    int n = rand()%2 + 1;
	char *hello = to_string(n);
	char buffer[1024] = {0};
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
    if(n == 1){
        serv_addr.sin_port = htons(PORT);
    }
    else{
        serv_addr.sin_port = htons(PORT+1);
    }
	
	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
	{
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nConnection Failed \n");
		return -1;
	}
	send(sock , hello , strlen(hello) , 0 );
	printf("Preference of event sent !!\n");
	// valread = read( sock , buffer, 1024);
	// printf("%s\n",buffer );

    // send random numbers to T or C
    while(1){
        int random = rand()%100;
        // change random number to buffer
        sprintf(buffer, "%d", random);
        printf("\nSending random number %d to T or C\n", random);
        send(sock, buffer, sizeof(buffer), 0);
        sleep(1);
    }
}