
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
#include "_6_telemetry.h"

/*Global variables*/



int loremSensor;





//static int htrParameter[4];

int16_t status=0;





/*init*/
void setup()
{
    Serial.begin(9600);
    server.begin();
    

    

    initAll();
    vTaskStartScheduler();

    
}

void loop() {
  // put your main code here, to run repeatedly:

}
