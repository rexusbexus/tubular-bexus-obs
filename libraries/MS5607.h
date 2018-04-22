#ifndef MS5607_h
#define MS5607_h

#include <Arduino.h>
#include <SPI.h>

class MS5607
{
//reset_sequence()
    protected:
        
    Public:
        void reset_sequence();
        void convertionD1(int i);
        void convertionD2(int i);
        uint16_t PROMread(uint8_t i);
        uint132_t readADC();
    
    
};

#endif
