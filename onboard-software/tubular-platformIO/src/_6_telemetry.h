#ifndef _6_TELEMETRY_H
#define _6_TELEMETRY_H

#include <Ethernet2.h>
#include <EthernetUdp2.h>

byte mac[] = { 0x2C, 0xF7, 0xF1, 0x08, 0x0F, 0x57 };
IPAddress ip(1, 1, 1, 1); //Local IPv4 adress
//IPAddress myDns(192,168,1, 1);
//IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 0, 0, 0);
IPAddress remote(1, 1, 1, 2);
unsigned int localPort = 8888;
EthernetUDP Udp;
EthernetServer server = EthernetServer(4000);

void transmit();

#endif