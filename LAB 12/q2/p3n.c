#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include<string.h>
#include<signal.h>
#define MAX 100

struct mesg_buffer {
    long mesg_type;
    char mesg_text[100];
} message, rcv;

int left = -1, right = -1, cntf =3, cntr =0, pgid;

void Lefthandler(int sig, siginfo_t* si, void *uc){
    left = si->si_pid;
    printf("RECEIVED SIGNAL in P3 FROM P2 : %d\n", si->si_pid);

void Righthandler(int sig, siginfo_t* si, void *uc){
    right = sig;
    printf("RECEIVED SIGNAL in P2 FROM RIGHT : %d\n", sig);
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
  
    key = ftok("progfile", 65);
  
    msgid = msgget(key, 0666 | IPC_CREAT);
    // type value 3 as given in the question
    message.mesg_type = 3;
    char str[10];
  
    sprintf(str,"%d",getpid());
    // fgets(message.mesg_text,MAX,stdin);
    strcpy(message.mesg_text, str);
  
    msgsnd(msgid, &message, sizeof(message), 0);

    printf("Data send is : %s \n", message.mesg_text);

    // receving from p4
    printf("Waiting for msg from 4\n");
    msgrcv(msgid, &rcv, sizeof(rcv), 4, 0);
    msgrcv(msgid, &rcv, sizeof(rcv), 4, 0);
    printf("Data Received from p4 is : %s \n", rcv.mesg_text);
    right = atoi(rcv.mesg_text);
    kill(right, SIGUSR1);

    msgrcv(msgid, &message, 100, 3, 0);      
    pgid = atoi(message.mesg_text);
    printf("pgid - %d\n", pgid);

    while(1);
}