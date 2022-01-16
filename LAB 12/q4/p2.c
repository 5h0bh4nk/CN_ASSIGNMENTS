#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <semaphore.h>
#include <sys/wait.h>
#include<sys/shm.h>
#include<string.h>
#include <fcntl.h>


int main(){
    void *x,*y;
    char bufferA[100],bufferB[100];
    int shmid_x, shmid_y;
    int xvalue,yvalue;
    char c;
    shmid_x= shmget((key_t)11111,1024,0666);
    shmid_y= shmget((key_t)22222,1024,0666);
    x=shmat(shmid_x,NULL,0);
    y=shmat(shmid_y,NULL,0);

    sem_t *S1 = sem_open("S1",O_EXCL);
    sem_t *S2 = sem_open("S2",O_EXCL);

    printf("Waiting for signal S1...\n");
    sem_wait(S1);

    // reading x value
    xvalue = atoi((char*)x);
    printf("Read value of x is = %d\n",xvalue);
    yvalue = xvalue +1;
    snprintf(bufferB,100,"%d",yvalue);
    printf("Updating y value to %d \n", yvalue);
    strcpy(y,bufferB);
    sem_post(S1);
    printf("Enter 2 to signal S2: ");
    scanf("%c",&c);
    printf("Sending signal S2.....\nTerminating program\n");
    sem_post(S2);
}