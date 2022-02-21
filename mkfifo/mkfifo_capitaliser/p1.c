#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>


void* reader(void *arg)
{
    int ffd = open("green", O_RDONLY);
    char buffer[1024];
    while(1)
    {
        read(ffd, buffer, sizeof(buffer));
        printf("%s\n", buffer);
    }
}

void*  writer(void *arg)
{
    int fd = *(int *)arg;
    char buffer[1024];
    while(1)
    {
        scanf("%s", buffer);
        int x = strlen(buffer);
        write(fd, buffer, strlen(buffer));
    }
}

int main()
{
    FILE* file = popen("./p2", "w");
    int fd = fileno(file);
    mkfifo("green", 0666);

    pthread_t reader, writer;

    pthread_create(&reader, NULL, reader, NULL);
    pthread_create(&writer, NULL, writer, &fd);

    pthread_join(reader, NULL);
    pthread_join(writer, NULL);

    return 0;
}