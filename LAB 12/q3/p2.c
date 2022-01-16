#include <stdio.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <sys/wait.h>
#include <unistd.h>

int main(){
    sem_t *S12 = sem_open("S12",O_EXCL);
    sem_t *S23 = sem_open("S23",O_CREAT|O_EXCL,0666,0);
    char c;
    printf("I am P2. I am waiting for Semaphore S12\n");
    sem_wait(S12);
    printf("I got semaphore S12 signalling from P1\n");
    sem_post(S12);
    sem_post(S23);
    sem_wait(S23);
    printf("I am P2. Enter any character to sem-signal( S23): ");
    scanf("%c",&c);
    printf(" I am signalling semaphore signal of S23\n");
    sem_post(S23);
    
}