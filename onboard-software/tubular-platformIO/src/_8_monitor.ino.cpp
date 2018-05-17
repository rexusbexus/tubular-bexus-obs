#include <Arduino.h>
#define WDT_KEY (0xA5)
/********************************************************************************
  extern "C" void _watchdogDefaultSetup (void) { WDT_Disable (WDT); }

  void watchdogSetup (void) __attribute__ ((weak, alias("_watchdogDefaultSetup")));
*********************************************************************************/

/*This function is called from init(). If the user does not provide
  this function, then the default action is to disable watchdog.
  This function has to be overriden, otherwise watchdog won't work !! */

void watchdogSetup(void) {
  /*** watchdogDisable (); ***/
}

void initWatchdog()
{
  // Enable watchdog.
  WDT->WDT_MR = WDT_MR_WDD(0xFFF)
                | WDT_MR_WDRPROC
                | WDT_MR_WDRSTEN
                | WDT_MR_WDV(256 * 7); // Watchdog triggers a reset after 7 seconds if underflow
  // 2 seconds equal 84000000 * 2 = 168000000 clock cycles
  /* Slow clock is running at 32.768 kHz
    watchdog frequency is therefore 32768 / 128 = 256 Hz
    WDV holds the periode in 256 th of seconds  */

  //Serial.begin(9600);
  //uint32_t status = (RSTC->RSTC_SR & RSTC_SR_RSTTYP_Msk) >> 8; // Get status from the last Reset
  //Serial.print("RSTTYP = 0b"); Serial.println(status, BIN);  // Should be 0b010 after first watchdog reset
}

int flag[4] = {0};
/* It exists a number places where the flags are set
 * Flag 0: _3_Sensor.ino.cpp
 * Flag 1: _4_heater.ino.cpp
 * Flag 2: _5_asc.ino.cpp
 * Flag 3: _7_telecommand.ino.cpp
*/
void flagPost(int flagNr) {
  flag[flagNr] = 1;
  if (flag[0] == 1 && flag[1] == 1 && flag[2] == 1 && flag[3] == 1) {
    flag[0] = 0;
    flag[1] = 0;
    flag[2] = 0;
    flag[3] = 0;
    WDT->WDT_CR = WDT_CR_KEY(WDT_KEY)
                  | WDT_CR_WDRSTT;
  }
}