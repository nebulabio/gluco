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
 *  In this example we have used the serial monitor like a programed
 *  console where we can choose the sensor we want to view.  *   
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
 *  Author: Ahmad Saad & Luis Martín 
 */


#include <eHealth.h>
#include <PinChangeInt.h>


char serialByte;
uint8_t state = 0;
uint8_t cont = 0;

float parameter = 0.0;

    void setup() 
    {
      Serial.begin(115200);
      initScreen();
      delay(100);
    }
    
    void loop() 
    {      
      state = 0;
   
       while (Serial.available()>0){  
        serialByte=Serial.read();
        
          if ((serialByte=='A') || (serialByte == 'a')){ //Airflow sensor  
            airFlowMenu();
            airFlowBucle();

          } else if ((serialByte=='S') || (serialByte == 's')){
            skinSensorMenu();
            skinSensorBucle();
            
          } else if ((serialByte=='E') || (serialByte == 'e')){
            ECGMenu();
            ECGBucle();
            
          } else if ((serialByte=='P') || (serialByte == 'p')){
            pulsioximeterMenu();
            pulsioximeterBucle();
            
          } else if ((serialByte=='B') || (serialByte == 'b')){
            bodyPositionMenu();
            bodyPositionBucle();
            
          } else if ((serialByte=='T') || (serialByte == 't')){
            temperatureMenu();
            temperatureBucle();
            
          } else if ((serialByte=='N') || (serialByte == 'n')){
            BloodPressureMenu();
            BloodPressureBucle();             
          }  
       }        
    }
    
//=================================================================================================================
//=================================================================================================================
    void initScreen(void) 
    {  
      printLogoEhealth(); 
      delay(100); 
      mainMenu();
    }
//=================================================================================================================
//=================================================================================================================    
    void mainMenu(void) 
    {
       lineFeed();  Serial.print(F("  --------> EHEALTH MENU OPTIONS <--------"));  lineFeed();  lineFeed();
           
       initialSpace();   Serial.print(F("A : AirFlow sensor options"));       lineFeed(); 
       initialSpace();   Serial.print(F("S : Skin sensor options"));          lineFeed(); 
       initialSpace();   Serial.print(F("E : ECG sensor options"));           lineFeed(); 
       initialSpace();   Serial.print(F("P : Pulsioximeter sensor options")); lineFeed(); 
       initialSpace();   Serial.print(F("B : Body position sensor options")); lineFeed(); 
       initialSpace();   Serial.print(F("T : Temperature sensor options"));   lineFeed();     
       initialSpace();   Serial.print(F("N : Blood Pressure sensor options"));lineFeed(); 
    } 
//=================================================================================================================
//=================================================================================================================
    void airFlowMenu(void)  
    {  
      lineFeed();      Serial.print(F("--------> AIRFLOW MENU OPTIONS <--------"));  lineFeed();  lineFeed(); 
      initialSpace();  Serial.print(F("W : Wave form"));  lineFeed();
      initialSpace();  Serial.print(F("V : Analogic value"));  lineFeed();  
      initialSpace();  Serial.print(F("B : BACK MAIN MENU"));  lineFeed();  
    }  
//=================================================================================================================
//=================================================================================================================   
    void airFlowBucle(void) {      
      while (state == 0) {
        while (Serial.available()>0){  
          serialByte=Serial.read();        
             if ((serialByte=='W') || (serialByte == 'w')){ //Airflow sensor 
                while(1){                
                  serialByte=Serial.read();                     
                  eHealth.airFlowWave(eHealth.getAirFlow());
                     
                  if ((serialByte== 'B') || (serialByte == 'b')) {
                    airFlowMenu();                         
                    break;   
                  }               
                }            
              } else if ((serialByte=='V') || (serialByte == 'v')){
                  while(1){                
                    serialByte=Serial.read();                     
                    Serial.print(F(" Airflow analogic value : "));
                    Serial.println(eHealth.getAirFlow());
                    delay(20);                     
                       if ((serialByte== 'B') || (serialByte == 'b')) {
                         airFlowMenu();                         
                         break;   
                       }
                  }                 
              } else if ((serialByte=='B') || (serialByte == 'b')){              
                state = 1; 
              } 
           }          
        }       
      mainMenu();  
    }
