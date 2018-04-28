#ifndef UNIT_TEST
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


/*init*/
void setup()
{
    Serial.begin(9600);
    //server.begin();
    
    initAll();
    vTaskStartScheduler();

    
}

void loop() {
  // put your main code here, to run repeatedly:

}
#endif