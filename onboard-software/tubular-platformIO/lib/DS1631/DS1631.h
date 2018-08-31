#ifndef DS1631_h
#define DS1631_h


#define TEMP_ADDR (0x90 >> 1) 
#include <Wire.h>

class DS1631
{
    private:
    uint8_t ADDRESS;
    int i2c_transmission = 5;


    public:
    void initDS1631(uint8_t ADDRESS);
    float getTemperature(uint8_t ADDRESS);

};
#endif
