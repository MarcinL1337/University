#include "icmp_send.h"


u_int16_t compute_icmp_checksum (const void *buff, int length)
{
    const u_int16_t* ptr = buff;
    u_int32_t sum = 0;
    assert (length % 2 == 0);
    for (; length > 0; length -= 2)
        sum += *ptr++;
    sum = (sum >> 16U) + (sum & 0xffffU);
    return (u_int16_t)(~(sum + (sum >> 16U)));
}


void initHeader(struct icmphdr *header, int id, int seq){
    header->type = ICMP_ECHO;
    header->code = 0;
    header->un.echo.id = id;
    header->un.echo.sequence = seq;
    header->checksum = 0;
    header-> checksum = compute_icmp_checksum((u_int16_t*)&header, sizeof(*header)); //sizeof(header)?
}


void sendp(int id, int seq, int sock_fd, struct sockaddr_in *recipient){
    struct icmphdr *header;
    initHeader(header, id, seq);

    if (setsockopt(sock_fd, IPPROTO_IP, IP_TTL, &ttl_max, sizeof(int)) == -1) {
		fprintf(stderr, "setsockopt error: %s\n", strerror(errno)); 
		return EXIT_FAILURE;
	}

    ssize_t bytes_sent = sendto(sock_fd, 
                          header, 
                          sizeof(*header), 
                          0, 
                          (struct sockaddr*) recipient,
                          sizeof(*recipient));
    
    if(bytes_sent == -1){
        fprintf(stderr, "sendto error: %s\n", strerror(errno)); 
		return EXIT_FAILURE;
    }
}