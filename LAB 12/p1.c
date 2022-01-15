#include<signal.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

void handler(int sig)
{
    printf("RECEIVED SIGNAL %d\n", sig);
}

int main(){
    signal(SIGUSR1, handler);
    // raise(SIGUSR1);
    while(1);
}