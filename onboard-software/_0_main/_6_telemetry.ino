/* Telemtery object
   Authors: BEXUS-TUBULAR software team

*/



void transmit() {
  std::vector<float>  tempData = readData(0);
  std::vector<float>  humidData = readData(1);
  std::vector<float>  pressData = readData(2);
  std::vector<float>  airFData = readData(3);

  Udp.beginPacket(remote, localPort);
  Udp.write("tub,ps");
  Udp.write(nrPressSensors);
  for (int i = 0; i < nrPressSensors; i++) //Loop number of time there are press sensors
  {
    Udp.write(pressData[i]);
   // Udp.write(",");
  }


  Udp.write("ts,");
  Udp.write(nrTempSensors);
  for (int i = 0; i < nrTempSensors; i++) //Loop number of time there are temp sensors
  {
    
    Udp.write(tempData[i]);
    Udp.write(",");
  }

  Udp.write("ts,");
  Udp.write(nrAirFSensors);
  for (int i = 0; i < nrAirFSensors; i++) //Loop number of time there are air flow sensors
  {
    Udp.write(airFData[i]);
    Udp.write(",");
  }

  Udp.write("ts,");
  Udp.write(nrHumidSensors);
  for (int i = 0; i < nrHumidSensors; i++) //Loop number of time there are humidity sensors
  {
    Udp.write((humidData[i]));
    Udp.write(",");
  }


  Udp.write("st,");
  for (int i = 0; i <= (htr2_pin - pumpPin) ; i++) //Loop number of time there are humidity sensors
  {
    status = status + status + digitalRead(pumpPin + i);
             //status << status;
  }
  Udp.write(status);
  Udp.write(getMode());
  Udp.write("\n");
  Udp.endPacket();


}
