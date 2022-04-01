#include "networks.h"
#include "networks.c"
#define PROT_UMP 200
#define PROT_FIE 240
#define PROT_BAT 169
#define SA struct sockaddr

char *recv_from_rawh(int rsfd, char *src_addr)
{
    char *buffer = (char *)malloc(4096 * sizeof(char));
    bzero(buffer, 4096);
    struct sockaddr_in for_addr;
    for_addr.sin_family = PF_INET;
    for_addr.sin_addr.s_addr = inet_addr(src_addr);
    int len = sizeof(for_addr);
    int retVal = -1;
    if ((retVal = recvfrom(rsfd, buffer, 4096, 0, (SA *)&for_addr, (socklen_t *)&len)) == -1)
    {
        perror("Error in recv from fielder\n");
        return NULL;
    }
    return buffer;
}

int main(){
    int f_rsfd = -1;
    if ((f_rsfd = socket(PF_INET, SOCK_RAW, PROT_FIE)) == -1)
    {
        perror("Error in creation of raw socket !\n");
        exit(EXIT_FAILURE);
    }

    int one = 1;
    const int *val = &one;
    if(setsockopt(f_rsfd, IPPROTO_IP, IP_HDRINCL, val, sizeof(one)) < 0)
        printf("\n\t Warning: I was not able to set HDRINCL!\n");

    while (1)
    {
        struct ip *iph = (struct ip *)recv_from_rawh(f_rsfd, "1.2.3.4");
        char *message = get_message_from_packet((char *)iph);
        printf("%s\n", message);
    }
}