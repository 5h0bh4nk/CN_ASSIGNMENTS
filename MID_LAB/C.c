#include "networks.h"
#include "networks.c"
#define ADDRESS "mysocket"

int winnerCount = 0;

void* thread_function(void* arg){
    int psfd = *(int*)arg;
    printf(" thread Request received for participation from %d \n\n", psfd);
    int valread;
    char buffer[1024] = {0};
    while(winnerCount < 18){
        valread = read( psfd , buffer, 1024);

        int randomByT = rand()%26;
        // turn buffer to integer
        int randomByP = atoi(buffer)%26;
        char byT = 'A' + randomByT; 
        char byP = 'A' + randomByP;

        if(byP==byT){
            // TODO : send the bwlow info to S via raw SOckets
            printf("\nParticipant %d won the event !!\n", psfd);
            winnerCount++;
        }
        else printf("\nParticipant %d lost the event !!\n", psfd);
    }
    pthread_exit(NULL);
}

int main(){
    // connect to R using unix domain socket
	int usfd;
	struct sockaddr_un userv_addr;
  	int userv_len,ucli_len;

  	usfd = socket(AF_UNIX, SOCK_STREAM, 0);

  	if(usfd==-1)
  	perror("\nsocket  ");

  	bzero(&userv_addr,sizeof(userv_addr));
  	userv_addr.sun_family = AF_UNIX;
   	strcpy(userv_addr.sun_path, ADDRESS);

	userv_len = sizeof(userv_addr);

	if(connect(usfd,(struct sockaddr *)&userv_addr,userv_len)==-1)
	perror("\n connect ");

	else printf("\nconnect succesful !! \n\n");

    while(1){
        // receive sfd of participant processes
        int psfd = recv_fd(usfd);
        if(psfd==-1) continue;

        printf("Request received for participation from %d \n\n", psfd);
        pthread_t thread;
        pthread_create(&thread, NULL, thread_function, &psfd);

        if(winnerCount>=18){
            // send signal to stop accepting participants for this event
            printf("\nAll participants have won the event !!\n");
            int fd = fileno(popen("pidof ./R", "r"));
            char s[1000];
            read(fd, &s, 1000);
            int R = atoi(s);
            kill(R,SIGUSR1);
            break;
        }
    }
}