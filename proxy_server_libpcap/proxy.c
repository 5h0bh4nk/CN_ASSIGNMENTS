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
        *(u_char *)(&answer) = *(u_char *)w;
        sum += answer;
    }
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    answer = ~sum;
    return (answer);
}

void print_packet_info(const u_char *packet, struct pcap_pkthdr packet_header);
void my_packet_handler(
    u_char *args,
    const struct pcap_pkthdr *packet_header,
    const u_char *packet_body)
{
    print_packet_info(packet_body, *packet_header);
    struct ether_header *eth_header;
    eth_header = (struct ether_header *)packet_body;
    // if (ntohs(eth_header->ether_type) != ETHERTYPE_IP) {
    //     printf("Not an IP packet. Skipping...\n\n");
    //     return;
    // }

    /* The total packet length, including all headers
       and the data payload is stored in
       header->len and header->caplen. Caplen is
       the amount actually available, and len is the
       total packet length even if it is larger
       than what we currently have captured. If the snapshot
       length set with pcap_open_live() is too small, you may
       not have the whole packet. */
    printf("Total packet available: %d bytes\n", packet_header->caplen);
    printf("Expected packet size: %d bytes\n", packet_header->len);

    /* Pointers to start point of various headers */
    const u_char *ip_header;
    const u_char *tcp_header;
    const u_char *payload;

    /* Header lengths in bytes */
    int ethernet_header_length = 14; /* Doesn't change */
    int ip_header_length;
    int tcp_header_length;
    int payload_length;

    /* Find start of IP header */
    ip_header = packet_body + ethernet_header_length;
    struct iphdr *ip;
    ip = (struct iphdr *)ip_header;
    char sip[] = "126.0.0.2";
    if (ip->saddr == inet_addr(sip))
    {
        printf("Packet Received from User\n");
        fflush(stdout);

        char buff[65535];

        // ethernet header code
        struct ether_header *eth = (struct ether_header *)buff;
        uint8_t src[6], dst[6];
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
        memcpy(buff, dst, 6 * (sizeof(uint8_t)));
        memcpy(buff + 6 * (sizeof(uint8_t)), src, 6 * (sizeof(uint8_t)));
        buff[12] = ETH_P_ARP / 256;
        buff[13] = ETH_P_ARP % 256;

        // ip header code
        struct iphdr *iph1 = (struct iphdr *)(buff + sizeof(struct ether_header));
        char sip[] = "128.0.0.3";
        char dip[] = "129.0.0.3";
        iph1->ihl = 5;
        iph1->version = 4;
        iph1->tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr) + strlen(buff);
        iph1->protocol = 6;
        iph1->ttl = 64;
        iph1->saddr = inet_addr(sip);
        iph1->daddr = inet_addr(dip);
        iph1->check = in_cksum((unsigned short *)iph1, sizeof(struct iphdr));

        // tcp header code
        struct tcphdr *tcp;
        tcp = (struct tcphdr *)(buff + 14 + (iph1->ihl * 4));

        tcp->dest = 8084;         // destination port;
        tcp->source = 8082;       // source port;
        tcp->doff = 8;            // length of tcp header
        tcp->seq = 156851347;     // sequence number;
        tcp->ack_seq = 654864538; // acknowledgement number
        tcp->fin = 0;
        tcp->psh = 1;
        tcp->rst = 0;
        tcp->ack = 1;
        tcp->syn = 0;       // SYN bit
        tcp->urg = 0;       // urgent bit
        tcp->window = 1000; // window size

        char msg[] = "Hello";

        for (int i = 0; i < sizeof(msg); i++)
            (buff + sizeof(struct ether_header) + sizeof(struct iphdr) + sizeof(struct tcphdr))[i] = msg[i];

        int n = sizeof(struct ether_header) + sizeof(struct iphdr) + sizeof(struct tcphdr) + sizeof(msg);

        // send packet code
        char errbuf[PCAP_ERRBUF_SIZE];
        pcap_t *handle = NULL;
        handle = pcap_open_live("lo", 65535, 1, 10, errbuf);
        if (!handle)
        {
            fprintf(stderr, "pcap_open_live: %s\n", errbuf);
            exit(1);
        }
        if (pcap_inject(handle, buff, n) < 0)
        {
            fprintf(stderr, "pcap_sendpacket: %s\n", pcap_geterr(handle));
        }
        else
        {
            printf("Packet injected\n\n");
            fflush(stdout);
        }
        sleep(3);
        pcap_close(handle);
        return;
    }
    else
    {
        printf("Packet Received from Other Than User\n\n");
        fflush(stdout);
        return;
    }
    return;
    /* The second-half of the first byte in ip_header
       contains the IP header length (IHL). */
    ip_header_length = ((*ip_header) & 0x0F);
    /* The IHL is number of 32-bit segments. Multiply
       by four to get a byte count for pointer arithmetic */
    ip_header_length = ip_header_length * 4;
    printf("IP header length (IHL) in bytes: %d\n", ip_header_length);

    /* Now that we know where the IP header is, we can
       inspect the IP header for a protocol number to
       make sure it is TCP before going any further.
       Protocol is always the 10th byte of the IP header */
    u_char protocol = *(ip_header + 9);
    if (protocol != IPPROTO_TCP)
    {
        printf("Not a TCP packet. Skipping...\n\n");
        return;
    }

    /* Add the ethernet and ip header length to the start of the packet
       to find the beginning of the TCP header */
    tcp_header = packet_body + ethernet_header_length + ip_header_length;
    struct tcphdr *tcp;
    tcp = (struct tcphdr *)tcp_header;
    if (tcp->source == 8082)
    {
        printf("Gotcha!!\n");
        fflush(stdout);
    }
    /* TCP header length is stored in the first half
       of the 12th byte in the TCP header. Because we only want
       the value of the top half of the byte, we have to shift it
       down to the bottom half otherwise it is using the most
       significant bits instead of the least significant bits */
    tcp_header_length = ((*(tcp_header + 12)) & 0xF0) >> 4;
    /* The TCP header length stored in those 4 bits represents
       how many 32-bit words there are in the header, just like
       the IP header length. We multiply by four again to get a
       byte count. */
    tcp_header_length = tcp_header_length * 4;
    printf("TCP header length in bytes: %d\n", tcp_header_length);

    /* Add up all the header sizes to find the payload offset */
    int total_headers_size = ethernet_header_length + ip_header_length + tcp_header_length;
    printf("Size of all headers combined: %d bytes\n", total_headers_size);
    payload_length = packet_header->caplen -
                     (ethernet_header_length + ip_header_length + tcp_header_length);
    printf("Payload size: %d bytes\n", payload_length);
    payload = packet_body + total_headers_size;
    printf("Memory address where payload begins: %p\n\n", payload);

    /* Print payload in ASCII */
    // if (payload_length > 0) {
    //     const u_char *temp_pointer = payload;
    //     int byte_count = 0;
    //     while (byte_count++ < payload_length) {
    //         printf("%c", *temp_pointer);
    //         temp_pointer++;
    //     }
    //     printf("\n");
    // }
    // payload_length = 20;
    // const u_char *temp_pointer = payload;
    //     int byte_count = 0;
    //     while (byte_count++ < payload_length) {
    //         printf("%c", *temp_pointer);
    //         temp_pointer++;
    //     }
    //     printf("\n");

    return;
}

