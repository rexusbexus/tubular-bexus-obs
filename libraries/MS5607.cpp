#include <MS5607.h>
#include <SPI.h>


MS5607::MS5607(int PIN)
{
	_PIN=PIN; 
	pinMode(_PIN,OUTPUT); 
}

// Gives the reset command
void MS5607::reset_sequence()
{
	SPI.transfer(0x1E); 
}

// Conversion for D1 must be completed before ADC read
// 0x40 will give OSR=256, i=1->OSR=512, i=4->OSR=4096
// Note!: 0<=i<=4
void MS5607::convertionD1(int i) {
    uint16_t convertD1;
    convertD1 = 0x40+2*i;
    SPI.transfer(convertD1); 
}

// Conversion for D2 must be completed before ADC read
// 0x40 will give OSR=256, i=1->OSR=512, i=4->OSR=4096
// Note!: 0<=i<=4
void MS5607::convertionD2(int i) {
    uint16_t convertD2;
    convertD2 = 0x50+2*i;
    SPI.transfer(convertD2); 
}

// Read the PROM
// 0xA0 to 0xAE
// Note!: 0<=i<=7
// OBS! If one needs the entire 128b PROM one needs to 
// read all the bytes individually 
uint16_t MS5607::PROMread(uint8_t i)
{
    uint16_t PROMbyte;
    PROMbyte = SPI.transfer(0xA0+2*i);
    return PROMbyte;
}

// Read the ADC of the current selected chip
uint32_t MS5607::readADC() {
    uint32_t ADCvalue;
    ADCvalue = SPI.transfer(0x00);
    return ADCvalue;
}

uint32_t MS5607::readADC_calc(uint16_t PROMbyte[8], int i) {
    uint16_t PROMbyte[8];
    
    for (uint8_t k=0;k<=7;k++) {
        PROMbyte[k] = PROMread(k);
    }
    
    convertionD1(i);
    delay(1+2*i);
    uint32_t ADCpress;
    ADCpress = SPI.transfer(0x00);
    
    convertionD2(i);
    delay(1+2*i);
    uint32_t ADCtemp;
    ADCtemp = SPI.transfer(0x00);
    
    int32_t deltaT  = ADCtemp - PROMbyte[5]*2^8;
    int32_t TEMP    = 2000 + deltaT*PROMbyte[6]/2^23;
    
    int64_t OFFSET  = PROMbyte[2]*2^17 + PROMbyte[4]*deltaT/2^6;
    int64_t SENS    = PROMbyte[1]*2^16 + PROMbyte[3]*deltaT/2^7
    
    int32_t press   = (ADCpress*SENS/2^21 - OFFSET)/2^15;
    
    return press;
}
