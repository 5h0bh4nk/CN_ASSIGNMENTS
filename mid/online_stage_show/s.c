#include "header.h"
#define ADDRESS "mysocket"

 int main(){
    int usfd;
    struct sockaddr_un userv_addr,ucli_addr;
    int userv_len,ucli_len;
    usfd = socket(AF_UNIX , SOCK_STREAM , 0);
    perror("socket");
    bzero(&userv_addr,sizeof(userv_addr));
    userv_addr.sun_family = AF_UNIX;
    strcpy(userv_addr.sun_path, ADDRESS);
    unlink(ADDRESS);
    userv_len = sizeof(userv_addr);
    if(bind(usfd, (struct sockaddr *)&userv_addr, userv_len)==-1)
    perror("server: bind");
    listen(usfd, 10);
    ucli_len=sizeof(ucli_addr);
    int nusfd;
    nusfd=accept(usfd, (struct sockaddr *)&ucli_addr, &ucli_len);

    char buf[1024];
    bzero(buf,1024);
    recv(nusfd,buf,1024,0);
    printf("Recieved message : %s\n",buf);
    fflush(stdout);

    int pidAnchor;

    // if buffer says to stop show , then stop it .
    if(strcmp(buf,"stop")==0){
        printf("Stopping show !!\n");
        // send signal to anchor
        int fd = fileno(popen("pidof ./a", "r"));
        char s[1000];
        read(fd, &s, 1000);
        pidAnchor = atoi(s);

        printf("pid of anchor is %d\n", pidAnchor);
        kill(pidAnchor,SIGUSR1);
    }

    // connect judge to accept question
    int nusfdJudge = accept(usfd, (struct sockaddr *)&ucli_addr, &ucli_len);
    bzero(buf,1024);
    recv(nusfdJudge,buf,1024,0);
    // printf("Recieved question from judge : %s\n",buf);
    fflush(stdout);

    // send question to performer
    send(nusfd, buf, 1024, 0);

    // recv answer from performer
    bzero(buf,1024);
    recv(nusfd,buf,1024,0);
    printf("Recieved answer : %s\n",buf);
    fflush(stdout);

    // send singal to all judges to send score 
    int fd = fileno(popen("pidof ./j1", "r"));
    char s[1000];
    read(fd, &s, 1000);
    int pidJudge1 = atoi(s);
    kill(pidJudge1,SIGUSR2);
    fd = fileno(popen("pidof ./j2", "r"));
    read(fd, &s, 1000);
    int pidJudge2 = atoi(s);    
    kill(pidJudge2,SIGUSR2);
    fd = fileno(popen("pidof ./j3", "r"));
    read(fd, &s, 1000);
    int pidJudge3 = atoi(s);
    kill(pidJudge3,SIGUSR2);

    // recv score from judges
    int score = 0;
    bzero(buf,1024);
    int j1 = accept(usfd, (struct sockaddr *)&ucli_addr, &ucli_len);
    recv(j1,buf,1024,0);
    printf("Recieved score from judge 1 : %s\n",buf);
    fflush(stdout);
    score += atoi(buf);
    bzero(buf,1024);
    int j2 = accept(usfd, (struct sockaddr *)&ucli_addr, &ucli_len);
    recv(j2,buf,1024,0);
    printf("Recieved score from judge 2 : %s\n",buf);
    fflush(stdout);
    score += atoi(buf);
    bzero(buf,1024);
    int j3 = accept(usfd, (struct sockaddr *)&ucli_addr, &ucli_len);
    recv(j3,buf,1024,0);
    printf("Recieved score from judge 3 : %s\n",buf);
    score += atoi(buf);
    fflush(stdout);

    // send score to anchor
    kill(pidAnchor,SIGUSR2);
    int anchor = accept(usfd, (struct sockaddr *)&ucli_addr, &ucli_len);
    char snum[10];
    snprintf(snum, 10, "%d", score);
    send(anchor, snum, 1024, 0);

	return 0;
 }