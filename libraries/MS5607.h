#ifndef MS5607_h
#define MS5607_h

#include <Arduino.h>
#include <SPI.h>

class MS5607
{
//reset_sequence()
    private:
        int _PIN;
        float pressure;
        int i;
        int pinSelect;
        uint8_t convertD1;
        uint8_t convertD2;
        
    public:
        MS5607(int PIN);
        void reset_sequence(int pinSelect);
        void convertionD1(int i, int pinSelect);
        void convertionD2(int i, int pinSelect);
        void PROMread(int pinSelect);
        uint32_t readADC(int pinSelect);
        void ADC_calc(uint32_t ADCpress, uint32_t ADCtemp);
        //float getPressure(int i);
        uint16_t PROMbyte[8];
        int32_t TEMP;
        int32_t pres;
        uint32_t ADCpress;
        uint32_t ADCtemp;

    
    
};

#endif
