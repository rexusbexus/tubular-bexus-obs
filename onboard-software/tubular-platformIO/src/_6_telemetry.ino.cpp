/* Telemtery object
   Authors: BEXUS-TUBULAR software team

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


int16_t status=0;
extern unsigned int localPort;
IPAddress remote(1, 1, 1, 2);
extern EthernetUDP Udp;
extern RTCDue rtc;

void transmit() {
  Serial.println("I'm at transmit");
  std::vector<float>  tempData = readData(0);
  std::vector<float>  humidData = readData(1);
  std::vector<float>  pressData = readData(2);
  std::vector<float>  airFData = readData(3);
  // Serial.println("Finished gathering data");
  Udp.beginPacket(remote, localPort);
  Udp.write("gs,");
  Udp.write(rtc.unixtime());
  Udp.write(",ps,");
  Udp.write(nrPressSensors); Udp.write(",");
  floatval dummy;
  status_bytes dummyStatus; dummyStatus.val = 0;
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
  for (int i = 0; i <= (htr2_pin - pumpPin) ; i++) //Loop number of time there are humidity sensors
  {
    dummyStatus.val = dummyStatus.val + dummyStatus.val + digitalRead(pumpPin + i);
             
  }
  // Serial.println(status);
  Udp.write(dummyStatus.bytes[0]); Udp.write(dummyStatus.bytes[1]);
  Udp.write(",md,");
  Udp.write(getMode());
  // Serial.println("Wrote mode");
  Udp.endPacket();
  Serial.println("Exiting Transmit");
}
#endif