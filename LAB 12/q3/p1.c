#include <stdio.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/wait.h>


int main(){
    // unliking from other programs
    sem_unlink("S12");
    sem_unlink("S23");
    sem_unlink("S34");
    sem_unlink("S41");

    // creating semaphores as specified
    sem_t *S12 = sem_open("S12",O_CREAT|O_EXCL,0666,1);
    sem_t *S41 = sem_open("S41",O_CREAT|O_EXCL,0666,0);
    char c;
    sem_wait(S12);
    printf("I am P1. Enter any character to sem-signal( S12): ");
    scanf("%c",&c);
    printf("I am signalling semaphore signal of S12\n");
    sem_post(S12);
    printf("I am waiting for semaphore S41\n");
    sem_wait(S41);
    printf("I got semaphore signalling from P4\n");
    sem_post(S41);
    
}