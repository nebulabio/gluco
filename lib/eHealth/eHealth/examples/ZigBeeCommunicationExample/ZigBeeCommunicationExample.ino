/*
 *  eHealth sensor platform for Arduino and Raspberry from Cooking-hacks.
 *
 *  Description: "The e-Health Sensor Shield allows Arduino and Raspberry Pi 
 *  users to perform biometric and medical applications by using 9 different 
 *  sensors: Pulse and Oxygen in Blood Sensor (SPO2), Airflow Sensor (Breathing),
 *  Body Temperature, Electrocardiogram Sensor (ECG), Glucometer, Galvanic Skin
 *  Response Sensor (GSR - Sweating), Blood Pressure (Sphygmomanometer) and 
 *  Patient Position (Accelerometer)."  
 *
 *  Explanation: This example shows the way to communicate with  
 *  the Arduino using ZigBee protocol. 
 *
 *  Copyright (C) 2012 Libelium Comunicaciones Distribuidas S.L.
 *  http://www.libelium.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Version 0.1
 *  Author: Luis Martin & Ahmad Saad 
 */
 
 
#include <PinChangeInt.h>
#include <eHealth.h>

char recv[128];
uint8_t cont = 0;

//  Note :  The Xbee modules must be configured previously. 
//  See the next link http://www.cooking-hacks.com/index.php/documentation/tutorials/arduino-xbee-shield 

void setup()
{ 
  Serial.begin(9600);
  
  eHealth.initPulsioximeter();
  eHealth.initPositionSensor(); 
  
  //Attach the inttruptions for using the pulsioximeter.   
  PCintPort::attachInterrupt(6, readPulsioximeter, RISING);
  delay(1000);
  
}

void loop()
{    
      
      //1. Read from eHealth. 
       int airFlow = eHealth.getAirFlow();     
       float temperature = eHealth.getTemperature();     
       float conductance = eHealth.getSkinConductance();   
       float resistance = eHealth.getSkinResistance();
       int BPM = eHealth.getBPM();     
       int SPO2 = eHealth.getOxygenSaturation();     
       uint8_t pos = eHealth.getBodyPosition();      
    

      Serial.print(int(airFlow));     Serial.print("#");
      Serial.print(temperature);      Serial.print("#");
      Serial.print(int(BPM));         Serial.print("#");
      Serial.print(int(SPO2));        Serial.print("#");
      Serial.print(conductance);      Serial.print("#");
      Serial.print(int(resistance));  Serial.print("#");
      Serial.print(int(pos));         Serial.print("#");
      Serial.print("\n");    
    
      // Reduce this delay for more data rate
      delay(250);   
}

void check(){
  cont=0; delay(500);
  while (Serial.available()>0)
  {
     recv[cont]=Serial.read(); delay(10);
     cont++;
  }
  recv[cont]='\0';
  Serial.println(recv);
  Serial.flush(); delay(100);
}



//Include always this code when using the pulsioximeter sensor
//=========================================================================
void readPulsioximeter(){  
  
  cont ++;
 
  if (cont == 50) { //Get only one of 50 measures to reduce the latency
    eHealth.readPulsioximeter();  
    cont = 0;
  }
}

