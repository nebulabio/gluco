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
 *  Explanation: This example shows the way to send a text message with 
 *  the coporal temperature using the GPRS module. 
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
 *  Author: Ahmad Saad & Luis Martin
 */
 
// For more information about the GPRS shield please see our tutorial
// in cooking-hacks web site. http://www.cooking-hacks.com


#include <eHealth.h>

// the pin to switch on the module (without press on button) 
int pinModuleOn = 2;      

// ********* is the number to call
char phoneNumber[]="**********";    


void switchModule(){       
  digitalWrite(pinModuleOn,HIGH);
  delay(2000);
  digitalWrite(pinModuleOn,LOW);
}

void setup() {

  // UART baud rate
  Serial.begin(115200);               
  delay(2000);

  // Sitches the module ON
  // switchModulo();    

  for (int i=0;i < 5;i++){
    Serial.println("Push the button");
    delay(5000);
  }
  // Sets the SMS mode to text
  Serial.println("AT+CMGF=1");         
  delay(100);
}

void loop(){

  delay(100);
  float temperature = eHealth.getTemperature();     

  delay(1500);
  // send the SMS number 
  Serial.print("AT+CMGS=\"");     
  Serial.print(phoneNumber);
  Serial.println("\"");
  // the SMS body 
  while(Serial.read()!='>');      
  Serial.print(temperature);

  delay(1000);
  //sends ++
  Serial.write(0x1A);       
  Serial.write(0x0D);
  Serial.write(0x0A);
  delay(5000);

}



