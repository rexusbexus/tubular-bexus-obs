
#include <ArduinoSTL.h>
#include <vector>

#include <Wire.h>
#include <SD.h>

#include <SPI.h>
#include <stdbool.h>

#include <Ethernet2.h>
#include <EthernetUdp2.h>

#include <FreeRTOS_ARM.h>

#include <M2M_LM75A.h>
#include <MS5611.h>
#include <MS5xxx.h>
#include <AWM43300V.h>
#include <HDC2010.h> //humidity sensor lib
#include <RTCDue.h>



/*Global variables*/
/*
 * Pin 20, 21 reserved for SDA and SCL for I2C connection with temperature sensors and humidity sensor
 */
#define pumpPin 22
#define valve1 23
#define valve2 24
#define valve3 25
#define valve4 26
#define valve5 27
#define valve6 28
#define valve7 29
#define valve8 30
#define valve9 31
#define valve10 32
#define flushValve 33
#define CACvalve 34
#define htr1_pin 35
#define htr2_pin 36
/*
   Pressusre sensors and  will use MOSI-SDI, MISO-SDO, SCK-SCLK, and digital pins-(CSB)chipselect pins.
*/
/*The followings are chipselect pins */
#define pressSensorPin1 37
#define pressSensorPin2 38
#define pressSensorPin3 39
#define pressSensorPin4 40
#define pressSensorPin5 41
#define pressSensorPin6 42
/*Airflow sensor will be connected to pin 43 */
#define airFsensorPin   A10
#define sdPin           10

#define nrTempSensors   9
#define nrPressSensors  6
#define nrHumidSensors  1
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
//static int htrParameter[4];
static float ascParameter[16];
int16_t status=0;

byte mac[] = { 0x2C, 0xF7, 0xF1, 0x08, 0x0F, 0x57 };
IPAddress ip(1, 1, 1, 1); //Local IPv4 adress
//IPAddress myDns(192,168,1, 1);
//IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 0, 0, 0);
IPAddress remote(1, 1, 1, 2);
unsigned int localPort = 8888;
EthernetUDP Udp;
EthernetServer server = EthernetServer(4000);

typedef union {
    float val;
    uint8_t bytes[4];
} floatval;

/*init*/
void setup()
{
    Serial.begin(9600);
    server.begin();
    sem = xSemaphoreCreateMutex();
    semPeriodic = xSemaphoreCreateBinary();
    initAll();
    vTaskStartScheduler();

    RTCDue rtc(XTAL);//Init start RTC object.
    rtc.begin();
    rtc.setTime(0,0,0);
    rtc.setDate(1,1,2018);
}

void loop() {
  // put your main code here, to run repeatedly:

}
