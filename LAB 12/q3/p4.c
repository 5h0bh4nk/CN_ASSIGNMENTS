#include <stdio.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <sys/wait.h>
#include <unistd.h>

int main(){
    sem_t *S34 = sem_open("S34",O_EXCL);
    sem_t *S41 = sem_open("S41",O_EXCL);
    char c;
    printf("I am P4. I am waiting for Semaphore S34\n");
    sem_wait(S34);
    printf("I got semaphore S34 signalling from P3\n");
    sem_post(S34);
    sem_post(S41);
    sem_wait(S41);
    printf("I am P4. Enter any character to sem-signal( S41): ");
    scanf("%c",&c);
    printf(" I am signalling semaphore signal of S41\n");
    sem_post(S41);
}