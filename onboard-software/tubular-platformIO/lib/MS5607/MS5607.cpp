#include <MS5607.h>
#include <SPI.h>


MS5607::MS5607(int PIN)
{
	_PIN=PIN; 
	pinMode(_PIN,OUTPUT); 
}

// Gives the reset command
void MS5607::reset_sequence(int pinSelect)
{
    SPI.beginTransaction(SPISettings(10000, MSBFIRST, SPI_MODE0));
    digitalWrite(pinSelect, LOW);

	SPI.transfer(0x1E);

    digitalWrite(pinSelect, HIGH);
    SPI.endTransaction(); 
}

// Conversion for D1 must be completed before ADC read
// 0x40 will give OSR=256, i=1->OSR=512, i=4->OSR=4096
// Note!: 0<=i<=4
void MS5607::convertionD1(int i, int pinSelect) {
    SPI.beginTransaction(SPISettings(10000, MSBFIRST, SPI_MODE0));
    digitalWrite(pinSelect, LOW);

    uint8_t convertD1;
    convertD1 = 0x40+2*i;
    SPI.transfer(convertD1);

    digitalWrite(pinSelect, HIGH);
    SPI.endTransaction(); 
}

// Conversion for D2 must be completed before ADC read
// 0x40 will give OSR=256, i=1->OSR=512, i=4->OSR=4096
// Note!: 0<=i<=4
void MS5607::convertionD2(int i, int pinSelect) {
    SPI.beginTransaction(SPISettings(10000, MSBFIRST, SPI_MODE0));
    digitalWrite(pinSelect, LOW);

    uint8_t convertD2;
    convertD2 = 0x50+2*i;
    SPI.transfer(convertD2); 

    digitalWrite(pinSelect, HIGH);
    SPI.endTransaction();
}

// Read the PROM
// 0xA0 to 0xAE
// Note!: 0<=i<=7
// OBS! If one needs the entire 128b PROM one needs to 
// read all the bytes individually 
void MS5607::PROMread(int pinSelect)
{   
    PROMbyte[8] = {-1};
    byte ROMcommand = 0xA0;
    Serial.print("Sensor:"); Serial.println(pinSelect);
    for (uint8_t k = 0; k <= 7; k++) {

        SPI.beginTransaction(SPISettings(10000, MSBFIRST, SPI_MODE0));
        digitalWrite(pinSelect, LOW);

        PROMbyte[k] = SPI.transfer(ROMcommand + 2 * k);
        Serial.print("PROMbyte: buffer "); Serial.println(PROMbyte[k]);
        PROMbyte[k] = SPI.transfer16(0xFFFF);
        
        Serial.print("PROMbyte adress: "); Serial.print(ROMcommand + 2 * k); Serial.print(". Value: "); Serial.println(PROMbyte[k]);
         
    
        digitalWrite(pinSelect, HIGH);//chipSelectPin1
        SPI.endTransaction();
    }
        
        SPI.beginTransaction(SPISettings(10000, MSBFIRST, SPI_MODE0));
        digitalWrite(pinSelect, LOW);

        /*PROMbyte[0] = SPI.transfer(ROMcommand);
        //Serial.print("PROMbyte: buffer "); Serial.println(PROMbyte[0]);
        PROMbyte[0] = SPI.transfer16(0xFFFF);
        
        Serial.print("PROMbyte adress: "); Serial.print(ROMcommand); 
        Serial.print(". Value: "); Serial.println(PROMbyte[0]);
    
        digitalWrite(pinSelect, HIGH);//chipSelectPin1
        SPI.endTransaction();*/
}

// Read the ADC of the current selected chip
uint32_t MS5607::readADC(int pinSelect) {
    uint32_t ADCvalue;
    uint32_t ADCvalue_MSB;

    SPI.beginTransaction(SPISettings(10000, MSBFIRST, SPI_MODE0));
    digitalWrite(pinSelect, LOW);

    ADCvalue = SPI.transfer(0x00);
    ADCvalue_MSB = SPI.transfer16(0xFFFF);
    ADCvalue = SPI.transfer(0xFF);
    ADCvalue_MSB >> 8;
    ADCvalue = ADCvalue_MSB + ADCvalue;

    digitalWrite(pinSelect, HIGH);
    SPI.endTransaction();

    return ADCvalue;
}


void MS5607::ADC_calc(uint32_t ADCpress, uint32_t ADCtemp) {

    int32_t deltaT  = ADCtemp - PROMbyte[5] * 256;
    TEMP    = 2000 + deltaT * PROMbyte[6] / 8388608;

    long int OFFSET  = (PROMbyte[2] * 131072) + (PROMbyte[4] * deltaT) / (64);
    if (OFFSET > 25769410560) { //min and max have to be defined per datasheet.
      //OFFSET = 25769410560;
    } else if (OFFSET < -17179344900) {
      //OFFSET = -17179344900;
    }

    long int SENS    = (PROMbyte[1] * 65536) + (PROMbyte[3] * deltaT) / (128);
    if (SENS > 12884705280) { //min and max have to be defined per datasheet.
      // SENS = 12884705280;
    } else if (OFFSET < -8589672450) {
      // SENS = -8589672450;
    }

    pres   = ((ADCpress * SENS / 2097152) - OFFSET) / (32768);
	
	/*double T2=0., OFF2=0., SENS2=0.;
	if(TEMP<2000) {
	  T2=deltaT*deltaT/pow(2,31);
	  OFF2=61*(TEMP-2000)*(TEMP-2000)/pow(2,4);
	  SENS2=2*(TEMP-2000)*(TEMP-2000);
	  if(TEMP<-1500) {
	    OFF2+=15*(TEMP+1500)*(TEMP+1500);
	    SENS2+=8*(TEMP+1500)*(TEMP+1500);
	  }
	}
	
	TEMP-=T2;
	OFFSET-=OFF2;
	SENS-=SENS2;
	pres=(((ADCpress*SENS)/pow(2,21)-OFFSET)/pow(2,15));*/
	
  }
  
int32_t MS5607::getTemp()
{
	return TEMP;
}

int32_t MS5607::getPres()
{
	return pres;
}