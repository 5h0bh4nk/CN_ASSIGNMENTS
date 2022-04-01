#include<time.h>
#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<sys/select.h>
#include<pthread.h>
#include<signal.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/shm.h>
#include<unistd.h>
#include<sys/un.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include<errno.h>
#include<netinet/if_ether.h>
#include<net/ethernet.h>
#include<netinet/ether.h>
#include<netinet/udp.h>
#include<netinet/in.h>
#include<netdb.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define ADDRESS "mysocket"

int main(){
    int usfd;
    struct sockaddr_un userv_addr;
    int userv_len,ucli_len;
    usfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(usfd==-1)
    perror("\nsocket ");
    bzero(&userv_addr,sizeof(userv_addr));
    userv_addr.sun_family = AF_UNIX;
    strcpy(userv_addr.sun_path, ADDRESS);
    userv_len = sizeof(userv_addr);
    if(connect(usfd,(struct sockaddr *)&userv_addr,userv_len)==-1)
    perror("\n connect ");
    else printf("\nconnect succesful\n"); 
	
	char buf[1024];

    bzero(buf,1024);
    printf("Enter :");
    scanf("%s",buf);
    send(usfd,buf,strlen(buf)+1,0);

    bzero(buf,1024);
    recv(usfd,buf,1024,0);
    printf("Recieved question from judge : %s\n",buf);
    fflush(stdout);

    // send answer to screen
    bzero(buf,1024);
    printf("Answer  :");
    scanf("%s",buf);
    send(usfd,buf,strlen(buf)+1,0);

	return 0;
}