//=================================================================================================================
//=================================================================================================================    
    void skinSensorMenu(void)  
    {
      lineFeed();      Serial.print(F("--------> SKIN SENSOR MENU OPTIONS <--------"));  lineFeed();  lineFeed(); 
      outputMenu();      
    } 
//=================================================================================================================
//=================================================================================================================    
    void skinSensorBucle(void) {
      while (state == 0) {
          while (Serial.available()>0){  
            serialByte=Serial.read();        
               if ((serialByte=='S') || (serialByte == 's')){  
                  while(1){ 
                    
                    lineFeed();
                    
                    serialByte=Serial.read();  
                    float conductance = eHealth.getSkinConductance();  
                    long resistance = eHealth.getSkinResistance();   
                    float conductanceVoltage = eHealth.getSkinConductanceVoltage();   

                     if (conductance == -1) {
                       Serial.println(" No patient connection");
                     } else {
                       
                       Serial.print(F(" Skin conductance value : "));
                       Serial.println(conductance);
                    
                       Serial.print(F(" Skin resistance value : "));
                       Serial.println(resistance);
                 
                       Serial.print(F(" Skin conductance value in volts: "));
                       Serial.println(conductanceVoltage);
                    
                       delay(500); 
                     }
                       
                    if ((serialByte== 'B') || (serialByte == 'b')) {
                      skinSensorMenu();                         
                      break;   
                    }               
                  } 

                    
                } else if ((serialByte=='B') || (serialByte == 'b')){              
                  state = 1; 
                } 
             }          
          }          
        mainMenu();   
    }  
//=================================================================================================================
//=================================================================================================================     
    void ECGMenu(void) {
      lineFeed();  Serial.print(F("--------> ECG MENU OPTIONS <--------"));  lineFeed();  lineFeed(); 
      outputMenu();     
    }
//=================================================================================================================
//=================================================================================================================    
    void ECGBucle(void) {
        while (state == 0) {
          while (Serial.available()>0){  
            serialByte=Serial.read();        
               if ((serialByte=='S') || (serialByte == 's')){  
                  while(1){ 
                    
                    lineFeed();     
                    serialByte=Serial.read();  

                    delay(500); 
                       
                    if ((serialByte== 'B') || (serialByte == 'b')) {
                      ECGMenu();                         
                      break;   
                    }               
                  }                    
                } else if ((serialByte=='B') || (serialByte == 'b')){              
                  state = 1; 
                } 
             }          
          }          
        mainMenu(); 
    }  
//=================================================================================================================
//=================================================================================================================    
    void pulsioximeterMenu(void)  
    {
      lineFeed();      Serial.print(F("--------> PULSIOXIMETER MENU OPTIONS <--------"));  lineFeed();  lineFeed(); 
      outputMenu();     
    }   
//=================================================================================================================
//=================================================================================================================      
    void pulsioximeterBucle(void)
    {
        //Attach the interruptions for using the pulsioximeter.
        PCintPort::attachInterrupt(6, readPulsioximeter, RISING);
        eHealth.initPulsioximeter();

        while (state == 0) {
          while (Serial.available()>0){  
            serialByte=Serial.read();        
               if ((serialByte=='S') || (serialByte == 's')){  
                  while(1){ 
                    
                    //eHealth.readPulsioximeter();                   

                    lineFeed();                    
                    serialByte=Serial.read();  
                    Serial.print("PRbpm : "); 
                    Serial.print(eHealth.getOxygenSaturation());

                    Serial.print(" % SPo2 : ");
                    Serial.print(eHealth.getBPM());
  
                    Serial.print("\n");                    
                    delay(500); 
                       
                    if ((serialByte== 'B') || (serialByte == 'b')) {
                      pulsioximeterMenu();                         
                      break;   
                    }               
                  }                     
                } else if ((serialByte=='B') || (serialByte == 'b')){              
                  state = 1; 
                } 
             }          
          }          
        mainMenu();
        
        PCintPort::detachInterrupt(6);
    }  
//=================================================================================================================
//=================================================================================================================    
    void bodyPositionMenu(void)  
    {
      lineFeed();  Serial.print(F("--------> BODY POSTITIONMENU OPTIONS <--------"));  lineFeed();  lineFeed(); 
      outputMenu(); 
    } 
