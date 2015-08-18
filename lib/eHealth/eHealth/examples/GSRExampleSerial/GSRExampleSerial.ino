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
 *  In this example we use the skin sensor to measure some 
 *  parameters like the skin resistance and coductance. 
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
 *  along with this program.  If not, see <http://www.gnu.org/licenses/> .
 *
 *  Version 0.1
 *  Author: Luis Martin & Ahmad Saad 
 */

#include <eHealth.h> 

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(115200);  
}

// the loop routine runs over and over again forever:
void loop() {

  float conductance = eHealth.getSkinConductance();
  float resistance = eHealth.getSkinResistance();
  float conductanceVol = eHealth.getSkinConductanceVoltage();

  Serial.print("Conductance : ");       
  Serial.print(conductance, 2);  
  Serial.println("");         

  Serial.print("Resistance : ");       
  Serial.print(resistance, 2);  
  Serial.println("");    

  Serial.print("Conductance Voltage : ");       
  Serial.print(conductanceVol, 4);  
  Serial.println("");

  Serial.print("\n");

  // wait for a second  
  delay(1000);            
}




