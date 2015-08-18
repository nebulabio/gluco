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
 *  the Arduino Wifi Demo Android app. 
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
float parameter = 0.098;
uint8_t cont = 0;


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
  while (Serial.available()>0) {}
  // Enters in command mode    
    Serial.print("$$$"); check();  
  // Sets DHCP and TCP protocol
    Serial.print("set ip dhcp 1\r"); check();
    Serial.print("set ip protocol 1\r"); check();
  // Configures the way to join the network AP
    Serial.print("set wlan join 0\r"); check();
    Serial.print("join ANDROID\r"); check();
  
    Serial.print("set i h 255.255.255.255\r"); delay(1000);
    
    Serial.print("set i r 12345\r"); check();
    Serial.print("set i l 2000\r"); check();
    Serial.print("exit\r"); check();

    while(1){     
    
      eHealth.initBloodPressureSensor(0.098);
      
      //1. Read from eHealth. 
       int airFlow = eHealth.getAirFlow();     
       float temperature = eHealth.getTemperature();     
       float conductance = eHealth.getSkinConductance();   
       float resistance = eHealth.getSkinResistance();
       float conductanceVol = eHealth.getSkinConductanceVoltage();
       int BPM = eHealth.getBPM();     
       int SPO2 = eHealth.getOxygenSaturation();     
       uint8_t pos = eHealth.getBodyPosition();      
       int syst = eHealth.getSystolicPressure();
       int diast = eHealth.getDiastolicPressure();     
       float ECG = eHealth.getECG();
       //uint8_t glucose = eHealth.glucoseDataVector[0].glucose;      

      //Data sensor must be sent in this order to mobile android application 
      Serial.print(int(airFlow));     Serial.print("#");
      Serial.print(ECG);              Serial.print("#");
      Serial.print(syst);             Serial.print("#");
      Serial.print(diast);            Serial.print("#");
      Serial.print(int(0));           Serial.print("#"); //Glucose is not implemented yet
      Serial.print(temperature);      Serial.print("#");
      Serial.print(int(BPM));         Serial.print("#");
      Serial.print(int(SPO2));        Serial.print("#");
      Serial.print(conductance);      Serial.print("#");
      Serial.print(int(resistance));  Serial.print("#");
      Serial.print(int(airFlow));     Serial.print("#");
      Serial.print(int(pos));         Serial.print("#");
      Serial.print("\n");    
    
      // Reduce this delay for more data rate
      delay(250);  
  }
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

