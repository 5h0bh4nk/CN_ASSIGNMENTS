#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include<signal.h>
#include <stdlib.h>
  
// structure for message queue
struct mesg_buffer {
    long mesg_type;
    char mesg_text[100];
} message;


pid_t left = -1 , right = -1 ;
int cntf = 3 , cntr = 0;

void Lefthandler(int sig, siginfo_t* si, void *uc){
    left = si->si_pid;
    printf("RECEIVED SIGNAL in P1 FROM P4 : %d\n", si->si_pid);
    if(right!=-1 && cntf>=0){
        cntf--;
        kill(right, SIGUSR1);
    }
    if(cntf==-1){
        kill(left, SIGUSR2);
    }
}

void Righthandler(int sig, siginfo_t* si, void *uc){
    right = sig;
    printf("RECEIVED SIGNAL in P2 FROM RIGHT : %d\n", sig);
    if(left!=-1 && cntr>0){
        cntr--;
        kill(left, SIGUSR1);
    }
}
  
int main()
{
    struct sigaction act = {0};
    act.sa_sigaction= &Lefthandler;
    act.sa_flags = SA_SIGINFO | SA_RESTART;
    // sigfillset(&act.sa_mask);
    sigaction(SIGUSR1, &act, NULL);
    struct sigaction act1 = {0};
    act1.sa_sigaction= &Righthandler;
    act1.sa_flags = SA_SIGINFO | SA_RESTART;
    // sigfillset(&act.sa_mask);
    sigaction(SIGUSR2, &act1, NULL);

    key_t key;
    int msgid;
    left = -1, right = -1, cntf =3, cntr =0;
  
    // ftok to generate unique key
    key = ftok("progfile", 65);
  
    // msgget creates a message queue
    // and returns identifier
    msgid = msgget(key, 0666 | IPC_CREAT);
  
    printf("Waiting for msg\n");
    // msgrcv to receive message
    msgrcv(msgid, &message, sizeof(message), 2, 0);
  
    // display the message
    printf("Data Received from p2 is : %s \n", message.mesg_text);
    right = atoi(message.mesg_text);
    kill(right, SIGUSR1);
  
    // to destroy the message queue
    // msgctl(msgid, IPC_RMID, NULL);

    // right pass circular
    cntf--;
    
    while(left<0);
    printf("KILLING RIGHT %d when left = %d\n", right, left);
    kill(right, SIGUSR1);
    while(1);
  
    return 0;
}