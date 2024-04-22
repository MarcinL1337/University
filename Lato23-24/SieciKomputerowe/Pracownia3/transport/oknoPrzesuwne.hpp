#include "datagram.hpp"

static const int window_capacity = 600;

class Okno{
    private:
        int size;

    public:
        std::list<Datagram> datagrams;
        
        Okno(int size);
        void sendd(int sockfd, struct sockaddr_in *address);
        void received(int start, int size, char *buffer);
        void shift_window();

};
