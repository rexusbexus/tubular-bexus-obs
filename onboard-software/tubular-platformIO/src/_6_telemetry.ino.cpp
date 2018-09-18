/* Name: _6_telemetry.ino.cpp
 * Purpose: To gather the the sampled data and status of valves, pump and heaters.
 * Project: Tubular-Bexus.
 * Authors: Tubular-Bexus software group.
*/
#ifndef UNIT_TEST
#include <ArduinoSTL.h>
#include <vector>
#include <Ethernet2.h>
#include <EthernetUdp2.h>

#include "_6_telemetry.h"
#include "_2_mode.h"
#include "_3_sensor.h"
#include "_4_heater.h"
#include "_5_asc.h"
#include "ethernet.h"


int16_t status=0b0000000000000000;
extern unsigned int localPort;
IPAddress remote(172,16,18,160);
extern EthernetUDP Udp;
extern RTCDue rtc;
extern ethernet ethernet;
extern EthernetClient client;

void transmit() {
  // Serial.println("I'm at transmit");
  std::vector<float>  tempData = readData(0);
  std::vector<float>  humidData = readData(1);
  std::vector<float>  pressData = readData(2);
  std::vector<float>  airFData = readData(3);
  // Serial.println("Finished gathering data");
  // EthernetClient client = ethernet.checkClientAvailibility();
  // if(w5500.getPHYCFGR() == 191)
  // {
  // Serial.println("Transmitting");
  Udp.beginPacket(remote, localPort);
  Udp.write("gs,");
  Udp.write(rtc.unixtime());
  Udp.write(",ps,");
  Udp.write(nrPressSensors); Udp.write(",");
  floatval dummy;
  status_bytes dummyStatus; dummyStatus.val = status;
  for (int i = 0; i < nrPressSensors; i++) //Loop number of time there are press sensors
  {
    dummy.val = pressData[i];
    for (int k = 0; k < 4; k++)
    {
      Udp.write(dummy.bytes[k]);
    }
    Udp.write(",");
  }
  

  Udp.write("ts,");
  Udp.write(nrTempSensors); Udp.write(",");
  for (int i = 0; i < nrTempSensors; i++) //Loop number of time there are temp sensors
  {
    
    dummy.val = tempData[i];
    for (int k = 0; k < 4; k++)
    {
      Udp.write(dummy.bytes[k]);
    }
    Udp.write(",");
  }

  Udp.write("hs,");
  Udp.write(nrHumidSensors); Udp.write(",");
  for (int i = 0; i < nrHumidSensors; i++) //Loop number of time there are humidity sensors
  {
    dummy.val = humidData[i];
    for (int k = 0; k < 4; k++)
    {
      Udp.write(dummy.bytes[k]);
    }
    Udp.write(",");
  }

  Udp.write("as,");
  Udp.write(nrAirFSensors); Udp.write(",");
  for (int i = 0; i < nrAirFSensors; i++) //Loop number of time there are air flow sensors
  {
    dummy.val = airFData[i];
    for (int k = 0; k < 4; k++)
    {
      Udp.write(dummy.bytes[k]);
    }
    Udp.write(",");
  }


  Udp.write("st,");

      dummyStatus.val = (dummyStatus.val) | (int16_t(digitalRead(pumpPin)) << 0);
      dummyStatus.val = (dummyStatus.val) | (int16_t(digitalRead(valve1)) << 1);
      dummyStatus.val = (dummyStatus.val) | (int16_t(digitalRead(valve2)) << 2);
      dummyStatus.val = (dummyStatus.val) | (int16_t(digitalRead(valve3)) << 3);
      dummyStatus.val = (dummyStatus.val) | (int16_t(digitalRead(valve4)) << 4);
      dummyStatus.val = (dummyStatus.val) | (int16_t(digitalRead(valve5)) << 5);
      dummyStatus.val = (dummyStatus.val) | (int16_t(digitalRead(valve6)) << 6);
      dummyStatus.val = (dummyStatus.val) | (int16_t(digitalRead(valve7)) << 7);
      dummyStatus.val = (dummyStatus.val) | (int16_t(digitalRead(valve8)) << 8);
      dummyStatus.val = (dummyStatus.val) | (int16_t(digitalRead(valve9)) << 9);
      dummyStatus.val = (dummyStatus.val) | (int16_t(digitalRead(valve10)) << 10);
      dummyStatus.val = (dummyStatus.val) | (int16_t(digitalRead(flushValve)) << 11);
      dummyStatus.val = (dummyStatus.val) | (int16_t(digitalRead(CACvalve)) << 12);
      dummyStatus.val = (dummyStatus.val) | (int16_t(digitalRead(htr1_pin)) << 13);
      dummyStatus.val = (dummyStatus.val) | (int16_t(digitalRead(htr2_pin)) << 14);
      
      // Serial.println(status);
  Udp.write(dummyStatus.bytes[0]); 
  Udp.write(dummyStatus.bytes[1]);
  Udp.write(",md,");
  Udp.write(getMode());
  // Serial.println("Wrote mode");
  Udp.endPacket();
  // }
  // else{

  // }
  // Serial.println("Exiting Transmit");
}
#endif