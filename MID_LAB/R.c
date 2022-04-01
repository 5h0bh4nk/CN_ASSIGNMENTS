#include "networks.h"
#include "networks.c"
#define ADDRESS "mysocket"
#define PORT 8080
#define PORT2 8081
#define PROT_UMP 200
#define PROT_FIE 240
#define PROT_BAT 169

int accept1 = 1, accept2 = 1;

void* sigusr1_handler(void* arg){
	accept1 = 0;
}

void sigusr2_handler(int signo){
	accept2 = 0;
}

int main(){
	// stop sfd1 acceptance on sigusr1
	signal(SIGUSR1, sigusr1_handler);
	// stop sfd2 acceptance on sigusr2
	signal(SIGUSR2, sigusr2_handler);

    // connect R and T and C using unix domain socket
    int  usfd, sfd1, sfd2;
	struct sockaddr_un userv_addr,ucli_addr;
  	int userv_len,ucli_len;

	usfd = socket(AF_UNIX , SOCK_STREAM , 0);
	perror("socket");

  	bzero(&userv_addr,sizeof(userv_addr));

  	userv_addr.sun_family = AF_UNIX;
	strcpy(userv_addr.sun_path, ADDRESS);
	unlink(ADDRESS);
	userv_len = sizeof(userv_addr);

	if(bind(usfd, (struct sockaddr *)&userv_addr, userv_len)==-1)
	perror("server: bind");

	listen(usfd, 5);

	ucli_len=sizeof(ucli_addr);

	int Tusfd=accept(usfd, (struct sockaddr *)&ucli_addr, &ucli_len),
        Cusfd=accept(usfd, (struct sockaddr *)&ucli_addr, &ucli_len);

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
    addr1.sin_port = htons(atoi("8080"));
    addr1.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(sfd1, (struct sockaddr *)&addr1, sizeof(addr1));
    listen(sfd1, 10);

    addr2.sin_family = AF_INET;
    addr2.sin_port = htons(atoi("8081"));
    addr2.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(sfd2, (struct sockaddr *)&addr2, sizeof(addr2));
    listen(sfd2, 10);
    

    // connect the participant processes via BSD sockets 
    int server_fd, new_socket, valread;
	struct sockaddr_in server_addr, my_addr;
	int opt = 1;
    char myIP[16];
    int sockfd;
    unsigned int myPort;
	int addrlen = sizeof(server_addr);
	char buffer[1024] = {0};
	// char *hello = "Hello from server";

	// poll call here
	int size;
	struct pollfd fds[2];
	fds[0].fd = Tusfd;

	int cnt = 0;
	while(1){
        int ret = poll(pfd, 2, -1);
        if(ret == -1){
            perror("poll");
            exit(1);
        }
        if(pfd[0].revents == POLLIN){
            int nsfd = accept(sfd1, NULL, NULL);
			send_fd(Tusfd, nsfd);
			printf("Preference of event sent to T !!\n");
            cnt++;
        }
        if(pfd[1].revents == POLLIN){
            int nsfd = accept(sfd2, NULL, NULL);
			send_fd(Cusfd, new_socket);
			printf("Preference of event sent to C !!\n");
            cnt++;
        }
    }

}

