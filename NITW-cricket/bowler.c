#include "networks.h"
#include "networks.c"
#define ADDRESS  "mysocket"

struct myipheader
 {
#if __BYTE_ORDER == __LITTLE_ENDIAN
    unsigned int ihl:4;
    unsigned int version:4;
#elif __BYTE_ORDER == __BIG_ENDIAN
    unsigned int version:4;
    unsigned int ihl:4;
#else
# error	"Please fix <bits/endian.h>"
#endif
    uint8_t tos;
    uint16_t tot_len;
    uint16_t id;
    uint16_t frag_off;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t check;
    uint32_t saddr;
    uint32_t daddr;
    /*The options start here. */
 };

int main(int argc, char** argv){

	int  usfd;
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

	int nusfd;
	nusfd=accept(usfd, (struct sockaddr *)&ucli_addr, &ucli_len);

	int fd;
	if ((fd = open("ball.txt",O_RDONLY)) < 0)   /*open a new file */
	{
		perror("ball.txt");
		exit(1);
	}

	// 1. send the ball to the batsman
	printf("sending %d with sfd = %d\n", fd, fd);
	send_fd(nusfd, fd);

	// 2. send signal to umpire
	fd = fileno(popen("pidof ./umpire", "r"));
	char s[1000];
	read(fd, &s, 1000);
	int X = atoi(s);
	kill(X,SIGUSR1);

    // if(argc<2)printf("Enter protocal in arguments");
}