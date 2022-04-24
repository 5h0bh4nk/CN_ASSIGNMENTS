#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <poll.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/un.h>
#define __USE_BSD
#include <netinet/ip.h>
#define __FAVOR_BSD
#include <netinet/tcp.h>
#include <pcap.h>
#include <net/ethernet.h>
#include <netinet/ip_icmp.h>  
#include <netinet/udp.h>   

unsigned short in_cksum(unsigned short *addr, int len)
{
    register int sum = 0;
    u_short answer = 0;
    register u_short *w = addr;
    register int nleft = len;
    while (nleft > 1)
    {
      sum += *w++;
      nleft -= 2;
    }
    if (nleft == 1)
    {
      *(u_char *) (&answer) = *(u_char *) w;
      sum += answer;
    }
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    answer = ~sum;
    return (answer);
}



int main()
{
     char buff[65535];
    
    // ethernet header code
    struct ether_header *eth=(struct ether_header *)buff;
    uint8_t src[6],dst[6];
    src[0] = 0x8C;
    src[1] = 0x16;
    src[2] = 0x45;
    src[3] = 0xCE;
    src[4] = 0x8B;
    src[5] = 0xE4;
    dst[0] = 0x00;
    dst[1] = 0x25;
    dst[2] = 0x83;
    dst[3] = 0x70;
    dst[4] = 0x10;
    dst[5] = 0x00;
    memcpy(buff,dst,6*(sizeof (uint8_t)));
    memcpy(buff+6*(sizeof (uint8_t)),src,6*(sizeof (uint8_t)));
    buff[12] = ETH_P_ARP / 256;
    buff[13] = ETH_P_ARP % 256;
    


    // ip header code
    struct iphdr *iph=(struct iphdr *)(buff+sizeof(struct ether_header));
    char sip[]="126.0.0.2";
    char dip[]="128.0.0.3";
    iph->ihl=5;
    iph->version=4;
    iph->tot_len=sizeof(struct iphdr)+sizeof(struct tcphdr)+strlen(buff);
    iph->protocol=6;
    iph->ttl=64;
    iph->saddr=inet_addr(sip);
    iph->daddr=inet_addr(dip);
    iph->check=in_cksum((unsigned short *)iph,sizeof(struct iphdr));


    // tcp header code
    struct tcphdr *tcp;
    tcp = (struct tcphdr*)(buff+14+(iph->ihl*4));
    
    tcp->dest = 8082;//destination port;
    tcp->source = 8080;//source port;
    tcp->doff = 8;// length of tcp header
    tcp->seq = 156851345;// sequence number;
    tcp->ack_seq = 654864534;// acknowledgement number
    tcp->fin = 0;
    tcp->psh = 1;
    tcp->rst = 0;
    tcp->ack = 1;
    tcp->syn = 0;// SYN bit
    tcp->urg = 0;// urgent bit
    tcp->window = 1000;// window size

    char msg[]="Hello";

    for(int i=0;i<sizeof(msg);i++)
        (buff+sizeof(struct ether_header)+sizeof(struct iphdr)+sizeof(struct tcphdr))[i]=msg[i];
    
    int n=sizeof(struct ether_header)+sizeof(struct iphdr)+sizeof(struct tcphdr)+sizeof(msg);

    // send packet code
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    handle = pcap_open_live("lo", 65535, 1, 10, errbuf);
    if(!handle) {
        fprintf(stderr, "pcap_open_live: %s\n", errbuf);
        exit(1);
    }
    while(1){
    if(pcap_inject(handle,buff,n) < 0) {
        fprintf(stderr, "pcap_sendpacket: %s\n", pcap_geterr(handle));
    }
    else{
    printf("Packet injected\n");
    fflush(stdout);
    }
    sleep(3);
    }
    pcap_close(handle);

    return 0;
}