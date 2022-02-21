#include <stdio.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include<iostream>
using namespace std;
void *readthrd(void *arg)
{	while(1)
	{
	string s="";
	getline(cin,s);
	if(s.length()>0 && s[0]>='1'&&s[0]<='3')
	{
		string name="ser";
		name.push_back(s[0]);
		char *ch=&name[0];
		int pd=open(ch,O_WRONLY);
		int pos=s.find(":");
		s="server :"+s.substr(pos+1);
		char *buff=&s[0];
		write(pd,buff,s.length());
		close(pd);
	}
	}
}
void *writethrd(void *arg)
{
	while(1)
	{
		int fd = open("Hero", O_RDONLY);
		char ch2[1024];
		int count=read(fd, ch2, 1024); 
		string s(ch2);
		s=s.substr(0,count);
	      	if(s.length()>0 && s[0]>='1'&&s[0]<='3')
		{
			string name="ser";
			char ch5=s[0];
			int pos2=s.find(":");
			s=s.substr(pos2+1);
			name.push_back(s[0]);
			char *ch=&name[0];
			int pd=open(ch,O_WRONLY);
			int pos=s.find(":");
			string str="client ";
			str.push_back(s[0]);
			str.push_back(':');
			str=str+s.substr(pos+1);
			char *buff=&str[0];
			cout<<"From client "<<ch5<<" to "<<buff<<endl;
			write(pd,buff,str.length());
			close(pd);
		}
		close(fd);
	}
}
int main()
{
	pthread_t p1,p2;
	mkfifo("Hero",0666);
	mkfifo("ser1",0666);
	mkfifo("ser2",0666);
	mkfifo("ser3",0666);
	//while(1)
	//{
		
		pthread_create(&p1,NULL,readthrd,NULL);
		pthread_create(&p2,NULL,writethrd,NULL);
		pthread_join(p1,NULL);
		pthread_join(p2,NULL);
	//}
	return 0;
}
