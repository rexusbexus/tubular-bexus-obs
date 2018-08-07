/* Name: heaterLogic.ino.cpp
 * Purpose: To compare temperature with parameter values
 * and then return if they should be on/off.
 * Project: Tubular-Bexus.
 * Authors: Tubular-Bexus software group.
*/
#include <vector>
#include <Arduino.h>
#include "heaterLogic.h"
//#include <ArduinoSTL.h>
//#include <stdbool.h>
//#include "_4_heater.h"
#include "_5_asc.h"

std::vector<float> htrParam(4);
std::vector<float> tempAtHtr(2);



heater heaterCompare(){
//Compares with parameters
//bool htr1_flag;
//bool htr2_flag;
//bool *htr_flag[2];
    Serial.println("I'm at heaterCompare");
    heater htr_flag_struc;

    if(digitalRead(pumpPin)==1){
        //As long the pump is is on keep the heaters off.
        htr_flag_struc.htr1_flag = 0;
        htr_flag_struc.htr2_flag = 0;
        return htr_flag_struc;
    }
    if (tempAtHtr[0]<=-80 || tempAtHtr[1]>=100 || tempAtHtr[2]<=-80 || tempAtHtr[3]>=100)
    {
      /*
       * Check if within correct value of parameters
       * Also works for errors in tempAtHtr[].
       */
    }    
    else if (htrParam[0]>=tempAtHtr[0])
    {
        /*  IF parameter is larger or equal to 
         *  the temperature at heater 1 turn on 
         *  the heater 1.
         */
        htr_flag_struc.htr1_flag = 1;
    }
    else if ((htrParam[1])<=tempAtHtr[0])
    {   
        /*  IF parameter is smaller or equal to 
         *  the temperature at heater 1 turn off 
         *  the heater 1.
         */
        htr_flag_struc.htr1_flag = 0;
    }
    else if ((htrParam[2])>=tempAtHtr[1])
    {
        /*  IF parameter is larger or equal to 
         *  the temperature at heater 2 turn on 
         *  the heater 2.
         */
        htr_flag_struc.htr2_flag = 1;
    }
    else if ((htrParam[3])<=tempAtHtr[1])
    {
        /*  IF parameter is smaller or equal to 
         *  the temperature at heater 2 turn off 
         *  the heater 2.
         */
        htr_flag_struc.htr2_flag = 0;
    }
    else
    {
        /* If error occurs turn off heater.
         */  
        htr_flag_struc.htr1_flag = 0;
        htr_flag_struc.htr2_flag = 0;
    }


  Serial.println("Leaving heaterCompare");
  return htr_flag_struc;
}