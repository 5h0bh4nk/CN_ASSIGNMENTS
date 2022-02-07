#include <unistd.h>
#include<bits/stdc++.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include<arpa/inet.h>
#include <string.h>
#include  <poll.h>
#define PORT 8080

void receive_data(int sockfd){
    char buffer[1024] = {0};
    int valread;
    // while(1){
        valread = read(sockfd, buffer, 1024);
        if(valread == 0){
            printf("Client disconnected\n");
            // break;
        }
        printf("%s\n", buffer);
        bzero(buffer, 1024);
    // }
}

void capitalize(int sockfd){
    char buffer[1024] = {0};
    int valread;
    // while(1){
        valread = read(sockfd, buffer, 1024);
        if(valread == 0){
            printf("Client disconnected\n");
            // break;
        }
        for(char c: buffer){
            if(c >= 'a' && c <= 'z'){
                c = c - 'a' + 'A';
            }
        }
        printf("%s\n", buffer);
        bzero(buffer, 1024);
    // }
}

int lower_case(int sockfd){
    char buffer[1024] = {0};
    int valread;
    // while(1){
        valread = read(sockfd, buffer, 1024);
        if(valread == 0){
            printf("Client disconnected\n");
            // break;
        }
        for(char c: buffer){
            if(c >= 'A' && c <= 'Z'){
                c = c - 'A' + 'a';
            }
        }
        printf("%s\n", buffer);
        bzero(buffer, 1024);
    // }
}

int main(){
    int sfd1, sfd2, sfd3, nsfd1, nfsd2, nfsd3, valread1, valread2, valread3;
    struct sockaddr_in addr1, addr2, addr3;

    char buffer[1024] = {0};
    int port1 = PORT, port2 = PORT+1, port3 = PORT+2;
    unsigned int addrlen = sizeof(addr1);

    // Creating socket file descriptor 1
    if((sfd1 = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket");
        exit(EXIT_FAILURE);
    }
    addr1.sin_family = AF_INET;
    addr1.sin_addr.s_addr = INADDR_ANY;
    addr1.sin_port = htons(port1);

    if(bind(sfd1, (struct sockaddr *)&addr1, addrlen) == -1){
        perror("bind");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    getsockname(sfd1, (struct sockaddr *) &server_addr, &addrlen);
    printf("Local ip address: %s\n", inet_ntoa(server_addr.sin_addr));
    printf("Local port : %u\n", ntohs(server_addr.sin_port));

    if(listen(sfd1, 3) == -1){
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // Creating socket file descriptor 2
    if((sfd2 = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket");
        exit(EXIT_FAILURE);
    }
    addr2.sin_family = AF_INET;
    addr2.sin_addr.s_addr = INADDR_ANY;
    addr2.sin_port = htons(port2);

    if(bind(sfd2, (struct sockaddr *)&addr2, addrlen) == -1){
        perror("bind");
        exit(EXIT_FAILURE);
    }

    getsockname(sfd2, (struct sockaddr *) &server_addr, &addrlen);
    printf("Local ip address: %s\n", inet_ntoa(server_addr.sin_addr));
    printf("Local port : %u\n", ntohs(server_addr.sin_port));

    if(listen(sfd2, 3) == -1){
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // Creating socket file descriptor 3
    if((sfd3 = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket");
        exit(EXIT_FAILURE);
    }
    addr3.sin_family = AF_INET;
    addr3.sin_addr.s_addr = INADDR_ANY;
    addr3.sin_port = htons(port3);

    if(bind(sfd3, (struct sockaddr *)&addr3, addrlen) == -1){
        perror("bind");
        exit(EXIT_FAILURE);
    }

    getsockname(sfd3, (struct sockaddr *) &server_addr, &addrlen);
    printf("Local ip address: %s\n", inet_ntoa(server_addr.sin_addr));
    printf("Local port : %u\n", ntohs(server_addr.sin_port));

    if(listen(sfd3, 3) == -1){
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // polling all fds
    struct pollfd fds[3];
    fds[0].fd = sfd1;
    fds[1].fd = sfd2;
    fds[2].fd = sfd3;
    fds[0].events = POLLIN;
    fds[1].events = POLLIN;
    fds[2].events = POLLIN;

    while(1){
        int ret = poll(fds, 3, 200);
        if(ret < 0){
            perror("poll");
            exit(EXIT_FAILURE);
        }
        if(fds[0].revents & POLLIN){
            if((nsfd1 = accept(sfd1, (struct sockaddr *)&addr1, &addrlen)) == -1){
                perror("accept");
                exit(EXIT_FAILURE);
            }
            printf("Connection accepted from client 1\n");
            // creating thread to receive data from client 1
            pthread_t thread1;
            pthread_create(&thread1, NULL, (void *(*)(void *))receive_data, (void *)&nsfd1);
            close(nsfd1);
        }
        if(fds[1].revents & POLLIN){
            if((nfsd2 = accept(sfd2, (struct sockaddr *)&addr2, &addrlen)) == -1){
                perror("accept");
                exit(EXIT_FAILURE);
            }
            printf("Connection accepted from client 2\n");
            // creating thread to receive data from client 2
            pthread_t thread2;
            pthread_create(&thread2, NULL, (void *(*)(void *))capitalize, (void *)&nfsd2);
            close(nfsd2);
        }
        if(fds[2].revents & POLLIN){
            if((nfsd3 = accept(sfd3, (struct sockaddr *)&addr3, &addrlen)) == -1){
                perror("accept");
                exit(EXIT_FAILURE);
            }
            printf("Connection accepted from client 3\n");
            // creating thread to receive data from client 3
            pthread_t thread3;
            pthread_create(&thread3, NULL, (void *(*)(void *))lower_case, (void *)&nfsd3);
            close(nfsd3);
        }
    }
}