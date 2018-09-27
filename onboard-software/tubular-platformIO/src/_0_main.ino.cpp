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




/*init
  The first function that gets run. Initates the SPI 
  and the nessicary pins to run spi. Also starts the 
  SD wich work over the SPI. The RTOS also starts in
  this function.
*/
void setup()
{
    // pinMode(sdPin,OUTPUT);

    pinMode(21, OUTPUT);
    for(int i = 0; i<8; i++)
    {
      digitalWrite(21, LOW);
      delayMicroseconds(3);
      digitalWrite(21, HIGH);
      delayMicroseconds(3);
    }
    pinMode(21, INPUT);
    pinMode(sdPin,OUTPUT);
    pinMode(pressSensorPin1,OUTPUT);
    pinMode(pressSensorPin2,OUTPUT);
    pinMode(pressSensorPin3,OUTPUT);
    pinMode(pressSensorPin4,OUTPUT);
    pinMode(pressSensorPin5,OUTPUT);
    pinMode(pressSensorPin6,OUTPUT);
    pinMode(pressSensorPin7,OUTPUT);
    pinMode(10, OUTPUT);
    pinMode(LED, OUTPUT);
    Serial.begin(9600);

    digitalWrite(pressSensorPin1,HIGH);
    digitalWrite(pressSensorPin2,HIGH);
    digitalWrite(pressSensorPin3,HIGH);
    digitalWrite(pressSensorPin4,HIGH);
    digitalWrite(pressSensorPin5,HIGH);
    digitalWrite(pressSensorPin6,HIGH);
    digitalWrite(pressSensorPin7,HIGH);
    digitalWrite(10,HIGH);
    digitalWrite(LED, LOW);

    Wire.begin();
    Wire.setClock(33000);
    SPI.begin();
    // SD.end();
    if(!SD.begin(sdPin))
    {
      Serial.println("Card initialization failed");
      digitalWrite(LED, HIGH);
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