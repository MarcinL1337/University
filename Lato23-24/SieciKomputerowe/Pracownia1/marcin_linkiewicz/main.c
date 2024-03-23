// Marcin Linkiewicz, indeks: 323853

#include "icmp_receive.h"
#include "icmp_send.h"

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>


static u_int8_t ttl_max = 30;


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
		fprintf(stderr, "socket error: %s\n", strerror(errno)); 
		return EXIT_FAILURE;
	}


    char ip_addresses[3][20];
    u_int16_t id = getpid();
    bool is_finished = false;
    for(int8_t i = 1; i <= ttl_max; i++){

        // Wysyłanie pakietów
        for(int8_t j = 0; j < 3; j++)
            sendp(id, i, i+j, sock_fd, &recipient);

        int count = 0;
        int receive_ret;
        double time_elapsed = 0;
        

        struct timeval tv;
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        
        fd_set descriptors;

        char sender_ip[20];

        struct timespec start, end;
        // Zmierz czas
        clock_gettime(CLOCK_MONOTONIC, &start);

        // Odbieranie pakietów
        while(count < 3){
            
            FD_ZERO(&descriptors);
            FD_SET(sock_fd, &descriptors);
            int ready = select(sock_fd+1, &descriptors, NULL, NULL, &tv);
            if(ready < 0) return EXIT_FAILURE; // error
            else if(ready == 0) break; // timeout

            receive_ret = receive(id, i+count, sock_fd, sender_ip);
            
            
            if(receive_ret <= 1) { // error
                fprintf(stderr, "Receive error, receive_ret <= 1\n");
                return EXIT_FAILURE;
            }
            else if(receive_ret == 2){ // ICMP_ECHOREPLY
                is_finished = true;
                strcpy(ip_addresses[count], sender_ip);
                count++;
            }
            else if(receive_ret == 3){ // ICMP_TIME_EXCEEDED
                strcpy(ip_addresses[count], sender_ip);
                count++;
            }
            else if(receive_ret == 4){ // !(ICMP_ECHOREPLY || ICMP_TIME_EXCEEDED)
                continue;
            }
        }
        // Zmierz czas znowu
        clock_gettime(CLOCK_MONOTONIC, &end);
        time_elapsed = (end.tv_nsec - start.tv_nsec) / 1e6;

        printf("%d. ", i);
        if(count > 0){

            for(int8_t k = 0; k < count; k++){
                bool flag = false;
                for(int8_t l = 0; l < k; l++){
                    if(strcmp(ip_addresses[k], ip_addresses[l]) == 0){
                        flag = true;
                        break;
                    }
                }
                if(!flag)
                    printf("%s  ", ip_addresses[k]);
            }

            if(count == 3){ // 3 odpowiedzi
                double avg_time = time_elapsed / 3;
                printf("%.2lfms\n", avg_time);
            }
            else // 1 lub 2 odpowiedzi
                printf("???\n");
        }
        else{ // 0 odpowiedzi (*)
            printf("*\n");
        }
        if(is_finished) break;

    }
}