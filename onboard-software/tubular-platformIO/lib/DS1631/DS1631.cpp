#include "DS1631.h"

void DS1631::initDS1631(uint8_t ADDRESS)
{
  Wire.begin();

    Wire.beginTransmission(ADDRESS);
      Wire.write(0xAC); // 0xAC : Acces Config
      Wire.write(0x0C); // Continuous conversion & 12 bits resolution
    Wire.endTransmission();

    Wire.beginTransmission(ADDRESS);
      Wire.write((int)(0x51)); // Start Conversion
    Wire.endTransmission();
    // Serial.println(ADDRESS);
}


float DS1631::getTemperature(uint8_t ADDRESS) { 
        Wire.beginTransmission(ADDRESS);
            Wire.write((int)(0xAA));        // @AA : Temperature
        i2c_transmission = Wire.endTransmission();
        // Serial.print("I2C_transmission: "); Serial.println(i2c_transmission);
        
        if (i2c_transmission==0) {
            Wire.requestFrom(ADDRESS,2);        // READ 2 bytes
            delay(15);
            Wire.available();                 // 1st byte
                char msb = Wire.read();      // receive a byte
            Wire.available();                 // 2nd byte
                char lsb = Wire.read()>>4;      // receive a byte

            // T° processing, works for 12-bits resolution

            float tempCon =0;

            if (msb >= 0x80) { //if sign bit is set, msben temp is negative
              tempCon =  ((float)msb - 256) + ((float)lsb/16);
            }
            else 
            {  
              tempCon = (float)msb+(float)lsb/16;  
            }
            // Serial.print("Sensor number:"); Serial.print(TEMP_ADDR+i); Serial.print("    Temp con: "); Serial.print(tempCon); Serial.println(" C ");
            return tempCon;
          }
          else {
            initDS1631(ADDRESS);
            return -1000;
            // Serial.print("Error at: "); Serial.println(i);

           }

}