int main(int argc, char *argv[])
{
    char error_buffer[PCAP_ERRBUF_SIZE];
    pcap_t *handle;
    const u_char *packet;
    struct pcap_pkthdr packet_header;
    int packet_count_limit = 1;
    int timeout_limit = 10000; /* In milliseconds */
    struct bpf_program filter;
    char filter_exp[] = "port 8080";
    bpf_u_int32 subnet_mask, ip;

    pcap_if_t *alldevsp, *device;

    char errbuf[100], *devname, devs[100][100];
    int count = 1, n;

    // First get the list of available devices
    printf("Finding available devices ... ");
    if (pcap_findalldevs(&alldevsp, error_buffer))
    {
        printf("Error finding devices : %s", errbuf);
        exit(1);
    }
    printf("Done");

    // Print the available devices
    printf("\nAvailable Devices are :\n");
    for (device = alldevsp; device != NULL; device = device->next)
    {
        printf("%d. %s - %s\n", count, device->name, device->description);
        if (device->name != NULL)
        {
            strcpy(devs[count], device->name);
        }
        count++;
    }

    // Ask user which device to sniff
    printf("Enter the number of the device you want to sniff : ");
    scanf("%d", &n);
    devname = devs[n];

    if (pcap_lookupnet(devname, &ip, &subnet_mask, error_buffer) == -1)
    {
        printf("Could not get information for device: %s\n", devname);
        ip = 0;
        subnet_mask = 0;
    }
    /* Open device for live capture */
    handle = pcap_open_live(
        devname,
        BUFSIZ,
        packet_count_limit,
        timeout_limit,
        error_buffer);

    /* Attempt to capture one packet. If there is no network traffic
     and the timeout is reached, it will return NULL */
    //  packet = pcap_next(handle, &packet_header);
    //  if (packet == NULL) {
    //     printf("No packet found.\n");
    //     return 2;
    // }

    // if (pcap_compile(handle, &filter, filter_exp, 0, ip) == -1) {
    //     printf("Bad filter - %s\n", pcap_geterr(handle));
    //     return 2;
    // }
    // if (pcap_setfilter(handle, &filter) == -1) {
    //     printf("Error setting filter - %s\n", pcap_geterr(handle));
    //     return 2;
    // }
    pcap_loop(handle, 0, my_packet_handler, NULL);

    /* Our function to output some info */
    // print_packet_info(packet, packet_header);

    return 0;
}

void print_packet_info(const u_char *packet, struct pcap_pkthdr packet_header)
{
    printf("Packet capture length: %d\n", packet_header.caplen);
    printf("Packet total length %d\n", packet_header.len);
}