#include <vector>
#include <Arduino.h>
#include "heaterLogic.h"
//#include <ArduinoSTL.h>
//#include <stdbool.h>
//#include "_4_heater.h"

std::vector<float> htrParam(4);
std::vector<float> tempAtHtr(2);



heater heaterCompare(){
//Compares with parameters
//bool htr1_flag;
//bool htr2_flag;
//bool *htr_flag[2];
Serial.println("I'm at heaterCompare");
heater htr_flag_struc;
htr_flag_struc.htr1_flag=0;

    if (tempAtHtr[0]<=0 || tempAtHtr[0]==16777215 || tempAtHtr[1]<=0 || tempAtHtr[1]>=16777215)
    {
      /*
       * Check if within correct value of parameters
       * Also works for errors in tempAtHtr[]
       */
    }    
    else if (htrParam[0]>=tempAtHtr[0])
    {
        htr_flag_struc.htr1_flag = 1;
    }
    else if ((htrParam[1])<=tempAtHtr[0])
    {
        htr_flag_struc.htr1_flag = 0;
    }
    else if ((htrParam[2])>=tempAtHtr[1])
    {
        htr_flag_struc.htr2_flag = 1;
    }
    else if ((htrParam[3])<=tempAtHtr[1])
    {
        htr_flag_struc.htr2_flag = 0;
    }
    else
    {
        htr_flag_struc.htr1_flag = 0;
        htr_flag_struc.htr2_flag = 0;
    }

    // if (htr_flag_struc.htr1_flag ^ htr_flag_struc.htr2_flag)  // if 0 1 or 1 0
    // {
    //   //heaterControl(htr1_flag,htr2_flag);
    // }
    // else if (htr_flag_struc.htr1_flag && htr_flag_struc.htr2_flag) // if 1 1
    // {
    //   /*
    //    * If both heaters require to be turned on it will choose the one most
    //    * largest difference between their paramters and actual temperature.
    //    */
    //   if (((htrParam[0]) - tempAtHtr[0])>( (htrParam[2]) - tempAtHtr[1]))
    //   {
    //     //heaterControl(1,0);
    //     htr_flag_struc.htr1_flag = 1;
    //     htr_flag_struc.htr2_flag = 0;
    //   }
    //   else if (((htrParam[0]) - tempAtHtr[0])<( (htrParam[2]) - tempAtHtr[1]))
    //   {
    //     //heaterControl(0,1);
    //     htr_flag_struc.htr1_flag = 0;
    //     htr_flag_struc.htr2_flag = 1;
    //   }
    //   else // if 0 0
    //   {
    //     //heaterControl(htr1_flag,htr1_flag);
    //     htr_flag_struc.htr1_flag = 0;
    //     htr_flag_struc.htr2_flag = 0;
    //     /*
    //     * The heaters does not need to operate
    //     */
    //   }
      
    // } 

  //htr_flag[0] = &htr1_flag;
  //htr_flag[1] = &htr1_flag;
  Serial.println("Leaving heaterCompare");
  return htr_flag_struc;
}