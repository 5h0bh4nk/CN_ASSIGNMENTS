#include "header.h"
#define ADDRESS "mysocket"

void sig_handler(int signo){
    // ask a question to performer via screen
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
    else printf("\nconnect succesful");
    char buf[1024];
    bzero(buf,1024);
    printf("Enter :");
    scanf("%s",buf);
    send(usfd,buf,strlen(buf)+1,0);
}

void sig2_handler(int signo){
    // send score 
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
    else printf("\nconnect succesful !!\n");
    char buf[1024];
    bzero(buf,1024);
    printf("Enter score:");
    scanf("%s",buf);
    send(usfd,buf,strlen(buf)+1,0);
}

int main(){
    signal(SIGUSR1, sig_handler);
    signal(SIGUSR2, sig2_handler);
    while(1);
}