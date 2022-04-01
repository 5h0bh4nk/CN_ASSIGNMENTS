#include "networks.h"
#define __USE_BSD
#define __FAVOR_BSD	
#define BUFSIZE 1024

int main(){
	int s = socket (PF_INET, SOCK_RAW, IPPROTO_TCP);
  printf("socket created successfully\n");
  struct sockaddr_in sin;
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = inet_addr("127.0.0.1"); 

  char datagram[4096];
  scanf("Enter message to be sent : %s", datagram);
  if (sendto (s,datagram,	sizeof(datagram),0, (struct sockaddr *) &sin,sizeof (sin)) > 0)	
    printf ("Message sent successfully!!\n");
  return 0;
}
