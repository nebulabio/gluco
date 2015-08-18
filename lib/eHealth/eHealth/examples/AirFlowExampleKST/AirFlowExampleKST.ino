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
 *  In this example we read the value of the air flow sensor 
 *  and print the air Flow wave form using KST.
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

#include <eHealth.h>

extern volatile unsigned long timer0_overflow_count;
float fanalog0;
int analog0;
unsigned long time;
byte serialByte;

void setup() {
  Serial.begin(9600);
  Serial.println("Starting...");
}

void loop() {  
  while (Serial.available()>0) {
    serialByte=Serial.read();

    if (serialByte=='C'){
      while(1) {
        fanalog0=eHealth.getAirFlow();  
        // Use the timer0 => 1 tick every 4 us
        time=(timer0_overflow_count << 8) + TCNT0;
        // Microseconds conversion.
        time=time*4;
        //Print in a file for simulation
        Serial.print(time);
        Serial.print(";");
        Serial.println(fanalog0,5);
        if (Serial.available()>0){
          serialByte=Serial.read();
          if (serialByte=='F') break;
        }
      }
    }
  }
}

