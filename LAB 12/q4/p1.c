#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <semaphore.h>
#include<sys/shm.h>
#include<string.h>
#include <fcntl.h>

int main(){
    void *x,*y;
    int xvalue,yvalue;
    char c;
    char bufferA[100],bufferB[100];
    int shmid_x, shmid_y;

    shmid_x= shmget((key_t)11111,1024,0666|IPC_CREAT);
    shmid_y= shmget((key_t)22222,1024,0666|IPC_CREAT);

    x=shmat(shmid_x,NULL,0);
    y=shmat(shmid_y,NULL,0);

    snprintf(bufferA,100,"%d",1);
    snprintf(bufferB,100,"%d",1);
    strcpy(x,bufferA);
    strcpy(y,bufferB);

    sem_unlink("S1");
    sem_unlink("S2");
    sem_t *S1 = sem_open("S1",O_CREAT|O_EXCL,0666,1);
    sem_t *S2 = sem_open("S2",O_CREAT|O_EXCL,0666,0);

    // reading y value
    sem_wait(S1);
    printf("Reading SHM y.....\n");
    yvalue = atoi((char*)y);
    printf("Read y = %d\n",yvalue);

    // updating x value
    xvalue = yvalue + 1;
    snprintf(bufferA,100,"%d",xvalue);
    printf("Updating x value to %d \n", xvalue);
    strcpy(x,bufferA);

    // waiting to send signal on request
    printf("Enter '1' to signal S1: ");
    scanf("%c",&c);
    printf("Sending signal S1.....\n");
    sem_post(S1);

    // waiting for signal on S2
    printf("Waiting for S2.......\n");
    sem_wait(S2);
    printf("Received S2\nTerminating program\n");

}