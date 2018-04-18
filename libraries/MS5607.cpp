#include <MS5607.h>

// Gives the reset command
void MS5607::reset_sequence()
{
	SPI.transfer(0x1E); 
}

// Conversion for D1 must be completed before ADC read
// 0x40 will give OSR=256, i=1->OSR=512, i=4->OSR=4096
// Note!: 0<=i<=4
void MS5607::convertionD1(int i) {
    convertD1 = 0x40+2*i;
    SPI.transfer(convertD1); 
}

// Conversion for D2 must be completed before ADC read
// 0x40 will give OSR=256, i=1->OSR=512, i=4->OSR=4096
// Note!: 0<=i<=4
void MS5607::convertionD2(int i) {
    convertD2 = 0x50+2*i;
    SPI.transfer(convertD2); 
}

// Read the PROM
// 0xA0 to 0xAE
// Note!: 0<=i<=7
uint16_t MS5607::PROMread(uint8_t i)
{
    PROMbyte = SPI.transfer(0xA0+2*i);
    return PROMbyte;
}

// Read the ADC
uint32_t MS5607::readADC() {
    ADCvalue = SPI.transfer(0x00);
    return ADCvalue;
}


