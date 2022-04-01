#include "networks.h"
#include "networks.c"
#include <iostream>
using namespace std;
#define BUF_LEN 100
#define PORT 8000
#define ADDRESS "mysocket"

int generate(int speed, int spin){
	// generate random number between 0 and 40
	int r = rand() % 40;
	return r;
}


int main(int argc , char** argv){
	// int usfd;
	// struct sockaddr_un userv_addr;
  	// int userv_len,ucli_len;

  	// usfd = socket(AF_UNIX, SOCK_STREAM, 0);

  	// if(usfd==-1)
  	// perror("\nsocket  ");

  	// bzero(&userv_addr,sizeof(userv_addr));
  	// userv_addr.sun_family = AF_UNIX;
   	// strcpy(userv_addr.sun_path, ADDRESS);

	// userv_len = sizeof(userv_addr);

	// if(connect(usfd,(struct sockaddr *)&userv_addr,userv_len)==-1)
	// perror("\n connect ");

	// else printf("\nconnect succesful\n");

	// // int ffd = recv_fd(usfd);
	// // 3. read data from file 
	// char buf[BUF_LEN];
	// int n;
	// int rcid = recv_fd(usfd);
	// printf("%d", rcid);
	// bzero(buf,BUF_LEN);
	// read(rcid,buf,BUF_LEN);
	// printf("%s\n",buf);

	// int speed = atoi(buf[0]), spin = atoi(buf[2]);

	// // 4. generate random number
	// int r = generate(speed , spin);


	// send data to fielders 
	// int rsfd=socket(AF_INET,SOCK_RAW,2);
	// perror("socket");
	// int optval=1;
	// setsockopt(rsfd, 2, SO_BROADCAST, &optval, sizeof(int));//IP_HDRINCL
	// printf("opt");
	// 	struct sockaddr_in client;
	// client.sin_family=AF_INET;
	// client.sin_addr.s_addr=inet_addr("127.0.0.1");

	// char buff[]="hello";	
	// client.sin_addr.s_addr=INADDR_ANY;
	// bind(rsfd, (struct sockaddr *)&client, sizeof(client));
	// unsigned int client_len=sizeof(client);
	// printf("sending");
	// sleep(5);
    // sendto(rsfd,buff,strlen(buff)+1,0,(struct sockaddr*)&client,sizeof(client));
    // perror("send");

	// int sfd;
    // struct sockaddr_in addr, clientAddr;
    // int len = sizeof(clientAddr);
	// string run="207";
    // if ((sfd = socket(AF_INET, SOCK_RAW, 2)) < 0)
    // {
    //     perror("socket creation failed");
    //     exit(EXIT_FAILURE);
    // }
    // memset(&addr, 0, sizeof(addr));
    // memset(&clientAddr, 0, sizeof(clientAddr));

    // addr.sin_family = AF_INET;
    // addr.sin_port = htons(PORT);
    // addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // bind(sfd, (const struct sockaddr *)&addr,
    //      sizeof(addr));
	// sleep(5);
    // sendto(sfd, run.c_str(), run.size(), 0, (const struct sockaddr *)&addr, sizeof(addr));


	// if(argc<2)cout<<"Enter protocal in arguments";
	int rsfd=socket(AF_INET,SOCK_RAW,3);
	perror("socket");
	int optval=1;
	setsockopt(rsfd, IPPROTO_TCP, SO_BROADCAST, &optval, sizeof(int));//IP_HDRINCL
	cout<<"opt"<<endl;
		struct sockaddr_in client;
	client.sin_family=AF_INET;
	client.sin_addr.s_addr=inet_addr("127.0.0.1");

	char buff[]="hello";	
	client.sin_addr.s_addr=INADDR_ANY;

	unsigned int client_len=sizeof(client);
	cout<<"sending"<<endl;
	sleep(10);
	sendto(rsfd,buff,strlen(buff)+1,0,(struct sockaddr*)&client,sizeof(client));
	perror("send");
}