#include <M2M_LM75A.h>
#include <MS5611.h>
#include <MS5xxx.h>
#include <Wire.h>

#include <StandardCplusplus.h>
#include <vector>

#include <SPI.h>
#include <stdbool.h>

#include <Ethernet2.h>
#include <EthernetUdp2.h>

//#include <basic_io_arm.h>
#include <FreeRTOS_ARM.h>

//#include <Melon_MS5607.h> //pressure sensor lib
#include <HDC2010.h> //humidity sensor lib
//#include <Temperature_LM75_Derived.h> //temperature sensor lib


/*Global variables*/
#define pumpPin 20
#define valve1 21
#define valve2 22
#define valve3 23
#define valve4 24
#define valve5 25
#define valve6 26
#define valve7 27
#define valve8 28
#define valve9 29
#define valve10 30
#define flushValve 31
#define CACvalve 32
#define htr1_pin 33
#define htr2_pin 34

#define nrTempSensors   6
#define nrPressSensors  6
#define nrHumidSensors  2
#define nrAirFSensors   1
int loremSensor;

SemaphoreHandle_t sem;
SemaphoreHandle_t semPeriodic;
static int8_t state;
static int samplingRate;
static float tempReading [nrTempSensors]; //array size might change according to the fix quantity
static float humReading [nrHumidSensors];
static float pressReading [nrPressSensors]; //array size might change according to the fix quantity
static float afReading[nrAirFSensors];
static int htrParameter[4];
static float ascParameter[20];
double status=0;

byte mac[] = { 0x2C, 0xF7, 0xF1, 0x08, 0x0F, 0x57 };
IPAddress ip(1, 1, 1, 1); //Local IPv4 adress
//IPAddress myDns(192,168,1, 1);
//IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 0, 0, 0);
IPAddress remote(1, 1, 1, 2);
unsigned int localPort = 8888;
EthernetUDP Udp;
EthernetServer server = EthernetServer(4000);

/*init*/
void setup()
{
    sem = xSemaphoreCreateMutex();
    semPeriodic = xSemaphoreCreateBinary();
    initAll();
    vTaskStartScheduler();

}

void loop() {
  // put your main code here, to run repeatedly:

}
