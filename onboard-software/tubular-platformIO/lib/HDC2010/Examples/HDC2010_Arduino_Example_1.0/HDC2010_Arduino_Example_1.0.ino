/*
 * HDC2010.ino
 * Created: August 1st 2017
 * This sketch is released AS-IS into the public domain, no guarantee or warranty is given.
 * This Code is not supported by Texas Instruments.
 * 
 * Description: This sketch configures the HDC2010 to monitor for a specific temperature 
 * and humidity range, with regular one second samples. The temperature and humidity readings
 * of the device are printed to a standard 16x2 LCD display. When the threshold values are
 * exceeded, the LCD will also have "ALT" printed in the bottom right corner. The schematic 
 * for this example is included alongside the HDC2010 Arduino Source Code.
 * 
 * Copyright (C) 2017 Texas Instruments Incorporated - http://www.ti.com/ 
 * 
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the   
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
*/

#include <HDC2010.h>
#include <LiquidCrystal.h>


#define ADDR 0x40
#define ALERTPIN 2
HDC2010 sensor(ADDR);

// Initialize the LCD Display  RS, E,  D4, D5, D6, D7
LiquidCrystal       lcd(      3,  5,  6,  9,  10, 11);
float temperature = 0, humidity = 0;
int alarm = 0;

void setup() {

  // Initialize I2C communication
  sensor.begin();
    
  // Begin with a device reset
  sensor.reset();
  
  // Set up the comfort zone
  sensor.setHighTemp(28);         // High temperature of 28C
  sensor.setLowTemp(22);          // Low temperature of 22C
  sensor.setHighHumidity(55);     // High humidity of 55%
  sensor.setLowHumidity(40);      // Low humidity of 40%
  

  // Configure interrupt pin
  sensor.enableInterrupt();                   // Enable the Interrupt/DRDY pin
  sensor.enableThresholdInterrupt();          // Enable Interrupt triggering based on comfort zone
  sensor.setInterruptPolarity(ACTIVE_HIGH);   // Set Interrupt pin to Active High
  sensor.setInterruptMode(COMPARATOR_MODE);   // Set Interrupt to return to inactive state when in bounds

  
  // Configure Measurements
  sensor.setMeasurementMode(TEMP_AND_HUMID);  // Set measurements to temperature and humidity
  sensor.setRate(ONE_HZ);                     // Set measurement frequency to 1 Hz
  sensor.setTempRes(FOURTEEN_BIT);
  sensor.setHumidRes(FOURTEEN_BIT);

  
  // Configure Arduino Uno Interrupt Pin
  pinMode(ALERTPIN, INPUT);                   // ALERTPIN will be the interrupt input
  attachInterrupt(digitalPinToInterrupt(ALERTPIN), alert, CHANGE);  //ISR "alert" will be triggered by changes in ALERTPIN

  // Initialize the LCD Display
  lcd.begin(16, 2);
  
  // Print temperature label
  lcd.setCursor(0,0);
  lcd.write("TEMP(C): ");
  
  // Print Humidity label
  lcd.setCursor(0,1);
  lcd.write("%RH: ");

  //begin measuring
  sensor.triggerMeasurement();
}

void loop() {

  // read temperature and humidity
  temperature = sensor.readTemp();
  humidity = sensor.readHumidity();

  // Print the current temperature to the right of the label
  lcd.setCursor(9,0);
  lcd.print(temperature);

  // Print the current humidity to the right of the label
  lcd.setCursor(6,1);
  lcd.print(humidity);

  // Wait 1 second for the next reading
  delay(1000);
  
}

// ISR to print alert on the LCD display
void alert(void) {

  //Load the current state of the Alert Pin to the alarm variable
  alarm = digitalRead(ALERTPIN);
  lcd.setCursor(13,1); //point to where we want to write on the LCD

  //If the alert pin was high, print ALT
  if (alarm)
    {
      lcd.print("ALT");
    }
  //Otherwise overwrite "ALT" with spaces
  else
    {
      lcd.print("   ");
    }

  
}

