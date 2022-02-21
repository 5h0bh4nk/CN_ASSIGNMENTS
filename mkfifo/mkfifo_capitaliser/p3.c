#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>


int main()
{
    int ffd = open("green", O_WRONLY );

    while(1)
    {
        char buffer[1024];
        scanf("%s", buffer);
        int x = strlen(buffer);
        int i=0;
        for(int i=0; i<x; i++)
        {
          if(buffer[i] >= 'a' && buffer[i] <= 'z') 
              buffer[i] = buffer[i] -32;
        }
        printf("%s ", buffer);
        fflush(stdout);
    }
}