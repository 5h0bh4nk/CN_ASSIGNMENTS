#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include<signal.h>
#include <stdlib.h>
  
// structure for message queue
struct mesg_buffer {
    long mesg_type;
    char mesg_text[100];
} message, msg;


pid_t left = -1 , right = -1 ,pid, pid2, pid3, pid4;
int cntf = 3 , cntr = 0;

void Lefthandler(int sig, siginfo_t* si, void *uc){
    left = si->si_pid;
    printf("RECEIVED SIGNAL in P1 FROM P4 : %d\n", si->si_pid);
    int msgid = msgget((key_t)12345, 0666 | IPC_CREAT);

    msgrcv(msgid, &msg, 100, 18, 0);
    pid2 = atoi(msg.mesg_text);
    printf("Received p2 pid(msg) - %d\n", pid2);

    msgrcv(msgid, &msg, 100, 18, 0);
    pid3 = atoi(msg.mesg_text);
    printf("Received p3 pid(msg) - %d\n", pid3);

    msgrcv(msgid, &msg, 100, 18, 0);
    pid4 = atoi(msg.mesg_text);
    printf("Received p4 pid(msg) - %d\n", pid4);

    pid = getpid();
    // Create group of all processes
    setpgid(pid, 0);
    setpgid(pid2, 0);
    setpgid(pid3, 0);
    setpgid(pid4, 0);

    int grp_id = getpgid(pid);

    snprintf(msg.mesg_text, 100, "%d", grp_id);

    msg.mesg_type = 2;
    msgsnd(msgid, &msg, 10, 0);

    msg.mesg_type = 3;
    msgsnd(msgid, &msg, 10, 0);

    msg.mesg_type = 4;
    msgsnd(msgid, &msg, 10, 0);

    killpg(grp_id, SIGUSR2);
}

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