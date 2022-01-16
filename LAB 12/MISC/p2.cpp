#include<stdio.h>
#include<signal.h>
#include<unistd.h>
using namespace std;

void handler(int sig)
{
    printf("\nSignal %d received\n", sig);
}

int main(){
    signal(SIGINT, handler);
    while(1){
        printf("\nI am alive\n");
        sleep(1);
    }
}