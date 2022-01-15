#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>


int main()
{
    FILE* fd = popen("./p3", "w");
    dup2(fileno(fd), 1);
    while(1)
    {
        char buffer[1024];
        scanf("%s", buffer);
        printf("%s ", buffer);
        fflush(stdout);
    }
    return 0;
}