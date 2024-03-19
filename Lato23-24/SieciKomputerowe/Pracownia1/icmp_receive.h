#include <arpa/inet.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <netinet/ip_icmp.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <time.h>


void print_as_bytes(unsigned char* buff, ssize_t length);
int receive(int id, int ttl, int seq, int sock_fd, char *sender_ip);
