#ifndef ETHERNET_H
#define ETHERNET_H
#include <Ethernet2.h>
#include <EthernetUdp2.h>

class ethernet
{
    public: 
    void initEthernet();
    void initServer();
    void initUDP();
    EthernetClient checkClientAvailibility();

    private:
    EthernetServer *server;

};


#endif