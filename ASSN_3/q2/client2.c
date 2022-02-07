#include <unistd.h>
#include<bits/stdc++.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include<arpa/inet.h>
#include <string.h>
#include  <poll.h>
#define PORT 8081
using namespace std;

int main(){
    int sockfd, valread;

    struct sockaddr_in address;
    char buffer[1024] = {0};

    // create socket fd
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1){
        printf("Could not create socket\n");
        return -1;
    }

    // fill address struct
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);

    if(inet_pton(AF_INET, "127.0.0.1", &address.sin_addr) <= 0){
        printf("Invalid address\n");
        return -1;
    }
    while(connect(sockfd, (struct sockaddr *)&address, sizeof(address))){
        printf("Could not connect to server\n");
        sleep(5);
    }

    struct sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    getsockname(sockfd, (struct sockaddr *)&client_addr, &client_addr_size);
    printf("Binded to server %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    struct sockaddr_in server_addr;
    socklen_t server_addr_size = sizeof(server_addr);
    getpeername(sockfd, (struct sockaddr *)&server_addr, &server_addr_size);
    printf("Connected to server %s:%d\n", inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port));

    // receive data
    string s;
    cout<<"Enter data to send : ";
    cin>>s;
    strcpy(buffer, s.c_str());
    send(sockfd, buffer, strlen(buffer), 0);
    bzero(buffer, 1024);
    read(sockfd, buffer, 1024);
    printf("Server: %s\n", buffer);
    close(sockfd);
    return 0;
}