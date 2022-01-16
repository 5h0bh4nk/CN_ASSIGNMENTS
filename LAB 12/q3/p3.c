#include <stdio.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <sys/wait.h>
#include <unistd.h>


int main(){
    sem_t *S23 = sem_open("S23",O_EXCL);
    sem_t *S34 = sem_open("S34",O_CREAT|O_EXCL,0666,0);
    char c;
    printf("I am P3. I am waiting for Semaphore S23\n");
    sem_wait(S23);
    printf("I got semaphore S23 signalling from P2\n");
    sem_post(S23);
    sem_post(S34);
    sem_wait(S34);
    printf("I am P3. Enter any character to sem-signal( S34): ");
    scanf("%c",&c);
    printf(" I am signalling semaphore signal of S34\n");
    sem_post(S34);
}