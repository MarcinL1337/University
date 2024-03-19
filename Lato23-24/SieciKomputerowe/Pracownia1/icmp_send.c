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


// struct icmphdr initHeader(int id, int seq){
//     struct icmphdr header;
//     header.type = ICMP_ECHO;
//     header.code = 0;
//     header.un.echo.id = id;
//     header.un.echo.sequence = seq;
//     header.checksum = 0;
//     header.checksum = compute_icmp_checksum((u_int16_t*)&header, sizeof(header)); //sizeof(*header)?
//     return header;
// }

struct icmp initHeader(int id, int seq){
    struct icmp header;
    header.icmp_type = ICMP_ECHO;
    header.icmp_code = 0;
    header.icmp_hun.ih_idseq.icd_id = id;
    header.icmp_hun.ih_idseq.icd_seq = seq;
    header.icmp_cksum = 0;
    header.icmp_cksum = compute_icmp_checksum((u_int16_t*) &header, sizeof(header));
    return header;
}

int sendp(int id, int ttl, int seq, int sock_fd, struct sockaddr_in *recipient){
    fprintf(stderr, "sendp 1\n");
    // struct icmphdr header = initHeader(id, seq);
    struct icmp header = initHeader(id, seq);
    fprintf(stderr, "sendp 2\n");
    if (setsockopt(sock_fd, IPPROTO_IP, IP_TTL, &ttl, sizeof(int)) == -1) {
		fprintf(stderr, "setsockopt error: %s\n", strerror(errno)); 
		return EXIT_FAILURE;
	}

    ssize_t bytes_sent = sendto(sock_fd, 
                          &header, 
                          sizeof(header), 
                          0, 
                          (struct sockaddr*) recipient,
                          sizeof(*recipient));
    
    if(bytes_sent == -1){
        fprintf(stderr, "sendto error: %s\n", strerror(errno)); 
		return EXIT_FAILURE;
    }

    // return EXIT_SUCCESS;
}