/* Telemtery object
 * Authors: BEXUS-TUBULAR software team
 * 
 */

 

void transmit() {
  tempData = readData(0);
  humidData = readData(1);
  PressData = readData(2);
  airFData = readData(3);
  
  Udp.beginPacket(remote, localPort);
  tempSensor=nrPressSensors;
  Udp.write("tub,ps",tempSensor,",");
  for (int i=0;i<tempSensor-1;i++) //Loop number of time there are press sensors
  {
    Udp.write(*(pressData+i));
    Udp.write(",");
  }

  loremSensor=nrTempSensors;
  Udp.write("ts,",loremSensor,",");
  for (int i=0;i<nrloremSensors-1;i++) //Loop number of time there are temp sensors
  {
    Udp.write(*(tempData+i));
    Udp.write(",");
  }

  Udp.write("ts,",nrAirFSensors,",");
  for (int i=0;i<airFSensors-1;i++) //Loop number of time there are air flow sensors
  {
    Udp.write(*(airFData+i));
    Udp.write(",");
  }

  Udp.write("ts,",nrHumidSensors,",");
  for (int i=0;i<humidSensors-1;i++) //Loop number of time there are humidity sensors
  {
    Udp.write(*(humidData+i));
    Udp.write(",");
  }

  
  Udp.write("st,");
  for (int i=0;i<= (htr2_pin - pumpPin) ;i++) //Loop number of time there are humidity sensors
  {
    status = status + digitalRead(pumpPin+i)
    status << status;
  }
  Udp.write(status);
  Udp.write(getMode());
  Udp.write("\n");
  Udp.endPacket();
  

}

void loop() {
  // put your main code here, to run repeatedly:

} 
