#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include<string.h>
#include<signal.h>
#define MAX 100

struct mesg_buffer {
    long mesg_type;
    char mesg_text[100];
} message;

int left = -1, right = -1, cntf =3, cntr =0;

void Lefthandler(int sig, siginfo_t* si, void *uc){
    left = si->si_pid;
    printf("RECEIVED SIGNAL in P4 FROM P3 : %d\n", si->si_pid);
    if(right!=-1){
        kill(right, SIGUSR1);
    }
}

void Righthandler(int sig, siginfo_t* si, void *uc){
    right = sig;
    printf("RECEIVED SIGNAL in P2 FROM RIGHT : %d\n", sig);
    if(left!=-1){
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

    left = -1, right = -1, cntf =3, cntr =0;

    key_t key;
    int msgid;
  
    key = ftok("progfile", 65);
  
    msgid = msgget(key, 0666 | IPC_CREAT);
    // type value 3 as given in the question
    message.mesg_type = 4;
    char str[10];
    sprintf(str,"%d",getpid());
    // fgets(message.mesg_text,MAX,stdin);
    strcpy(message.mesg_text, str);
  
    msgsnd(msgid, &message, sizeof(message), 0);

    printf("Data send is : %s \n", message.mesg_text);


    // receiving from p1
    msgrcv(msgid, &message, sizeof(message), 1, 0);
    // msgrcv(msgid, &message, sizeof(message), 1, 0);
    printf("Data Received from p1 is : %s \n", message.mesg_text);
    right = atoi(message.mesg_text);
    kill(right, SIGUSR1);

    while(1);
}