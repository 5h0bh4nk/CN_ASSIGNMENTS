#include "networks.h"
#define __USE_BSD
#define __FAVOR_BSD	
#define BUFSIZE 1024

void print_tcp_packet(const u_char * Buffer)
{
    unsigned short iphdrlen;
     
    struct iphdr *iph = (struct iphdr *) Buffer ;
    iphdrlen = iph->ihl*4;
     
    struct tcphdr *tcph=(struct tcphdr*)(Buffer + iphdrlen );
             
    int header_size =  iphdrlen + tcph->doff*4;

    printf("\n\n--------     TCP Header     --------\n");

    printf("\n");
    printf("   |-Source Port      : %u\n",ntohs(tcph->source));
    printf("   |-Destination Port : %u\n",ntohs(tcph->dest));
    printf("   |-Sequence Number    : %u\n",ntohl(tcph->seq));
    printf("   |-Acknowledge Number : %u\n",ntohl(tcph->ack_seq));
    printf("   |-Header Length      : %d DWORDS or %d BYTES\n" ,(unsigned int)tcph->doff,(unsigned int)tcph->doff*4);
    printf("   |-Urgent Flag          : %d\n",(unsigned int)tcph->urg);
    printf("   |-Acknowledgement Flag : %d\n",(unsigned int)tcph->ack);
    printf("   |-Push Flag            : %d\n",(unsigned int)tcph->psh);
    printf("   |-Reset Flag           : %d\n",(unsigned int)tcph->rst);
    printf("   |-Synchronise Flag     : %d\n",(unsigned int)tcph->syn);
    printf("   |-Finish Flag          : %d\n",(unsigned int)tcph->fin);
    printf("   |-Window         : %d\n",ntohs(tcph->window));
    printf("   |-Checksum       : %d\n",ntohs(tcph->check));
    printf("   |-Urgent Pointer : %d\n",tcph->urg_ptr);
    printf("\n");
    printf("--------     DATA RECEIVED     --------");
    printf("\n");
    printf("%s\n",Buffer+header_size);
}

int main(){
	int saddr_size , data_size;
	struct sockaddr_in saddr;
	struct in_addr in;
	unsigned char *buffer = (unsigned char *)malloc(65536);
	int rsfd=socket(AF_INET,SOCK_RAW,IPPROTO_TCP);
	if(rsfd < 0){
		printf("Socket Error\n");
		return 1;
	}

	saddr.sin_family = AF_INET; 
    saddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    if (connect(rsfd, (struct sockaddr *)&saddr,sizeof(saddr)) < 0) { 
        printf("connection with the server failed...\n"); 
        return -1; 
    } 
	
    saddr_size = sizeof(saddr);
    //Receive a packet
    data_size = recvfrom(rsfd , buffer , 65536 , 0 , (struct sockaddr *)&saddr , (socklen_t*)&saddr_size);
    if(data_size <0 )
    {
        printf("Recvfrom error , failed to get packets\n");
        return 1;
    }
    buffer[data_size]='\0';
    print_tcp_packet(buffer);
	close(rsfd);
    printf("-------------------------------------\n");
    return 0;

}
