#include <netinet/ip.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip_icmp.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>


u_int8_t ttl_max = 30;


u_int16_t compute_icmp_checksum(const void *buff, int length);
void initHeader(struct icmphdr *header, int id, int seq);
void sendp(int id, int seq, int sock_fd, struct sockaddr_in *recipient);