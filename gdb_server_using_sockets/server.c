#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include  <poll.h>
#include <fcntl.h>

int compareFiles(FILE *fp1, FILE *fp2)
{
    char ch1 = getc(fp1);
    char ch2 = getc(fp2);
  
    int error = 0, pos = 0, line = 1;
  
    while (ch1 != EOF && ch2 != EOF)
    {
        pos++;
  
        if (ch1 == '\n' && ch2 == '\n')
        {
            line++;
            pos = 0;
        }

        if (ch1 != ch2)
        {
            error++;
            printf("Line Number : %d \tError"
               " Position : %d \n", line, pos);
        }
  
        ch1 = getc(fp1);
        ch2 = getc(fp2);
    }
  
    if(error==0){
        printf("\nTest case passed");
        return 1;
    }
    else{
        printf("\nTest case failed");
        return 0;
    }
}

int main(int argc,char**argv){
    int listenfd,connfd;
    socklen_t client;
    struct sockaddr_in server_addr,client_addr;

    // Creating socket file descriptor
    listenfd=socket(AF_INET,SOCK_STREAM,0);
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    server_addr.sin_port=htons(atoi(argv[1]));
    bind(listenfd,(struct sockaddr*)&server_addr,sizeof(server_addr));
    listen(listenfd,5);
    client=sizeof(client_addr);
    connfd=accept(listenfd,(struct sockaddr*)&client_addr,&client);
    char buffer[10000];
    FILE *fp;
    read(connfd,buffer,10000);
    // printf(buffer);
    fp=fopen("pcopy.cpp","w");
    fprintf(fp,"%s",buffer);
    fclose(fp);
    printf("p.cpp was received successfully\n");
    printf("Copy created is pcopy.cpp");


    // compiling pcopy.cpp
    system("g++ pcopy.cpp -o P.exe");

    int pid = fork();

    if(pid == 0)
    {
        // child process
        // open IT.txt with itfd
        int itfd = open("IT.txt", O_RDONLY);
        int poutfd = open("Pout.txt", O_WRONLY);
        printf("executing ....\n");
        dup2(itfd, 0);
        dup2(poutfd, 1);
        
        execl("P.exe", "P.exe", NULL);
    }
    else
    {
        // parent process
        wait(NULL);
        FILE* fp1 = fopen("OT.txt", "r");
        FILE* fp2 = fopen("Pout.txt", "r");
        int result = compareFiles(fp1, fp2);
        if(result == 1)
        {
            // write to connection
            write(connfd,"Test case passed",20);
        }
        else
        {
            write(connfd,"Test case failed",20);
        }
        printf("\n");
    }

}