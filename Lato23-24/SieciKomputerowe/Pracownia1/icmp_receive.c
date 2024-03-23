// Marcin Linkiewicz, indeks: 323853

#include "icmp_receive.h"


void print_as_bytes (unsigned char* buff, ssize_t length)
{
	for (ssize_t i = 0; i < length; i++, buff++)
		printf ("%.2x ", *buff);	
}


int receive(int id, int seq, int sock_fd, char *sender_ip)
{
	struct sockaddr_in sender;
	socklen_t sender_len = sizeof(sender);
	u_int8_t buffer[IP_MAXPACKET];
	ssize_t packet_len = recvfrom(sock_fd, buffer, IP_MAXPACKET, 0, (struct sockaddr*)&sender, &sender_len);
	
	
	if (packet_len < 0) {
		fprintf(stderr, "recvfrom error: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}
	
	char sender_ip_str[20]; 
	inet_ntop(AF_INET, &(sender.sin_addr), sender_ip_str, sizeof(sender_ip_str));

	struct ip* ip_header = (struct ip*) buffer;
	u_int8_t* icmp_packet = buffer + 4 * ip_header->ip_hl;
	struct icmp *icmp_header = (struct icmp*)icmp_packet;
	
	if(icmp_header->icmp_type == ICMP_ECHOREPLY && 
	   seq == icmp_header->icmp_hun.ih_idseq.icd_seq && 
	   id == icmp_header->icmp_hun.ih_idseq.icd_id){
		strcpy(sender_ip, sender_ip_str);
		return 2;
	}
	else if(icmp_header->icmp_type == ICMP_TIME_EXCEEDED){

		icmp_packet += 8;
		icmp_packet += 4 * ((struct ip*) icmp_packet)->ip_hl;
		struct icmp *icmp_header_time_exceeded = (struct icmp*)icmp_packet;

		strcpy(sender_ip, sender_ip_str);
		if(seq == icmp_header_time_exceeded->icmp_hun.ih_idseq.icd_seq && 
		   id == icmp_header_time_exceeded->icmp_hun.ih_idseq.icd_id){
			return 3;
		}
		return 4;
	}
	else{
		return 4;
	}

	return EXIT_FAILURE;
}
