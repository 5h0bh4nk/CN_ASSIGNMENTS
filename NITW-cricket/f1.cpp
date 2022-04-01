#include "networks.h"
#include<iostream>
using namespace std;
#include "networks.c"
#define BUF_LEN 100
#define PORT 8000

int main(){
	int rsfd=socket(AF_INET,SOCK_RAW,IPPROTO_TCP);
	// if(rsfd==-1)custom_perror("socket");
	char buf[BUF_LEN];
	struct sockaddr_in client;
	socklen_t clilen=sizeof(client);
	cout<<"receive"<<endl;
	recvfrom(rsfd,buf,BUF_LEN,0,(sockaddr*)&client,(socklen_t*)clilen);
	perror("recv");
	 struct iphdr *ip;
	ip=(struct iphdr*)buf;
	cout<<(buf+(ip->ihl)*4)<<endl;
}