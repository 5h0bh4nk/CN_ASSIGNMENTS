#include "header.h"
#define ADDRESS "mysocket"

void sig1_handler(int signo){
    // selecting random judge
    int r = (rand()%3) + 1;
    // sending signal to judge
    // char* snum;
    // snprintf(snum, 10, "%d", r);
    // itoa(r, snum, 10);
    // printf("I request judge %d to ask question ...", r);
    int fd = fileno(popen("pidof ./j1", "r"));
    char s[1000];
    read(fd, &s, 1000);
    int pidJudge = atoi(s);
    printf("I rquest judge 1 to give comments !!");
    kill(pidJudge,SIGUSR1);
}

void sig2_handler(int signo){
    // connecting to s for taking score
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
    recv(usfd,buf,1024,0);
    printf("The final score is %s\n",buf);
}

int main(){
    signal(SIGUSR1,sig1_handler);
    signal(SIGUSR2,sig2_handler);

    while(1);
}