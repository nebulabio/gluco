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
 *  Explanation: This example how to send data using 3G shield and 
 *  making a connection to a server. 
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

// For more information about the 3G shield please see our tutorial
// in cooking-hacks web site. http://www.cooking-hacks.com

#include <eHealth.h>


char data[512];
char tosend[128];
int led = 13;
int onModulePin = 2;        // the pin to switch on the module (without press on button) 

int x = 0;

char name[20];

char server[ ]="192.198.1.1"; //Your server IP address
char port[ ]="5555"; // Your port. 

void switchModule(){
  digitalWrite(onModulePin,HIGH);
  delay(2000);
  digitalWrite(onModulePin,LOW);
}

void setup(){

  Serial.begin(115200);         // UART baud rate
  delay(2000);
  pinMode(led, OUTPUT);
  pinMode(onModulePin, OUTPUT);
  switchModule();              // switches the module ON

  for (int i=0;i< 5;i++){
    delay(5000);
  } 

  Serial.println("AT+CGSOCKCONT=1,\"IP\",\"internetmas\"");
  Serial.flush();
  x=0;
  do
  {
    while(Serial.available()==0);
    data[x]=Serial.read();  
    x++;  
  } while(!(data[x-1]=='K'&&data[x-2]=='O')); //waits for response "Network opened"


}

void loop()
{
  float temperature = eHealth.getTemperature();    
  int longitud = sprintf(tosend,"%d", temperature); 

  Serial.print("AT+NETOPEN=\"TCP\",");
  //Opens the socket with the type of protocol and the port 
  Serial.println(port);        
  Serial.flush();
  x=0;
  
  do
  {
    while(Serial.available()==0);
    data[x]=Serial.read();  
    x++;  
  }
  while(!(data[x-1]=='K'&&data[x-2]=='O'));        //waits for response "Network opened"

  Serial.print("AT+TCPCONNECT=\""); //Connects with the server
  Serial.print(server);
  Serial.print("\",");    
  Serial.println(port);

  Serial.flush();
  while(Serial.read()!='K');  

  Serial.print("AT+TCPWRITE=");
  Serial.println(longitud, DEC);    //Sends TCP data
  Serial.flush();



  do 
  { 
    while (Serial.available() == 0) {};      
    //Serial.println(char(Serial.read()));      
  } while(Serial.read()!='>');  

  Serial.println(tosend); 

  x=0;

  do
  {
    while(Serial.available()==0);
    data[x]=Serial.read();  
    x++;                        
  }
  while(!(data[x-1]=='K'&&data[x-2]=='O'));

  Serial.println("AT+NETCLOSE"); //Opens the socket with the type of protocol and the port 
  Serial.flush();
  while(Serial.read()!='K'); 

  while(1);

}

