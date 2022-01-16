#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include<signal.h>
#include <stdlib.h>

int main(){
    key_t key;
    int msgid;
  
    // ftok to generate unique key
    key = ftok("progfile", 65);
  
    // msgget creates a message queue
    // and returns identifier
    msgid = msgget(key, 0666 | IPC_CREAT);

    msgctl(msgid, IPC_RMID, NULL);
}