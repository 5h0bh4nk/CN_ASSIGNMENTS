#include "networks.h"


/*
This file was copied from the Networks lab supplements.
*/

int recv_fd(int socket){
  int sent_fd, available_ancillary_element_buffer_space;
  struct msghdr socket_message;
  struct iovec io_vector[1];
  struct cmsghdr *control_message = NULL;
  char message_buffer[1];
  char ancillary_element_buffer[CMSG_SPACE(sizeof(int))];

  /* start clean */
  memset(&socket_message, 0, sizeof(struct msghdr));
  memset(ancillary_element_buffer, 0, CMSG_SPACE(sizeof(int)));

  /* setup a place to fill in message contents */
  io_vector[0].iov_base = message_buffer;
  io_vector[0].iov_len = 1;
  socket_message.msg_iov = io_vector;
  socket_message.msg_iovlen = 1;

  /* provide space for the ancillary data */
  socket_message.msg_control = ancillary_element_buffer;
  socket_message.msg_controllen = CMSG_SPACE(sizeof(int));

  if(recvmsg(socket, &socket_message, MSG_CMSG_CLOEXEC) < 0)
   return -1;

  if(message_buffer[0] != 'F')
  {
   /* this did not originate from the above function */
   return -1;
  }

  if((socket_message.msg_flags & MSG_CTRUNC) == MSG_CTRUNC)
  {
   /* we did not provide enough space for the ancillary element array */
   return -1;
  }

  /* iterate ancillary elements */
   for(control_message = CMSG_FIRSTHDR(&socket_message);
       control_message != NULL;
       control_message = CMSG_NXTHDR(&socket_message, control_message))
  {
   if( (control_message->cmsg_level == SOL_SOCKET) &&
       (control_message->cmsg_type == SCM_RIGHTS) )
   {
    sent_fd = *((int *) CMSG_DATA(control_message));
    return sent_fd;
   }
  }

  return -1;
}

int send_fd(int socket, int fd_to_send){
  struct msghdr socket_message;
  struct iovec io_vector[1];
  struct cmsghdr *control_message = NULL;
  char message_buffer[1];
  /* storage space needed for an ancillary element with a paylod of length is CMSG_SPACE(sizeof(length)) */
  char ancillary_element_buffer[CMSG_SPACE(sizeof(int))];
  int available_ancillary_element_buffer_space;

  /* at least one vector of one byte must be sent */
  message_buffer[0] = 'F';
  io_vector[0].iov_base = message_buffer;
  io_vector[0].iov_len = 1;

  /* initialize socket message */
  memset(&socket_message, 0, sizeof(struct msghdr));
  socket_message.msg_iov = io_vector;
  socket_message.msg_iovlen = 1;

  /* provide space for the ancillary data */
  available_ancillary_element_buffer_space = CMSG_SPACE(sizeof(int));
  memset(ancillary_element_buffer, 0, available_ancillary_element_buffer_space);
  socket_message.msg_control = ancillary_element_buffer;
  socket_message.msg_controllen = available_ancillary_element_buffer_space;

  /* initialize a single ancillary data element for fd passing */
  control_message = CMSG_FIRSTHDR(&socket_message);
  control_message->cmsg_level = SOL_SOCKET;
  control_message->cmsg_type = SCM_RIGHTS;
  control_message->cmsg_len = CMSG_LEN(sizeof(int));
  *((int *) CMSG_DATA(control_message)) = fd_to_send;

  return sendmsg(socket, &socket_message, 0);
}

unsigned short /* this function generates header checksums */
csum(unsigned short *buf, int nwords)
{
    unsigned long sum;
    for (sum = 0; nwords > 0; nwords--)
        sum += *buf++;
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    return ~sum;
}

int get_pid_from_name(char *name)
{
    char command[200] = {0};
    strcpy(command, "pidof ");
    strcat(command, name);
    int fd = fileno(popen(command, "r"));
    char s[1000];
    read(fd, &s, 1000);
    int X = atoi(s);
    return X;
}

struct ip *create_ip_packet(char *message, int prot_no, char *src_addr, char *dest_addr)
{
    struct ip *iph = (struct ip *)malloc(4096 * sizeof(char));
    iph->ip_hl = 5;
    iph->ip_v = 4;
    iph->ip_tos = 0;
    iph->ip_len = sizeof(struct ip) + strlen(message); /* no payload */
    iph->ip_id = htonl(54321);                         /* the value doesn't matter here */
    iph->ip_off = 0;
    iph->ip_ttl = 255;
    iph->ip_p = prot_no;
    iph->ip_sum = 0;                          /* set it to 0 before computing the actual checksum later */
    iph->ip_src.s_addr = inet_addr(src_addr); /* SYN's can be blindly spoofed */
    iph->ip_dst.s_addr = inet_addr(dest_addr);

    iph->ip_sum = csum((unsigned short *)iph, iph->ip_len >> 1);
    char *point = (char *)iph + (iph->ip_hl << 2);
    strcpy(point, message);
    return iph;
}

int send_ip_packet(struct ip *iph, int sock)
{
    struct sockaddr_in for_addr;
    for_addr.sin_family = PF_INET;
    for_addr.sin_addr = iph->ip_dst;

    return sendto(sock, iph, iph->ip_len, 0, (SA *)&for_addr, sizeof(for_addr));
}

char *recv_from_raw(int rsfd)
{
    char *buffer = (char *)malloc(4096 * sizeof(char));
    bzero(buffer, 4096);
    struct sockaddr_in for_addr;
    for_addr.sin_family = PF_INET;
    for_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    int len = sizeof(for_addr);
    int retVal = -1;
    if ((retVal = recvfrom(rsfd, buffer, 4096, 0, (SA *)&for_addr, (socklen_t *)&len)) == -1)
    {
        perror("Error in recv from fielder\n");
        return NULL;
    }
    return buffer;
}

char *get_message_from_packet(char *buffer)
{
    struct ip *iph = (struct ip *)buffer;
    char *ans = buffer + (iph->ip_hl << 2);
    return ans;
}