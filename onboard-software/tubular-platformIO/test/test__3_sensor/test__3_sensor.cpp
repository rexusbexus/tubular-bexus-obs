#include <Arduino.h>
#include "unity.h"
#include "_3_sensor.h"
#include "sensorManager.h"
#include <FreeRTOS_ARM.h>
#include <SD.h>



#ifdef UNIT_TEST

extern float tempReading [nrTempSensors]; //array size might change according to the fix quantity
extern float humReading [nrHumidSensors];
extern float pressReading [nrPressSensors]; //array size might change according to the fix quantity
extern float afReading[nrAirFSensors];

SemaphoreHandle_t sem;

void test_writeData()
{
   float curPressureMeasurement[] = {400,400,400,400,400,400};
   float curTemperatureMeasurement[] = {20,20,20,20,20,20,20,20,20};
   float curHumMeasurement[] = {40};
   float curAFMeasurement[] = {300};
   /*Save pressure measurements*/
    writeData(curPressureMeasurement, 2);

    /*Save temperature measurements*/
    writeData(curTemperatureMeasurement, 0);

    /*Save humidity measurements*/
    writeData(curHumMeasurement, 1);

    /*Save humidity measurements*/
    writeData(curAFMeasurement, 3);
    TEST_ASSERT_EQUAL_FLOAT(20, tempReading[0]);
    TEST_ASSERT_EQUAL_FLOAT(40, humReading[0]);
    TEST_ASSERT_EQUAL_FLOAT(400, pressReading[0]);
    TEST_ASSERT_EQUAL_FLOAT(300, afReading[0]);
}

// void test_savingDataToSDCard_function()
// {
//    float curPressureMeasurement[] = {400,400,400,400,400,400};
//    float curTemperatureMeasurement[] = {20,20,20,20,20,20,20,20,20};
//    float curHumMeasurement[] = {40};
//    float curAFMeasurement[] = {300};
//    savingDataToSD(curTemperatureMeasurement, curHumMeasurement, curPressureMeasurement, curAFMeasurement);
//    File datalog = SD.open("datalog.txt", FILE_READ);

   
// }

void setup()
{
    sem = xSemaphoreCreateMutex();
    pinMode(13, OUTPUT);
    UNITY_BEGIN();
    RUN_TEST(test_writeData);
    UNITY_END();
}

void loop()
{
    digitalWrite(13, HIGH);
    delay(500);
    digitalWrite(13, LOW);
    delay(500);
}

#endif