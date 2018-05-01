#ifndef UNIT_TEST
#include <Ethernet2.h>
#include <EthernetUdp2.h>
#include "ethernet.h"


void ethernet::initEthernet()
{
    byte mac[] = { 0x2C, 0xF7, 0xF1, 0x08, 0x0F, 0x57 };
    IPAddress ip(1, 1, 1, 1); //Local IPv4 adress
    //IPAddress myDns(192,168,1, 1);
    //IPAddress gateway(192, 168, 1, 1);
    IPAddress subnet(255, 0, 0, 0);
    Ethernet.begin(mac, ip, subnet);
    initServer();
    
}

EthernetClient ethernet::checkClientAvailibility()
{
    return server->available();
}

void ethernet::initServer()
{
    server = new EthernetServer(4000);
    server->begin();
}
#endif