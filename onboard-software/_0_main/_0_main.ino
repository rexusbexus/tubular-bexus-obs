<<<<<<< HEAD
#include <Arduino_FreeRTOS.h> //Free-RTOS lib
#include <semphr.h>
#include <task.h>
#include <SPI.h>
#include <stdbool.h>

#include <Melon_MS5607.h> //pressure sensor lib
#include <HDC2010.h> //humidity sensor lib
#include <Temperature_LM75_Derived.h> //temperature sensor lib
#include <utility/w5500.h>


/*Global variables*/
SemaphoreHandle_t sem = NULL;
static int state;
static int samplingRate;
static float tempReading [4]; //array size might change according to the fix quantity
static float humReading [1];
static float pressReading [2]; //array size might change according to the fix quantity
static int htrParameter[4];
static float ascParameter[20];


/*init*/
void setup()
{
    sem = xSemaphoreCreateBinary();
    Ethernet.begin(mac, ip, subnet);
    init();

}

void loop() {
  // put your main code here, to run repeatedly:

=======
#include <Arduino_FreeRTOS.h> //Free-RTOS lib
#include <semphr.h>
#include <task.h>
#include <SPI.h>
#include <stdbool.h>

#include <Melon_MS5607.h> //pressure sensor lib
#include <HDC2010.h> //humidity sensor lib
#include <Temperature_LM75_Derived.h> //temperature sensor lib


/*Global variables*/
SemaphoreHandle_t sem = NULL;
static int state;
static int samplingRate;
static float tempReading [4]; //array size might change according to the fix quantity
static float humReading [1];
static float pressReading [2]; //array size might change according to the fix quantity
static int htrParameter[4];
static float ascParameter[20];


/*init*/
void setup()
{
    sem = xSemaphoreCreateBinary();
    init();

}

void loop() {
  // put your main code here, to run repeatedly:

>>>>>>> parent of 156b0a9... Merge pull request #10 from mansyar/Gustav
}
