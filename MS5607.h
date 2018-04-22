#ifndef MS5607_h
#define MS5607_h

#include <Arduino.h>
#include <SPI.h>

class MS5607
{
//reset_sequence()
    protected:
        int _PIN;
        
    Public:
        MS5607(int PIN);
        void reset_sequence();
        void convertionD1(int i);
        void convertionD2(int i);
        uint16_t PROMread(uint8_t i);
        uint132_t readADC();
        uint32_t readADC_calc(uint16_t PROMbyte[8], int i)
    
    
};

#endif
