#include "networks.h"
#include<iostream>
using namespace std;
#include "networks.c"
#define BUF_LEN 100
#define PORT 8000

int main(){
// Connect to batsman to listen to score he send through raw sockets
    int sfd;
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    if ((sfd = socket(AF_INET, SOCK_RAW, 2)) < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    addr.sin_family = AF_INET;
    // addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    int len = sizeof(addr);

    unsigned char buffer[MAXLINE];
        // this will not only rec message but also the client addr
    int packetSize = recvfrom(sfd, (char *)buffer, MAXLINE,
                              0, (sockaddr *)&addr,
                              (socklen_t *)&len);
	printf("%s", buffer);
}