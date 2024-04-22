#include "oknoPrzesuwne.hpp"

Okno::Okno(int size){
    this->size = size;
    Datagram datagram(0, segment_size);
    this->datagrams.push_back(datagram);

    int count = 1;

    for(; count < window_capacity && this->datagrams.back().get_start() + segment_size < this->size; count++){
        int aux = this->size - (this->datagrams.back().get_start() + segment_size);
        if(aux <= segment_size){
            Datagram datagram2(this->datagrams.back().get_start() + segment_size, aux);
            this->datagrams.push_back(datagram2);
            
        } 
        else{
            Datagram datagram2(this->datagrams.back().get_start() + segment_size, segment_size);
            this->datagrams.push_back(datagram2);

        }
    }
}


void Okno::sendd(int sockfd, struct sockaddr_in *address){
    for(auto d : this->datagrams){
        if(!d.get_ack())
            d.send_segment(sockfd, address);
    }
}


void Okno::received(int start, int size, char *buffer){
    Datagram d(start, size);
    for(auto datagram = datagrams.begin(); datagram != datagrams.end(); datagram++){
        if(d.is_equal_to(*datagram) && !datagram->get_ack()){
            datagram->receive_segment(buffer);
            break;
        }
    }
}


void Okno::shift_window(){
    this->datagrams.pop_front();
    if(!this->datagrams.empty() && this->datagrams.back().get_start() + segment_size < this->size){
        int aux = this->size - (this->datagrams.back().get_start() + segment_size);
        if(aux <= segment_size){
            Datagram datagram(this->datagrams.back().get_start() + segment_size, aux);
            this->datagrams.push_back(datagram);
        }
        else{
            Datagram datagram(this->datagrams.back().get_start() + segment_size, segment_size);
            this->datagrams.push_back(datagram);
        }
    }
    
}