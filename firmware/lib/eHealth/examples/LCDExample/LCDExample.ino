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
 *  In this example we can view all data measures in the LCD, choosing
 *  between 3 screens by a push button integrated in the eHealth board. 
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

#include <PinChangeInt.h>
#include <eHealthDisplay.h>
#include <eHealth.h>

#define pushButton 4

uint8_t screenNumber = 1; 
uint8_t buttonState;
uint8_t cont = 0;

void setup() {
  Serial.begin(115200);
  delay(100); 

  //Configure and initializes the LCD.
  eHealthDisplay.init(); 
  delay(100); 
}


void loop() {

  //Screen number one with the numerical sensor values and body position 
  //=========================================================================

  eHealthDisplay.initValuesScreen();
  delay(100);

  //Attach the inttruptions for using the pulsioximeter     

  PCintPort::attachInterrupt(6, readPulsioximeter, RISING);

  while(screenNumber == 1) {
    //It prints data sensor measures in the LCD.
    buttonState = digitalRead(pushButton);
    delay(10);

    if(buttonState == 1) {
      screenNumber++;
    }

    eHealthDisplay.printValuesScreen();
  }

  PCintPort::detachInterrupt(6);


  //Screen number two wich represent the air flow wave 
  //=========================================================================

  eHealthDisplay.initAirFlowScreen();	

  while( screenNumber == 2) {
    buttonState = digitalRead(pushButton);    
    delay(10);

    if(buttonState==1){
      screenNumber++;
    }

    eHealthDisplay.printAirFlowScreen();      
  }

  //Screen number three wich represent the ECG wave 
  //=========================================================================

  eHealthDisplay.initECGScreen();	

  while( screenNumber == 3) {
    buttonState = digitalRead(pushButton);    
    delay(10);

    if(buttonState==1){
      screenNumber++;
    }

    eHealthDisplay.printECGScreen();
  }

  screenNumber = 1;
  delay(10);  
}

//Include always this function for using the pulsioximeter sensor
//=========================================================================
void readPulsioximeter() {  

  cont ++;

  if (cont == 50) {
    //Get only one of 50 measures to reduce the latency
    eHealth.readPulsioximeter();  
    cont = 0;
  }
}


