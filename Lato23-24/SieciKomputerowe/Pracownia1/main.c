#include "icmp_receive.h"
#include "icmp_send.h"

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>

void print_aux(char **ip_addresses, int address_count, double avg_time){

}


int main(int argc, char **argv){

    if(argc != 2){
        fprintf(stderr, "Usage: ./traceroute ip_addr\n");
        return EXIT_FAILURE;
    }

    char *dest = argv[1];

    struct sockaddr_in recipient;
    bzero(&recipient, sizeof(recipient));
    recipient.sin_family = AF_INET;
    int8_t check = inet_pton(AF_INET, dest, &recipient.sin_addr);

    if(!check){
        fprintf(stderr, "Invalid destination IP address\n");
        return EXIT_FAILURE;
    }

    int sock_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sock_fd < 0) {
		fprintf(stderr, "socket error aaa: %s\n", strerror(errno)); 
		return EXIT_FAILURE;
	}

    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;

    char ip_addresses[3][20];
    u_int16_t id = getpid();
    // int seq = 0;
    fprintf(stderr, "aaaa\n");
    bool is_finished = false;
    for(int8_t i = 0; i < ttl_max; i++){

        fprintf(stderr, "dupa %d\n", i);

        // Wysyłanie pakietów
        for(int8_t j = 0; j < 3; j++)
            sendp(id, i, i+j, sock_fd, &recipient);


        fprintf(stderr, "main 1\n");

        int count = 0, receive_ret;
        double time_elapsed = 0;
        
        // Zmierz czas
        clock_t t = clock();
        
        fd_set descriptors;

        // Odbieranie pakietów
        for(int8_t j = 0; j < 3 || time_elapsed > 1; j++){
            char sender_ip[20];
            
            FD_ZERO(&descriptors);
            FD_SET(sock_fd, &descriptors);
            int ready = select(sock_fd+1, &descriptors, NULL, NULL, &tv);
            if(ready < 0) return EXIT_FAILURE; // error
            else if(ready == 0) break; // timeout

            fprintf(stderr, "for 1\n");
            receive_ret = receive(id, i, i+j, sock_fd, sender_ip);
            fprintf(stderr, "ip = %s\n", sender_ip);
            fprintf(stderr, "for 2\n");
            // Zmierz czas znowu
            t = clock() - t;
            time_elapsed += (double)t / CLOCKS_PER_SEC;
            
            if(receive_ret <= 1) { // jakiś error
                fprintf(stderr, "Receive error, receive_ret <= 1\n");
                return EXIT_FAILURE;
            }
            else{
                if(receive_ret == 2) is_finished = true;
                strcpy(ip_addresses[count], sender_ip);
                count++;
            }
        }
        printf("%d. ", i+1);
        if(count == 3){ // wszystkie 3 odpowiedzi
            double avg_time = time_elapsed / 3 * 1000.0;
            if(receive_ret == 2){
                printf("%s  ", ip_addresses[0]);
                printf("%s  ", ip_addresses[1]);
                printf("%s  ", ip_addresses[2]);
                printf("%lfms\n", avg_time);
                break;
            }
        }
        else if(count > 0){ // mniej niż 3 odpowiedzi, ale więcej niz 1 (???)
            for(int8_t k = 0; k < count; k++)
                printf("%s  ", ip_addresses[k]);
            printf("???\n");
        }
        else{ // 0 odpowiedzi (*)
            printf("*\n");
        }
        if(is_finished) break;

    }
}