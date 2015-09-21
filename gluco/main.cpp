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
 *  In this example we are going to get data stored in the glucometer 
 *  memory and show the result in the serial monitor.   
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
 *  Author: Ahmad Saad  & Luis Martin
 */

#include <eHealth.h>

void setup() { 

     eHealth.readGlucometer();
     Serial.begin(115200);
     delay(100);
}

void loop() { 

     uint8_t numberOfData = eHealth.getGlucometerLength();
     Serial.print(F("Number of measures : "));
     Serial.println(numberOfData, DEC);
     delay(100);


     for (int i = 0; i<numberOfData; i++) {
          // The protocol sends data in this order
          Serial.println(F("=========================================="));

          Serial.print(F("Measure number "));
          Serial.println(i + 1);

          Serial.print(F("Date -> "));
          Serial.print(eHealth.glucoseDataVector[i].day);
          Serial.print(F(" of "));
          Serial.print(eHealth.numberToMonth(eHealth.glucoseDataVector[i].month));
          Serial.print(F(" of "));
          Serial.print(2000 + eHealth.glucoseDataVector[i].year);
          Serial.print(F(" at "));

          if (eHealth.glucoseDataVector[i].hour < 10) {
               Serial.print(0); // Only for best representation.
          }

          Serial.print(eHealth.glucoseDataVector[i].hour);
          Serial.print(F(":"));

          if (eHealth.glucoseDataVector[i].minutes < 10) {
               Serial.print(0);// Only for best representation.
          }
          Serial.print(eHealth.glucoseDataVector[i].minutes);

          if (eHealth.glucoseDataVector[i].meridian == 0xBB)
               Serial.println(F(" pm"));
          else if (eHealth.glucoseDataVector[i].meridian == 0xAA)
               Serial.println(F(" am"));

          Serial.print(F("Glucose value : "));
          Serial.print(eHealth.glucoseDataVector[i].glucose);
          Serial.println(F(" mg/dL"));
     }

     delay(20000);
}