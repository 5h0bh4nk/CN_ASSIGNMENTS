#include "networks.h"
#include "networks.c"
#define ADDRESS "mysocket"
#define PROT_UMP 200
#define PROT_FIE 240
#define PROT_BAT 169

int winnerCount = 0, usfd;

void* thread_function(void* arg){
    int psfd = *(int*)arg;
    printf(" thread Request received for participation from %d \n\n", psfd);
    int valread;
    char buffer[1024] = {0};
    while(winnerCount < 18){
        valread = read( psfd , buffer, 1024);

        int randomByT = rand()%100;
        // turn buffer to integer
        int randomByP = atoi(buffer);

        if(randomByT == randomByP){
            // TODO : send the bwlow info to S via raw SOckets
            // create a raw socket
            int r_rsfd = -1;
            if ((r_rsfd = socket(PF_INET, SOCK_RAW, 3)) == -1)
            {
                perror("error in creation of raw socket\n");
                exit(EXIT_FAILURE);
            }
            char message[4096] = {0};

            sprintf(message, "Participant %d won the event !!", psfd);
            int one = 1;
            const int *val = &one;
            if (setsockopt(r_rsfd, IPPROTO_IP, IP_HDRINCL, val, sizeof(one)) < 0)
                printf("\n\t Warning: I was not able to set HDRINCL!\n");

            
            struct ip *iph = create_ip_packet(message, PROT_FIE, "1.2.3.4", "127.0.0.1");
            if (send_ip_packet(iph, r_rsfd) == -1)
            {
                perror("Error in sending message to screen !\n");
            }
            // else printf("Message sent to screen !\n");
                
            winnerCount++;
        }
        else{
            printf("Participant %d lost the event !!\n", psfd);
        }
    }
    pthread_exit(NULL);
}

int main(){
    // connect to R using unix domain socket
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