//=================================================================================================================
//=================================================================================================================     
    void bodyPositionBucle(void)
    {
       while (state == 0) {
          while (Serial.available()>0){

            eHealth.initPositionSensor();
            serialByte=Serial.read();        
               if ((serialByte=='S') || (serialByte == 's')){  
                  while(1){ 
                    
                    lineFeed();     
                    serialByte=Serial.read();  
                    
                    Serial.print("Current position : ");
                    uint8_t position = eHealth.getBodyPosition(); 
                    eHealth.printPosition(position);
                    delay(100);                    
                       
                    if ((serialByte== 'B') || (serialByte == 'b')) {
                      bodyPositionMenu();                         
                      break;   
                    }               
                  }                    
                } else if ((serialByte=='B') || (serialByte == 'b')){              
                  state = 1; 
                } 
             }          
          }          
        mainMenu();
    }  
//=================================================================================================================
//=================================================================================================================    
    void temperatureMenu(void)  
    {
      lineFeed();  Serial.print(F("--------> TEMPERATURE MENU OPTIONS <--------"));  lineFeed();  lineFeed(); 
      outputMenu();
    }    
//=================================================================================================================
//=================================================================================================================    
    void temperatureBucle(void)
    {
          while (state == 0) {
          while (Serial.available()>0){  
            serialByte=Serial.read();        
               if ((serialByte=='S') || (serialByte == 's')){  
                  while(1){ 
                    
                    lineFeed();     
                    serialByte=Serial.read();
                    
                    float temperature = eHealth.getTemperature();   
                    Serial.print("Temperature (ºC): ");       
                    Serial.println(temperature, 2);                    
                    
                    delay(1000); 
                       
                    if ((serialByte== 'B') || (serialByte == 'b')) {
                      temperatureMenu();                         
                      break;   
                    }               
                  }                    
                } else if ((serialByte=='B') || (serialByte == 'b')){              
                  state = 1; 
                } 
             }          
          }          
        mainMenu(); 
    }  
//=================================================================================================================
//=================================================================================================================    
    void BloodPressureMenu(void)  
    {
      lineFeed();  Serial.print(F("--------> BLOOD PRESSURE MENU OPTIONS <--------"));  lineFeed();  lineFeed(); 
      outputMenu();    
    }
//=================================================================================================================
//=================================================================================================================     

    void BloodPressureBucle()
    {
          while (state == 0) {
          while (Serial.available()>0){  
            serialByte=Serial.read();        
               if ((serialByte=='S') || (serialByte == 's')){  
                  while(1){ 
                    
                    lineFeed();     
                    serialByte=Serial.read();  
                    Serial.println("Press On/Off button please...");

                    eHealth.initBloodPressureSensor(parameter); 
                    
                    Serial.println("****************************");
                    Serial.print("Systolic blood pressure value : ");
                    Serial.println(eHealth.getSystolicPressure());
                  
                    Serial.println("****************************");
                    Serial.print("Diastolic blood pressure value : ");
                    Serial.println(eHealth.getDiastolicPressure());
                    delay(3000); 
                       
                    if ((serialByte== 'B') || (serialByte == 'b')) {
                      BloodPressureMenu();                         
                      break;   
                    }               
                  }                    
                } else if ((serialByte=='B') || (serialByte == 'b')){              
                  state = 1; 
                } 
             }          
          }          
        mainMenu();
    }
    


//=================================================================================================================
//=================================================================================================================   
    void outputMenu(void) {
        initialSpace();  Serial.print(F("S : Serial output value"));  lineFeed();  
        initialSpace();  Serial.print(F("B : BACK MAIN MENU"));  lineFeed();   
    }

//=================================================================================================================
//=================================================================================================================
    void printLogoEhealth(void)  
    {  
      starLine();      
      starPrint();  blank();        HLeterOne();    blank();       blank();         blank();         tLeterOne();   hLeterOne();  starPrint();   lineFeed();    
      starPrint();  eLeterTwo();    HLeterTwo();    eLeterTwo();   aLeterTwo();     lLeterTwo();     tLeterTwo();   hLeterTwo();  starPrint();   lineFeed();
      starPrint();  eLeterThree();  HLeterThree();  eLeterThree(); aLeterThree();   lLeterThree();   tLeterThree(); hLeterThree();  starPrint();   lineFeed();
      starPrint();  eLeterFour();   HLeterFour();   eLeterFour();  aLeterFour();    lLeterFour();    tLeterFour();  hLeterFour();  starPrint();   lineFeed();
      starPrint();  eLeterFive();   HLeterFive();   eLeterFive();  aLeterFive();    lLeterFive();    tLeterFive();  hLeterFive();  starPrint();   lineFeed();
      starPrint();  eLeterSix();    HLeterSix();    eLeterSix();   aLeterSix();     lLeterSix();     tLeterSix();   hLeterSix();  starPrint();   lineFeed();
      starLine();
    }
