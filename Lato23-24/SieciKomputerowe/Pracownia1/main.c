#include "icmp_receive.h"
#include "icmp_send.h"


int main(int argc, char **argv){
    if(argc != 2){
        fprintf(stderr, "Usage: ./traceroute ip_addr\n");
        return EXIT_FAILURE;
    }

    char *dest = argv[0];

    struct sockaddr_in recipient;
    bzero (&recipient, sizeof(recipient));
    recipient.sin_family = AF_INET;
    int8_t check = inet_pton(AF_INET, dest, &recipient.sin_addr);

    if(!check){
        fprintf(stderr, "Invalid destination IP address\n");
        return EXIT_FAILURE;
    }

    int sock_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sock_fd < 0) {
		fprintf(stderr, "socket error: %s\n", strerror(errno)); 
		return EXIT_FAILURE;
	}

    struct pollfd ps;
    ps.fd = sock_fd;
    ps.events = POLLIN;
    ps.revents = 0;
    
    for(int8_t i = 0; i < ttl_max; i++){

        // Wysyłanie pakietów
        for(int8_t j = 0; j < 3; j++)
            sendp(i, getpid(), sock_fd, &recipient);

        int count = 0;
        double time_elapsed = 0;
        // Zmierz czas
        int t = clock();
        // int ready = poll(&ps, 1, 1000);
        for(int8_t j = 0; j < 3 || time_elapsed > 1; j++){
            int receive_ret = receive(i, getpid(), sock_fd);
            // Zmierz czas znowu
            t = clock() - t
            time_elapsed += (double)t / CLOCKS_PER_SEC;
            
            if(receive_ret <= 1) { // jakiś error
                return EXIT_FAILURE;
            }
            else{
                count++;

            }
        }
        if(read == 1 && ps.revents == POLLIN && count == 3){ // jest git
            int avg_time = time_elapsed / 3;
        }
        else{ // nie jest git
            fprintf(stderr, "socket error: %s\n", strerror(errno));
            return EXIT_FAILURE;
        }
    }
}