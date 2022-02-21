#include <stdio.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include<iostream>
using namespace std;
void *readthrd(void *arg)
{
	while(1)
	{
		int fd=open("Hero",O_WRONLY);
		string s="";
		getline(cin,s);
		s="2 :"+s;
		char *buff=&s[0];
		write(fd,buff,s.length());
		close(fd);
	}
}
void *writethrd(void *arg)
{
	while(1)
	{
		int pd=open("ser2",O_RDONLY);
		char ch2[1024];
		int count=read(pd, ch2, 1024); 
		string s(ch2);
		s=s.substr(0,count);
	      	cout<<s<<endl;
		close(pd);
	}
}
int main()
{
	pthread_t p1,p2;
	mkfifo("Hero",0666);
	mkfifo("ser2",0666);
	pthread_create(&p1,NULL,readthrd,NULL);	
	pthread_create(&p2,NULL,writethrd,NULL);
	pthread_join(p1,NULL);
	pthread_join(p2,NULL);
	return 0;
}
