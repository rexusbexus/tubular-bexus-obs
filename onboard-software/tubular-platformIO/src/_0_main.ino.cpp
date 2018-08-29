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
#include "_3_sensor.h"




/*init*/
void setup()
{
    // pinMode(sdPin,OUTPUT);
    pinMode(sdPin,OUTPUT);
    pinMode(pressSensorPin1,OUTPUT);
    pinMode(pressSensorPin2,OUTPUT);
    pinMode(pressSensorPin3,OUTPUT);
    pinMode(pressSensorPin4,OUTPUT);
    pinMode(pressSensorPin5,OUTPUT);
    pinMode(pressSensorPin6,OUTPUT);
    pinMode(pressSensorPin7,OUTPUT);
    pinMode(10, OUTPUT);
    Serial.begin(9600);

    digitalWrite(pressSensorPin1,HIGH);
    digitalWrite(pressSensorPin2,HIGH);
    digitalWrite(pressSensorPin3,HIGH);
    digitalWrite(pressSensorPin4,HIGH);
    digitalWrite(pressSensorPin5,HIGH);
    digitalWrite(pressSensorPin6,HIGH);
    digitalWrite(pressSensorPin7,HIGH);
    digitalWrite(10,HIGH);

    SPI.begin();
    // SD.end();
    if(!SD.begin(sdPin))
    {
      Serial.println("Card initialization failed");
      //return;
    }
    
    //server.begin();
    initAll();
    vTaskStartScheduler();

    
}

void loop() {
  // put your main code here, to run repeatedly:

}
#endif