//=================================================================================================================
//=================================================================================================================
    void eLeterTwo(void)   { Serial.print(F(" _____  "));  }
    void eLeterThree(void) { Serial.print(F("|  __ | "));  }  
    void eLeterFour(void)  { Serial.print(F("|  ___| "));  }  
    void eLeterFive(void)  { Serial.print(F("| |___  "));  }
    void eLeterSix(void)   { Serial.print(F("|_____| "));  }
    
    void HLeterOne(void)   { Serial.print(F(" _    _  ")); }
    void HLeterTwo(void)   { Serial.print(F("| |  | | ")); }
    void HLeterThree(void) { Serial.print(F("| |__| | ")); }  
    void HLeterFour(void)  { Serial.print(F("|  __  | ")); }  
    void HLeterFive(void)  { Serial.print(F("| |  | | ")); }
    void HLeterSix(void)   { Serial.print(F("|_|  |_| ")); }   

    
    void aLeterTwo(void)   { Serial.print(F(" ______  ")); }
    void aLeterThree(void) { Serial.print(F("|      | ")); }  
    void aLeterFour(void)  { Serial.print(F("|  {}  | ")); }  
    void aLeterFive(void)  { Serial.print(F("|  __  | ")); }
    void aLeterSix(void)   { Serial.print(F("|_|  |_| ")); }
    
    void lLeterTwo(void)   { Serial.print(F(" _      "));  }
    void lLeterThree(void) { Serial.print(F("| |     "));  }  
    void lLeterFour(void)  { Serial.print(F("| |     "));  }  
    void lLeterFive(void)  { Serial.print(F("| |____ "));  }
    void lLeterSix(void)   { Serial.print(F("|______|"));  }
    
    void tLeterOne(void)   { Serial.print(F("    _     "));}
    void tLeterTwo(void)   { Serial.print(F(" _| |__  ")); }
    void tLeterThree(void) { Serial.print(F("(_   __) ")); }  
    void tLeterFour(void)  { Serial.print(F("  | |    ")); }  
    void tLeterFive(void)  { Serial.print(F("  | |__  ")); }
    void tLeterSix(void)   { Serial.print(F("  |____) ")); }
    
    void hLeterOne(void)   { Serial.print(F(" _       "));}
    void hLeterTwo(void)   { Serial.print(F("| |      ")); }
    void hLeterThree(void) { Serial.print(F("| |___   ")); }  
    void hLeterFour(void)  { Serial.print(F("|  _  |  ")); }  
    void hLeterFive(void)  { Serial.print(F("| | | |  ")); }
    void hLeterSix(void)   { Serial.print(F("|_| |_|  ")); }

//=================================================================================================================
//=================================================================================================================  
    
    void blank(void)   { Serial.print(F("        "));}

//=================================================================================================================
//=================================================================================================================    
    void initialSpace(void)  
    {      
        Serial.print(F("        "));  
    }   
//=================================================================================================================
//=================================================================================================================    
    void starPrint(void)  
    {
      Serial.print(F("* ")); 
    }
//=================================================================================================================
//=================================================================================================================    
    void lineFeed(void)  
    {
      Serial.print(F("\n")); 
    }
//=================================================================================================================
//=================================================================================================================    
    void starLine(void)  
    {
      for (int i = 0; i< 63; i++) 
      {
        Serial.print(F("*"));
      }      
      lineFeed();
    }        
//=================================================================================================================
//=================================================================================================================

int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

//=================================================================================================================
//=================================================================================================================

//Include always this code for using the pulsioximeter sensor
//=========================================================================
void readPulsioximeter(){  
  
  cont ++;
 
  if (cont == 30) { //Get only one of 25 measures to reduce the latency
    eHealth.readPulsioximeter();  
    cont = 0;
  }
}
