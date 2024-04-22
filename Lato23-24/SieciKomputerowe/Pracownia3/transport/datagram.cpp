#include "datagram.hpp"


Datagram::Datagram(long long start, long long size){
    this->start = start;
    this->size = size;
    this->ack = false;
}


int Datagram::send_segment(int sock_fd, struct sockaddr_in *recipient){
    std::string msg = "GET " + std::to_string(this->start) + " " + std::to_string(this->size) + "\n";
    
    ssize_t bytes_sent = sendto(sock_fd, 
                                msg.c_str(), 
                                msg.length(),
                                0, 
                                (struct sockaddr*) recipient,
                                sizeof(*recipient));

    if(bytes_sent == -1){
        fprintf(stderr, "send_segment error: %s\n", strerror(errno)); 
		return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}


void Datagram::receive_segment(char *message){
    this->ack = true;
    std::memcpy(this->received_message, message, this->size);
}


bool Datagram::is_equal_to(Datagram datagram){
    return this->size == datagram.get_size() && this->start == datagram.get_start();
}


int Datagram::get_start(){
    return this->start;
}


int Datagram::get_size(){
    return this->size;
}


bool Datagram::get_ack(){
    return this->ack;
}


char* Datagram::get_received_message(){
    return this->received_message;
}