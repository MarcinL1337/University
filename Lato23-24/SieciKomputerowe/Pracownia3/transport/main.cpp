#include "oknoPrzesuwne.hpp"


bool is_ip_addr_correct(char* ip_addr){
    int8_t dot_count = 0, aux = 0;
    char *addr_part[3];

    int idx = 0;
    for(char c = ip_addr[idx]; idx < strlen(ip_addr); idx++){
        if(aux > 3) return false;

        if(c == '.'){
            dot_count++;
            aux = 0;
        }
        else if(c >= '0' && c <= '9'){
            aux++;
        }
        else return false;
    }

    if(aux <= 3 && dot_count == 3)
        return true;
    return false;
}


int main(int argc, char **argv){
    if(argc != 5){
        fprintf(stderr, "Usage: ./transport [adres_ip] [port] [nazwa_pliku] [rozmiar (B)]");
        return EXIT_FAILURE;
    }

    char *ip_addr = argv[1];
    int port = std::stoi(argv[2]);
    char *nazwa_pliku = argv[3];
    long long rozmiar = std::stoll(argv[4]);

    if(!is_ip_addr_correct){
        fprintf(stderr, "Incorrect ip address!");
        return EXIT_FAILURE;
    }

    if(port > 65536 or port < 1){
        fprintf(stderr, "Port has to be an integer between than 1 and 65536!");
        return EXIT_FAILURE;
    }

    FILE *file = std::fopen(nazwa_pliku, "w");
    
    int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock_fd < 0) {
		fprintf(stderr, "socket error: %s\n", strerror(errno)); 
		return EXIT_FAILURE;
	}

    struct sockaddr_in server_address;
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);
	inet_pton(AF_INET, ip_addr, &server_address.sin_addr);


    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 100000;
    
    fd_set descriptors;

    
    Okno okno(rozmiar);
    okno.sendd(sock_fd, &server_address);

    bool end = false;   
    int save_count = 0;

    while(!end){
        FD_ZERO(&descriptors);
        FD_SET(sock_fd, &descriptors);
        int ready = select(sock_fd+1, &descriptors, NULL, NULL, &tv);
        if(ready < 0) return EXIT_FAILURE; // error
        else if(ready == 0){ // timeout
            
            //zrób coś
        
        } 
        else{ // git
            struct sockaddr_in sender;
            socklen_t sender_len = sizeof(sender);
            u_int8_t buffer[IP_MAXPACKET];
            ssize_t packet_len = recvfrom(sock_fd, buffer, IP_MAXPACKET, /*MSG_DONTWAIT*/0, (struct sockaddr*)&sender, &sender_len);
            
            if (packet_len < 0) {
                fprintf(stderr, "recvfrom error: %s\n", strerror(errno));
                return EXIT_FAILURE;
            }

            if (server_address.sin_addr.s_addr == sender.sin_addr.s_addr &&
                server_address.sin_port == sender.sin_port){
                    int start, size;
                    if(sscanf((char *)buffer, "DATA %d %d\n", &start, &size) == 2){
                        int offset = std::to_string(start).length() + std::to_string(size).length() + 7;
                        okno.received(start, size, (char *)buffer + offset);
                    }
            }

            if(okno.datagrams.front().get_ack()){
                Datagram datagram = okno.datagrams.front();
                fwrite(datagram.get_received_message(), sizeof(char), datagram.get_size(), file);
                save_count++;
            }

            if(save_count == ceil((double) rozmiar / segment_size)){
                fclose(file);
                end = true;
            }
        }
    }
}