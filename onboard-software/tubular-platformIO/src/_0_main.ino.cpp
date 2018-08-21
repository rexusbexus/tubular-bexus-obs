/* Name: _0_main.ino.cpp
 * Purpose: To start the onoard software
 * Project: Tubular-Bexus.
 * Authors: Tubular-Bexus software group.
*/
#ifndef UNIT_TEST
#include <Arduino.h>
#include <ArduinoSTL.h>
#include <vector>

#include <Wire.h>
#include <SD.h>

#include <SPI.h>
#include <stdbool.h>

#include <Ethernet2.h>
#include <EthernetUdp2.h>

#include <FreeRTOS_ARM.h>

#include <RTCDue.h>


#include "_1_init.h"
#include "ethernet.h"

#define sdPin           4


/*init*/
void setup()
{
    // pinMode(sdPin,OUTPUT);
    pinMode(sdPin,OUTPUT);
    pinMode(5,OUTPUT);
    pinMode(38,OUTPUT);
    pinMode(39,OUTPUT);
    pinMode(40,OUTPUT);
    pinMode(41,OUTPUT);
    pinMode(42,OUTPUT);
    pinMode(43,OUTPUT);
    Serial.begin(9600);

    digitalWrite(5,HIGH);
    digitalWrite(38,HIGH);
    digitalWrite(39,HIGH);
    digitalWrite(40,HIGH);
    digitalWrite(41,HIGH);
    digitalWrite(42,HIGH);
    digitalWrite(43,HIGH);

    SPI.begin();
    SD.end();
    if(!SD.begin(sdPin))
    {
      Serial.println("Card initialization failed");
      return;
    }
    
    
    //server.begin();
    initAll();
    vTaskStartScheduler();

    
}

void loop() {
  // put your main code here, to run repeatedly:

}
#endif