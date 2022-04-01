#include "networks.h"
#include "networks.c"

void* ball_notification(int signo){
    printf("Ball thrown from bowler !!!!\n\n");
}

int main(){
    signal(SIGUSR1, ball_notification);
    while(1){}
}