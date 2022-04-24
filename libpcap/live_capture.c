#include <stdio.h>
#include <time.h>
#include <pcap.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>

void print_packet_info(const u_char *packet, struct pcap_pkthdr packet_header);

int main(int argc, char *argv[]) {
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
    printf("Enter the choice of the device you want to sniff : ");
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
        error_buffer
    );

     /* Attempt to capture one packet. If there is no network traffic
      and the timeout is reached, it will return NULL */
     packet = pcap_next(handle, &packet_header);
     if (packet == NULL) {
        printf("No packet found.\n");
        return 2;
    }

    /* Our function to output some info */
    print_packet_info(packet, packet_header);

    return 0;
}

void print_packet_info(const u_char *packet, struct pcap_pkthdr packet_header) {
    printf("Packet capture length: %d\n", packet_header.caplen);
    printf("Packet total length %d\n", packet_header.len);
}