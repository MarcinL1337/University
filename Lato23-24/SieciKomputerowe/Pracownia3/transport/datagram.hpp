#include <arpa/inet.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <bits/stdc++.h>

static const int segment_size = 700;

class Datagram {
    private:
        int start, size;
        bool ack;
        char received_message[segment_size];
    
    public:
        Datagram(int start, int size);

        int send_segment(int sock_fd, struct sockaddr_in *recipient);
        void receive_segment(char *message);

        int get_start();
        int get_size();
        bool get_ack();
        char *get_received_message();
};