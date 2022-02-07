#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include  <poll.h>
#include <fcntl.h>
#include <pthread.h>

void connect_s1(int nsfd){
    int sockfd;
    struct sockaddr_in server_addr,client_addr;
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    server_addr.sin_port=htons(atoi("8081"));
    inet_pton(AF_INET,"8081",&server_addr.sin_addr);
    connect(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr));
    char buffer[10000];
    recv(nsfd,buffer,10000,0);
    send(sockfd,buffer,10000,0);

    bzero(buffer, sizeof(buffer));
    
    // writing back
    read(sockfd,buffer,10000);
    write(nsfd,buffer,10000);
}

void connect_s2(int nsfd){
    int sockfd;
    struct sockaddr_in server_addr,client_addr;
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    server_addr.sin_port=htons(atoi("8082"));
    inet_pton(AF_INET,"8082",&server_addr.sin_addr);
    connect(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr));
    char buffer[10000];
    recv(nsfd,buffer,10000,0);
    send(sockfd,buffer,10000,0);

    bzero(buffer, sizeof(buffer));
    
    // writing back
    read(sockfd,buffer,10000);
    write(nsfd,buffer,10000);
}

void solver(int nsfd){
    char buffer[1024] = {0};
    int valread;
    read(nsfd, buffer, 1024);
    printf("%s\n", buffer);
    pthread_t t1,t2;

    if(strncmp(buffer, "1", 1)==0){
        pthread_create(&t1, NULL, (void *(*)(void *))connect_s1, (void *)&nsfd);
        pthread_join(t1, NULL);
    }
    else if(strncmp(buffer, "2", 1) == 0){
        pthread_create(&t2, NULL, (void *(*)(void *))connect_s2, (void *)&nsfd);
        pthread_join(t2, NULL);
    }
    else{
        printf("Invalid input\n");
    }
}


int main(int argc,char**argv){
    int sfd1,sfd2;
    struct pollfd pfd[2];
    struct sockaddr_in addr1, addr2;

    sfd1 = socket(AF_INET, SOCK_STREAM, 0);
    sfd2 = socket(AF_INET, SOCK_STREAM, 0);

    pfd[0].fd = sfd1;
    pfd[1].fd = sfd2;
    pfd[0].events = POLLIN;
    pfd[1].events = POLLIN;

    setsockopt(sfd1, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));
    setsockopt(sfd2, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));

    addr1.sin_family = AF_INET;
    addr1.sin_port = htons(atoi("1234"));
    addr1.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(sfd1, (struct sockaddr *)&addr1, sizeof(addr1));
    listen(sfd1, 10);

    addr2.sin_family = AF_INET;
    addr2.sin_port = htons(atoi("4321"));
    addr2.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(sfd2, (struct sockaddr *)&addr2, sizeof(addr2));
    listen(sfd2, 10);


    pthread_t t1,t2;
    int cnt = 0;
    while(1){
        if(cnt == 2){
            printf("Both connections are established\n");
            break;
        }
        int ret = poll(pfd, 2, -1);
        if(ret == -1){
            perror("poll");
            exit(1);
        }
        if(pfd[0].revents == POLLIN){
            int nsfd = accept(sfd1, NULL, NULL);
            pthread_create(&t1, NULL, (void*)solver, nsfd);
            cnt++;
        }
        if(pfd[1].revents == POLLIN){
            int nsfd = accept(sfd2, NULL, NULL);
            pthread_create(&t2, NULL, (void*)solver, nsfd);
            cnt++;
        }
    }

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
}