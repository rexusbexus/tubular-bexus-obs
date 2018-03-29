

#define pumpPin 20
#define valve1 21
#define valve2 22
#define valve3 23
#define valve4 24
#define valve5 25
#define valve6 26
#define valve7 27
#define valve8 28
#define valve9 29
#define valve10 30
#define flushValve 31
#define CACvalve 32



void initASC()
{
  initReading();
  initPumpControl)();
  initValvesControl();
}

void initReading()
{
  xTaskCreate(
    reading
    ,  (const portCHAR *) "reading";   // Name
    ,  128  // This stack size 
    ,  NULL
    ,  1  // Priority
    ,  NULL );
}

void initPumpControl()
{
  pinMode(pumpPin, OUTPUT);
}

void initValvesControl()
{
  pinMode(valve1, OUTPUT);
  pinMode(valve2, OUTPUT);
  pinMode(valve3, OUTPUT);
  pinMode(valve4, OUTPUT);
  pinMode(valve5, OUTPUT);
  pinMode(valve6, OUTPUT);
  pinMode(valve7, OUTPUT);
  pinMode(valve8, OUTPUT);
  pinMode(valve9, OUTPUT);
  pinMode(valve10, OUTPUT);
  pinMode(flushValve, OUTPUT);
  pinMode(CACvalve, OUTPUT);
}

void reading()
{
   
   float *ascParam;
   int bagcounter = 1;
   float *currPressure;
   float meanPressure;
   
   TickType_t xLastWakeTime;
   xLastWakeTime = xTaskGetTickCount ();

   while(1)
   {
     static int currMode = getMode();
     
     ascParam = getASCParameter(bagcounter);
     currPressure = readData(2);

     /*Calculating mean pressure from several pressure sensors*/
     meanPressure = (currPressure + (currPressure+1))/2;

     switch (currMode){
     /*Standby*/
     case standbyMode:
     break;
     /*Normal - Ascent*/
     case normalAscent:
     digitalWrite(CACvalve, HIGH);
     if (meanPressure >= ascParam && meanPressure<= (ascParam+1))
     {
        valvesControl(11, 1); //delay(10);
        pumpControl(1); //delay(1000);
        valvesControl(11, 0); //delay(10);
        valvesControl(bagcounter, 1); //delay(10); 
     }
     else
     {
        pumpControl(0); //delay(100);
        valvesControl(bagcounter, 0); //delay(10);
        if (bagcounter<10 && meanPressure>=(ascParam+1)){
          //bagcounter++;  // need to figure this out later
        }
     }
     break;
     /*Normal - Descent*/
     case normalDescent:
     if (meanPressure >= ascParam && meanPressure<= (ascParam+1))
     {
        valvesControl(11, 1); //delay(10);
        pumpControl(1);
        valvesControl(11, 0); //delay(10);
        valvesControl(bagcounter, 1); //delay(10);   
     }
     else
     {
        pumpControl(0); //delay(100);
        valvesControl(bagcounter, 0); //delay(10);
        if (bagcounter<10 && meanPressure<= ascParam){
          bagcounter++;
        }
     }
     break;
     /*SAFE*/
     case safeMode:
     digitalWrite(CACvalve, LOW);
     for (bagcounter=1;bagcounter<=10;bagcounter++)
     {
       pumpControl(0);
       valvesControl(bagcounter, 0);
     }
     break;
   }
   vTaskDelayUntil(&xLastWakeTime, (500 / portTICK_PERIOD_MS) );
   }
}


float* getASCParam(int bag)
{
  float dummyParameter[2]
  xSemaphoreTake(sem, portMAX_DELAY);
  dummyParameter[0] = ascParameter[(bag*2)-2];
  dummyParameter[1] = ascParameter[(bag*2)-1];
  xSemaphoreGive(sem);
  return dummyParameter
}

void pumpControl(int cond)
{
  xSemaphoreTake(sem, portMAX_DELAY);
  switch (cond){
    case 0:
    digitalWrite(pumpPin, LOW);
    break;

    case 1:
    digitalWrite(pumpPin, HIGH);
    break;
  }
  xSemaphoreGive(sem);
}

void valvesControl(int valve, int cond)
{
  xSemaphoreTake(sem, portMAX_DELAY);
  if (cond==1){
  switch (valve){
    case 1:
    digitalWrite(valve1, HIGH);
    break;

    case 2:
    digitalWrite(valve2, HIGH);
    break;

    case 3:
    digitalWrite(valve3, HIGH);
    break;

    case 4:
    digitalWrite(valve4, HIGH);
    break;

    case 5:
    digitalWrite(valve5, HIGH);
    break;

    case 6:
    digitalWrite(valve6, HIGH);
    break;

    case 7:
    digitalWrite(valve7, HIGH);
    break;

    case 8:
    digitalWrite(valve8, HIGH);
    break;

    case 9:
    digitalWrite(valve9, HIGH);
    break;

    case 10:
    digitalWrite(valve10, HIGH);
    break;

    case 11:
    digitalWrite(flushValve, HIGH);
    break;
  }
  }
  else {
    switch (valve){
    case 1:
    digitalWrite(valve1, LOW);
    break;

    case 2:
    digitalWrite(valve2, LOW);
    break;

    case 3:
    digitalWrite(valve3, LOW);
    break;

    case 4:
    digitalWrite(valve4, LOW);
    break;

    case 5:
    digitalWrite(valve5, LOW);
    break;

    case 6:
    digitalWrite(valve6, LOW);
    break;

    case 7:
    digitalWrite(valve7, LOW);
    break;

    case 8:
    digitalWrite(valve8, LOW);
    break;

    case 9:
    digitalWrite(valve9, LOW);
    break;

    case 10:
    digitalWrite(valve10, LOW);
    break;

    case 11:
    digitalWrite(flushValve, LOW);
    break;
  }
  xSemaphoreGive(sem);
}
}
