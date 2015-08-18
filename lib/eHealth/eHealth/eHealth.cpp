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
 *  Author: Ahmad Saad & Luis Martín
 */


// include this library's description file
#include "eHealth.h"


//***************************************************************
// Accelerometer Variables and definitions						*
//***************************************************************

	//! not the wire library, can't use pull-ups
	#include "utils/i2c.h"
	
	//! Breakout board defaults to 1, set to 0 if SA0 jumper is set
	#define SA0 1  
		#if SA0
			#define MMA8452_ADDRESS 0x1D  //! SA0 is high, 0x1C if low
		#else
			#define MMA8452_ADDRESS 0x1C  
		#endif

	#define	int1Pin 2
	#define int2Pin 3

	//! Set the scale below either 2, 4 or 8.
	const byte scale = 2;

	//! Set the output data rate below. Value should be between 0 and 7.
	//! 0=800Hz, 1=400, 2=200, 3=100, 4=50, 5=12.5, 6=6.25, 7=1.56
	const byte dataRate = 0;

//***************************************************************
// BloodPressureSensor tables											*
//***************************************************************

	const int pressures[] = {85, 87, 88, 90, 91, 92, 94, 96, 98, 100, 101, 102, 103, 105, 106, 107, 108, 110, 112, 114, 122, 123, 124, 131, 137, 147, 153, 156 };                   
	//!                 	 |   |   |	 |   |   |   |   |   |    |    |    |    |    |     |    |    |    |    |    |   |    |    |    |    |    |    |    |                                    
	//!                   	 0   1   2   3   4   5   6   7   8    9   10   11   12   13    14   15    16   17   18  19  20   21   22   23   24   25   26   27 


	const double voltages[] = {1.120, 1.16, 1.1685, 1.180, 1.1860 , 1.21, 1.2227, 1.24, 1.254, 1.265, 1.27, 1.275, 1.2825, 1.290, 1.296, 1.316, 1.33 , 1.34 ,1.37, 1.39, 1.4535, 1.46, 1.465, 1.51, 1.557, 1.64, 1.68, 1.71}; 
	//!							|      |      |      |       |      |      |       |      |     |      |       |     |      |     |      |      |     |      |       |     |     |      |     |       |     |     |     |            
	//!							0      1      2      3       4      5      6       7      8     9     10      11     12     13    14     15     16    17    18      19      20    21    22     23    24     25    26    27


//***************************************************************
// Constructor of the class										*
//***************************************************************

	//! Function that handles the creation and setup of instances
	eHealthClass::eHealthClass(void) { /*void constructor*/ }
	

//***************************************************************
// Public Methods												*
//***************************************************************


	//!******************************************************************************
	//!	Name:	initPositionSensor()												*
	//!	Description: Initializes the position sensor and configure some values.		*
	//!	Param : void																*
	//!	Returns: void																*
	//!	Example: eHealth.initPositionSensor();										*
	//!******************************************************************************

	void eHealthClass::initPositionSensor(void)
	{
		byte c;

		/* Set up the interrupt pins, they're set as active high, push-pull */
		pinMode(int1Pin, INPUT);
		digitalWrite(int1Pin, LOW);
		pinMode(int2Pin, INPUT);
		digitalWrite(int2Pin, LOW);
  
		/* Read the WHO_AM_I register, this is a good test of communication */
		c = readRegister(0x0D);  // Read WHO_AM_I register
		if (c == 0x2A)	{ // WHO_AM_I should always be 0x2A
			initMMA8452(scale, dataRate);  // init the accelerometer if communication is good
			Serial.println("MMA8452Q is online...");
		} else {
			Serial.print("Could not connect to MMA8452Q: 0x");
			Serial.println(c, HEX);
			//while (1);  // Loop forever if communication doesn't happen
		}
	}


	//!******************************************************************************
	//!	Name:	initBloodPressureSensor()											*
	//!	Description: Initializes the BloodPressureSensor sensor.					*
	//!	Param : floar parameter with correction value 								*
	//!	Returns: void																*
	//!	Example: eHealth.initBloodPressureSensor();									*
	//!******************************************************************************

	void eHealthClass::initBloodPressureSensor(float parameter)
	{	
		float pressureMAPactual = 0;
		float pressureMAP = 0;
		float pressure = 0; //Pressure sensor lecture
		float pulse = 0; //Voltage pulses
		float voltage = 0;
		unsigned char state = 0;
		unsigned int  initial = 0;
		float pressuremmHG = 10;
		unsigned int i = 0; 
		boolean rank = 0; 

		double pastVoltages[30];
		double furtherVoltages[30];
		double aux[50];
		
		initial = digitalRead(5);      
		 
		/*while (initial == 1) {
			initial = digitalRead(5); //Waiting for pulse.
		}*/
	
		if (initial == 0) {
			char cont = 0;
			Serial.print("Measurement started ...............");
			Serial.println("Wait please................");    
			state = 1;    
			delay(3000);  // Wait for 5 seconds to eliminate noise measurement 
			pressureMAPactual=0;
			pressure = 0;
    
			while (initial == 0) {
				pressure = analogRead(4);// ----> Reading pressure sensor
				pulse = analogRead(5); // ----> Reading voltage pulses       
				initial = digitalRead(5); // ----> Start measure
			
				if ( cont == 30 ) {
					cont = 0;
				}
			
				aux[cont] = ( pressure * 5 ) / 1023; 
				cont ++;
				
				if (pressureMAPactual < pulse) { // Save max value
					pressureMAPactual = pulse;
					pressureMAP = pressure;

					for (int i = 0; i<30; i++) { 
						pressure = analogRead(4);// Reading pressure sensor
						furtherVoltages[i] = (pressure * 5) / 1023;
						pastVoltages[i] = aux[i];
					}
				}
			}
		

			voltage = 0; // Clear variable
	  
			for (int i = 0; i <30; i++) {
				voltage = voltage + furtherVoltages[i] + pastVoltages[i]; 
			} // Average value
	  
			voltage = (voltage / 60) + parameter ; // Correction value

			Serial.print("Average voltage : ");
			Serial.println(voltage);
			
			if (state == 1) { 
				i = 0; 
				rank = 0;

				while ((rank == 0) & (i <27)) { // Searching for range int table of voltages
					if (voltages[i] >= voltage ) { 
						rank = 1;
					}
					i++; 
				}
				i--;
				pressuremmHG = pressures[i-1] + (voltage - voltages[i-1]) * 100; // Asign pressure LUP.

				systolic = (int(pressuremmHG));
				diastolic = (int((pressuremmHG * 0.54) / 0.85 ) );
				state = 0; 
				pressureMAP = 0;
				pressureMAPactual=0;
				pressuremmHG = 0;
				voltage = 0;
				initial = 1;
			}
		}
	}


	
	//!******************************************************************************
	//!		Name:	initPulsioximeter()												*
	//!		Description: Initializes the pulsioximeter sensor.						*
	//!		Param : void															*
	//!		Returns: void															*
	//!		Example: eHealth.initPulsioximeter();									*
	//!******************************************************************************

	void eHealthClass::initPulsioximeter(void) 
	{
		// Configuring digital pins like INPUTS
		pinMode(13, INPUT);		pinMode(12, INPUT);
		pinMode(11, INPUT);		pinMode(10, INPUT);
		pinMode( 9, INPUT);		pinMode( 8, INPUT);
		pinMode( 7, INPUT);		pinMode( 6, INPUT);
		// attach a PinChange Interrupt to our pin on the rising edge
	}

	
	//!******************************************************************************
	//!		Name:	getTemperature()												*
	//!		Description: Returns the corporal temperature.							*
	//!		Param : void															*
	//!		Returns: float with the corporal temperature value.						*
	//!		Example: float temperature = eHealth.getTemperature();					*
	//!******************************************************************************

	float eHealthClass::getTemperature(void)
	{	
		//Local variables
		float Temperature; //Corporal Temperature 
		float Resistance;  //Resistance of sensor.
		float ganancia=5.0;
		float Vcc=3.3;
		float RefTension=3.0; // Voltage Reference of Wheatstone bridge.
		float Ra=4700.0; //Wheatstone bridge resistance.
		float Rc=4700.0; //Wheatstone bridge resistance.
		float Rb=821.0; //Wheatstone bridge resistance.
		int sensorValue = analogRead(A3);
		
		float voltage2=((float)sensorValue*Vcc)/1023; // binary to voltage conversion  

		// Wheatstone bridge output voltage.
		voltage2=voltage2/ganancia;
		// Resistance sensor calculate  
		float aux=(voltage2/RefTension)+Rb/(Rb+Ra);
		Resistance=Rc*aux/(1-aux);    
		if (Resistance >=1822.8) {
			// if temperature between 25ºC and 29.9ºC. R(tª)=6638.20457*(0.95768)^t
			Temperature=log(Resistance/6638.20457)/log(0.95768);  
		} else {
			if (Resistance >=1477.1){
					// if temperature between 30ºC and 34.9ºC. R(tª)=6403.49306*(0.95883)^t
					Temperature=log(Resistance/6403.49306)/log(0.95883);  
			} else {
				if (Resistance >=1204.8){
					// if temperature between 35ºC and 39.9ºC. R(tª)=6118.01620*(0.96008)^t
					Temperature=log(Resistance/6118.01620)/log(0.96008); 
				}
				else{
					if (Resistance >=988.1){
						// if temperature between 40ºC and 44.9ºC. R(tª)=5859.06368*(0.96112)^t
						Temperature=log(Resistance/5859.06368)/log(0.96112); 
					}
					else {
						if (Resistance >=811.7){
							// if temperature between 45ºC and 50ºC. R(tª)=5575.94572*(0.96218)^t
							Temperature=log(Resistance/5575.94572)/log(0.96218); 
						}
					}
				}
			}  
		}
		
		return Temperature;
	}

	//!******************************************************************************
	//!		Name:	getOxygenSaturation()											*
	//!		Description: Returns the oxygen saturation in blood in percent.			*
	//!		Param : void															*
	//!		Returns: int with the oxygen saturation value							*
	//!		Example: int SPO2 = eHealth.getOxygenSaturation();						*
	//!******************************************************************************

	int eHealthClass::getOxygenSaturation(void)
	{
		return SPO2;
	}

	//!******************************************************************************
	//!		Name:	getBPM()														*
	//!		Description: Returns the heart beats per minute.						*
	//!		Param : void															*
	//!		Returns: int with the beats per minute									*
	//!		Example: int BPM = eHealth.getBPM();									*
	//!******************************************************************************

	int eHealthClass::getBPM(void)
	{
		return BPM;
	}


	//!******************************************************************************
	//!		Name:	getSkinConductance()											*
	//!		Description: Returns the value of skin conductance.						*
	//!		Param : void															*
	//!		Returns: float with the value of skin conductance						*
	//!		Example: float conductance = eHealth.getSkinConductance();				*
	//!******************************************************************************

	float eHealthClass::getSkinConductance(void)
	{
		// Local variable declaration.   
		float resistance;
		float conductance;
		delay(1);
		
		// Read an analogic value from analogic2 pin.
		float sensorValue = analogRead(A2);
		float voltage = sensorValue*5.0/1023;

		conductance = 2*((voltage - 0.5) / 100000);

		// Conductance calculation
		resistance = 1 / conductance; 
		conductance = conductance * 1000000;
		delay(1);
		
		if (conductance > 1.0) 	return conductance;
		else return -1.0;
	}


	//!******************************************************************************
	//!		Name:	getSkinResistance()												*
	//!		Description: Returns the value of skin resistance.						*
	//!		Param : void															*
	//!		Returns: float with the value of skin resistance						*
	//!		Example: float resistance = eHealth.getSkinResistance();				*
	//!******************************************************************************

	float eHealthClass::getSkinResistance(void)
	{	
		// Local variable declaration.   
		float resistance;
		float conductance;
	
		// Read an analogic value from analogic2 pin.
		float sensorValue = analogRead(A2);
		float voltage = (sensorValue * 5.0) / 1023; 
	
		delay(2);
		conductance = 2*((voltage - 0.5) / 100000);
   
		//Conductance calcultacion
		resistance = 1 / conductance;
		delay(2);
	
		if (resistance > 1.0 ) return resistance;
		else return -1.0;
	}

	
	//!******************************************************************************
	//!		Name:	getSkinConductanceVoltage()										*
	//!		Description: Returns the skin conductance value in voltage .			*
	//!		Param : void															*
	//!		Returns: float with the skin conductance value in voltage 				*
	//!		Example: float volt = eHealth.getSkinConductanceVoltage();				*
	//!******************************************************************************

	float eHealthClass::getSkinConductanceVoltage(void)
	{
		delay(2);
	
		//Read analogic value from analogic2 pin.
		int sensorValue = analogRead(A2);
	
		//Convert the readed value to voltage.
		float voltage = ( sensorValue * 5.0 ) / 1023;

		delay(2);
		return voltage;
	}


	//!******************************************************************************
	//!		Name:	getECG()														*
	//!		Description: Returns an analogic value to represent the ECG.			*
	//!		Param : void															*
	//!		Returns: float with the ECG value in voltage			 				*
	//!		Example: float volt = eHealth.getECG();									*
	//!******************************************************************************

	float eHealthClass::getECG(void)
	{
		float analog0;
		// Read from analogic in. 
		analog0=analogRead(0);
		// binary to voltage conversion
		return analog0 = (float)analog0 * 5 / 1023.0;   
	}
	

	//!******************************************************************************
	//!		Name:	getBodyPosition()												*
	//!		Description: Returns the current body position.							*
	//!		Param : void															*
	//!		Returns: uint8_t with the the position of the pacient.	 				*
	//!		Example: uint8_t position = eHealth.getBodyPosition();					*
	//!******************************************************************************

	uint8_t eHealthClass::getBodyPosition(void)
	{
		static byte source;

		/* If int1 goes high, all data registers have new data */
		if (digitalRead(int1Pin)) {// Interrupt pin, should probably attach to interrupt function	
			readRegisters(0x01, 6, &data[0]);  // Read the six data registers into data array.
			
			/* For loop to calculate 12-bit ADC and g value for each axis */
			for (int i=0; i<6; i+=2) {
				accelCount[i/2] = ((data[i] << 8) | data[i+1]) >> 4;  // Turn the MSB and LSB into a 12-bit value
				
					if (data[i] > 0x7F) {
						accelCount[i/2] = ~accelCount[i/2] + 1;
						accelCount[i/2] *= -1;  // Transform into negative 2's complement #
					}
					
				accel[i/2] = (float) accelCount[i/2]/((1<<12)/(2*scale));  // get actual g value, this depends on scale being set
			}		
		}
		
		/* If int2 goes high, either p/l has changed or there's been a single/double tap */
		if (digitalRead(int2Pin)) {
			source = readRegister(0x0C);  // Read the interrupt source reg.
			
			if ((source & 0x10)==0x10)  // If the p/l bit is set, go check those registers
				portraitLandscapeHandler();      

			delay(50); // Delay here for a little printing visibility, make it longer, or delete it
		}

		delay(100);
  
		return bodyPos; 
	}


	//!******************************************************************************
	//!		Name:	getSystolicPressure()											*
	//!		Description: Returns the  value of the systolic pressure.				*
	//!		Param : void															*
	//!		Returns: int with the systolic pressure.								*
	//!		Example: int systolic = eHealth.getSystolicPressure();					*
	//!******************************************************************************

	int eHealthClass::getSystolicPressure(void)
	{
		return systolic;
	}


	//!******************************************************************************
	//!		Name:	getDiastolicPressure()											*
	//!		Description: Returns the  value of the diastolic pressure.				*
	//!		Param : void															*
	//!		Returns: int with the diastolic pressure.								*
	//!		Example: int diastolic = eHealth.getDiastolicPressure();				*
	//!******************************************************************************

	int eHealthClass::getDiastolicPressure(void)
	{
		return diastolic;
	}


	//!******************************************************************************
	//!		Name:	getAirFlow()													*
	//!		Description: Returns an analogic value to represent the air flow.		*
	//!		Param : void															*
	//!		Returns: int with the airFlow value (0-1023).							*
	//!		Example: int airFlow = eHealth.getAirFlow();							*
	//!******************************************************************************

	int eHealthClass::getAirFlow(void)
	{
		int airFlow = analogRead(A1);

		
		return airFlow; 
	}


	//!******************************************************************************
	//!		Name:	printPosition()													*
	//!		Description: Returns an analogic value to represent the air flow.		*
	//!		Param : uint8_t position : the current body position. 					*
	//!		Returns: void															*
	//!		Example: eHealth.printPosition(position);								*
	//!******************************************************************************

	void eHealthClass::printPosition( uint8_t position )
	{
		if (position == 1) {
			Serial.println("Supine position");    
		} else if (position == 2) {
			Serial.println("Left lateral decubitus");
		} else if (position == 3) {
			Serial.println("Rigth lateral decubitus");
		} else if (position == 4) {
			Serial.println("Prone position");
		} else if (position == 5) {
			Serial.println("Stand or sit position");
		} else  {
			Serial.println("non-defined position");
		}
	}

	
	//!******************************************************************************
	//!		Name:	readPulsioximeter()												*
	//!		Description: It reads a value from pulsioximeter sensor.				*
	//!		Param : void										 					*
	//!		Returns: void															*
	//!		Example: readPulsioximeter();											*
	//!******************************************************************************
 
	void eHealthClass::readPulsioximeter(void)
	{
		uint8_t digito[] = {0,0,0,0,0,0};

		uint8_t A = 0;
		uint8_t B = 0;
		uint8_t C = 0;
		uint8_t D = 0;
		uint8_t E = 0;
		uint8_t F = 0;
		uint8_t G = 0;

		for (int i = 0; i<6 ; i++) { // read all the led's of the module
			A = !digitalRead(13);
			B = !digitalRead(12);
			C = !digitalRead(11);
			D = !digitalRead(10);
			E = !digitalRead(9);
			F = !digitalRead(8);
			G = !digitalRead(7);
			
			digito[i] = segToNumber(A, B, C ,D ,E, F,G);    
			delayMicroseconds(2800); //2800 microseconds
		}

			SPO2 = 10 * digito[5] + digito[4];
			BPM  = 100 * digito[2] + 10 * digito[1] + digito[0];
	}
	

	//!******************************************************************************
	//!		Name: airflowWave()														*
	//!		Description: It prints air flow wave form in the serial monitor			*
	//!		Param : int air with the analogic value									*
	//!		Returns: void															*
	//!		Example: eHealth.airflowWave();											*
	//!******************************************************************************

	void eHealthClass::airFlowWave(int air)
	{
		for (int i=0; i < (air / 5) ; i ++) {  
				Serial.print("..");  
		}

		Serial.print("..");
		Serial.print("\n");
		delay(25);
	}


	//!******************************************************************************
	//!		Name: readGlucometer()													*
	//!		Description: It reads the data stored in the glucometer					*
	//!		Param : void															*
	//!		Returns: void															*
	//!		Example: eHealth.readGlucometer();										*
	//!******************************************************************************
	
	void eHealthClass::readGlucometer(void)
	{
		delay(100);
		Serial.begin(1200);
		delay(100);

		Serial.print("U"); // Start communication command. 
		delay(1000); // Wait while receiving data.

		Serial.print("\n"); 
		if (Serial.available() > 0) {
			length = Serial.read();// The protocol sends the number of measures 
			Serial.read(); // Read one dummy data

			for (int i = 0; i<length; i++) { // The protocol sends data in this order 
				glucoseDataVector[i].year = Serial.read(); 
				glucoseDataVector[i].month = Serial.read();
				glucoseDataVector[i].day = Serial.read();
				glucoseDataVector[i].hour = Serial.read();
				glucoseDataVector[i].minutes = Serial.read();

				Serial.read(); // Byte of separation must be 0x00.

				glucoseDataVector[i].glucose = Serial.read();
				glucoseDataVector[i].meridian = Serial.read();

				Serial.read(); // CheckSum 1
				Serial.read(); // CheckSum 2			
			}
		}
	}

	//!******************************************************************************
	//!		Name: getGlucometerLength()												*
	//!		Description: it returns the number of data stored in the glucometer		*
	//!		Param : void															*
	//!		Returns: uint8_t with length											*
	//!		Example: int length = eHealth.getGlucometerLength();					*
	//!******************************************************************************

	uint8_t eHealthClass::getGlucometerLength(void)
	{
		return length;
	}

	//!******************************************************************************
	//!		Name: numberToMonth()													*
	//!		Description: Convert month variable from numeric to character.			*
	//!		Param : int month in numerical format									*
	//!		Returns: String with the month characters (January, February...).		*
	//!		Example: Serial.print(eHealth.numberToMonth(month));					*
	//!******************************************************************************
	
	String eHealthClass::numberToMonth(int month)
	{
		if (month == 1)  return "January"; 
		else if (month == 2)  return "February";
		else if (month == 3)  return "March";
		else if (month == 4)  return "April";
		else if (month == 5)  return "May";
		else if (month == 6)  return "June";
		else if (month == 7)  return "July";
		else if (month == 8)  return "August";
		else if (month == 9)  return "September";
		else if (month == 10) return "October";
		else if (month == 11) return "November";
		else return "December";
	}

	
	//!******************************************************************************
	//!		Name:	version()														*
	//!		Description: It check the version of the library						*
	//!		Param : void															*
	//!		Returns: void															*
	//!		Example: eHealth.version();												*
	//!******************************************************************************

	int eHealthClass::version(void)
	{
		return 1;
	}


//***************************************************************
// Private Methods												*
//***************************************************************

	//! This function will read the p/l source register and
	//!	print what direction the sensor is now facing */
	
	void eHealthClass::portraitLandscapeHandler()
	{
		byte pl = readRegister(0x10);  // Reads the PL_STATUS register
		
		switch((pl&0x06)>>1)  // Check on the LAPO[1:0] bits
		{
			case 0:
				position[0] = 0;
			break;
			
			case 1:
				position[0] = 1;
			break;
			
			case 2:
				position[0] = 2;
			break;
			
			case 3:
				position[0] = 3;
			break;
		}
		
		if (pl&0x01)  // Check the BAFRO bit
			position[1] = 0;
		else
			position[1] = 1;
		if (pl&0x40)  // Check the LO bit
			position[2] = 0;
		else 
			position[2] = 1;
 
		bodyPosition();  
	}

/*******************************************************************************************************/

	//! Initialize the MMA8452 registers.

	void eHealthClass::initMMA8452(byte fsr, byte dataRate)
	{
		MMA8452Standby();  // Must be in standby to change registers
  
		/* Set up the full scale range to 2, 4, or 8g. */
		if ((fsr==2)||(fsr==4)||(fsr==8))
			writeRegister(0x0E, fsr >> 2);  
		else
			writeRegister(0x0E, 0);
			
		/* Setup the 3 data rate bits, from 0 to 7 */
		writeRegister(0x2A, readRegister(0x2A) & ~(0x38));
		
		if (dataRate <= 7)
			writeRegister(0x2A, readRegister(0x2A) | (dataRate << 3));
			
		/* Set up portrait/landscap registers */
		writeRegister(0x11, 0x40);  // Enable P/L
		writeRegister(0x13, 0x14);  // 29deg z-lock, 
		writeRegister(0x14, 0x84);  // 45deg thresh, 14deg hyst
		writeRegister(0x12, 0x05);  // debounce counter at 100ms
		
		/* Set up single and double tap */
		writeRegister(0x21, 0x7F);  // enable single/double taps on all axes
		writeRegister(0x23, 0x20);  // x thresh at 2g
		writeRegister(0x24, 0x20);  // y thresh at 2g
		writeRegister(0x25, 0x8);  // z thresh at .5g
		writeRegister(0x26, 0x30);  // 60ms time limit, the min/max here is very dependent on output data rate
		writeRegister(0x27, 0x28);  // 200ms between taps min
		writeRegister(0x28, 0xFF);  // 1.275s (max value) between taps max
		
		/* Set up interrupt 1 and 2 */
		writeRegister(0x2C, 0x02);  // Active high, push-pull
		writeRegister(0x2D, 0x19);  // DRDY int enabled, P/L enabled
		writeRegister(0x2E, 0x01);  // DRDY on INT1, P/L on INT2
		
		MMA8452Active();  // Set to active to start reading
	}

/*******************************************************************************************************/

	//! Sets the MMA8452 to standby mode. It must be in standby to change most register settings.
	
	void eHealthClass::MMA8452Standby()
	{
		byte c = readRegister(0x2A);
		writeRegister(0x2A, c & ~(0x01));
	}

/*******************************************************************************************************/

	//! Sets the MMA8452 to active mode. Needs to be in this mode to output data
	
	void eHealthClass::MMA8452Active()
	{
		byte c = readRegister(0x2A);
		writeRegister(0x2A, c | 0x01);
	}

/*******************************************************************************************************/	

	//! Read i registers sequentially, starting at address into the dest byte array.
	void eHealthClass::readRegisters(byte address, int i, byte * dest)
	{
		i2cSendStart();
		i2cWaitForComplete();
		
		i2cSendByte((MMA8452_ADDRESS<<1));	// write 0xB4
		i2cWaitForComplete();
		
		i2cSendByte(address);	// write register address
		i2cWaitForComplete();
		
		i2cSendStart();
		i2cSendByte((MMA8452_ADDRESS<<1)|0x01);	// write 0xB5
		i2cWaitForComplete();
		
		for (int j=0; j<i; j++) {
			i2cReceiveByte(TRUE);
			i2cWaitForComplete();
			dest[j] = i2cGetReceivedByte();	// Get MSB result
		}
		
		i2cWaitForComplete();
		i2cSendStop();
		
		cbi(TWCR, TWEN);// Disable TWI
		sbi(TWCR, TWEN);// Enable TWI
	}

/*******************************************************************************************************/

	//! Read a single byte from address and return it as a byte.
	
	byte eHealthClass::readRegister(uint8_t address)
	{
		byte data;
		
		i2cSendStart();
		i2cWaitForComplete();
		
		i2cSendByte((MMA8452_ADDRESS<<1));	// write 0xB4
		i2cWaitForComplete();
		
		i2cSendByte(address);	// write register address
		i2cWaitForComplete();
		
		i2cSendStart();
		
		i2cSendByte((MMA8452_ADDRESS<<1)|0x01);	// write 0xB5
		i2cWaitForComplete();
		i2cReceiveByte(TRUE);
		i2cWaitForComplete();
		
		data = i2cGetReceivedByte();	// Get MSB result
		i2cWaitForComplete();
		i2cSendStop();
		
		cbi(TWCR, TWEN);	// Disable TWI
		sbi(TWCR, TWEN);	// Enable TWI
		
		return data;
	}

/*******************************************************************************************************/

	//! Writes a single byte (data) into address 
	void eHealthClass::writeRegister(unsigned char address, unsigned char data)
	{
		i2cSendStart();
		i2cWaitForComplete();  
		i2cSendByte((MMA8452_ADDRESS<<1));// write 0xB4
		i2cWaitForComplete();  
		i2cSendByte(address);// write register address
		i2cWaitForComplete();  
		i2cSendByte(data);
		i2cWaitForComplete();  
		i2cSendStop();
	}

/*******************************************************************************************************/

	//! Assigns a value depending on body position.

	void eHealthClass::bodyPosition( void )
	{  
		if (( position[0] == 0 ) && (position[1] == 1) && (position [2] == 0)) {
			bodyPos = 1;
		} else if (( position[0] == 1 ) && (position[1] == 1) && (position [2] == 0)) {
			bodyPos = 1;
		} else if (( position[0] == 3 ) && (position[1] == 1) && (position [2] == 0)) {
			bodyPos = 1;
		} else if (( position[0] == 2 ) && (position[1] == 0) && (position [2] == 0)) {
			bodyPos = 1;
		} else if (( position[0] == 2 ) && (position[1] == 1) && (position [2] == 1)) {
			bodyPos = 1;
		} else if (( position[0] == 2 ) && (position[1] == 1) && (position [2] == 0)) {
			bodyPos = 1; 
			
		} else if (( position[0] == 0 ) && (position[1] == 1) && (position [2] == 1)) {
			bodyPos = 2;
		} else if (( position[0] == 0 ) && (position[1] == 0) && (position [2] == 1)) {
			bodyPos = 2;
			
		} else if (( position[0] == 1 ) && (position[1] == 1) && (position [2] == 1)) {
			bodyPos = 3;
		} else if (( position[0] == 1 ) && (position[1] == 0) && (position [2] == 1)) {
			bodyPos = 3;
		  
		} else if (( position[0] == 1 ) && (position[1] == 0) && (position [2] == 0)) {
			bodyPos = 4;
		} else if (( position[0] == 3 ) && (position[1] == 0) && (position [2] == 0)) {
			bodyPos = 4;
			
		} else if (( position[0] == 3 ) && (position[1] == 0) && (position [2] == 1)) {
			bodyPos = 5;
		} else if (( position[0] == 3 ) && (position[1] == 1) && (position [2] == 1)) {
			bodyPos = 5;
		} else if (( position[0] == 2 ) && (position[1] == 0) && (position [2] == 1)) {
			bodyPos = 5;
		} else  { 
			bodyPos = 6;
		}
	}

/*******************************************************************************************************/

	//! Converts from 7 segments to number.

	uint8_t eHealthClass::segToNumber(uint8_t A, uint8_t B, uint8_t C, uint8_t D, uint8_t E, uint8_t F, uint8_t G )
	{
		if ((A == 1) && (B == 1) && (C == 1) && (D == 0) && (E == 1) && (F == 1) && (G == 1)) {
			return 0;
	   
		} else if ((A == 0) && (B == 1) && (C == 0) && (D == 0) && (E == 1) && (F == 0) && (G == 0)) {  
			return 1;
		
		} else if ((A == 1) && (B == 1) && (C == 0) && (D == 1) && (E == 0) && (F == 1) && (G == 1)) { 
			return 2;
		
		} else if ((A == 1) && (B == 1) && (C == 0) && (D == 1) && (E == 1) && (F == 0) && (G == 1)) { 
			return 3;
		
		} else if ((A == 0) && (B == 1) && (C == 1) && (D == 1) && (E == 1) && (F == 0) && (G == 0)) { 
			return 4;
		
		} else if ((A == 1) && (B == 0) && (C == 1) && (D == 1) && (E == 1) && (F == 0) && (G == 1)) { 
			return 5;
		
		} else if ((A == 1) && (B == 0) && (C == 1) && (D == 1) && (E == 1) && (F == 1) && (G == 1)) { 
			return 6;
		
		} else if ((A == 1) && (B == 1) && (C == 0) && (D == 0) && (E == 1) && (F == 0) && (G == 0)) {
			return 7;  
		
		} else if ((A == 1) && (B == 1) && (C == 1) && (D == 1) && (E == 1) && (F == 1) && (G == 1)) { 
			return 8;
		
		} else if ((A == 1) && (B == 1) && (C == 1) && (D == 1) && (E == 1) && (F == 0) && (G == 1)) { 
			return 9;
			
		} else  {
			return 0;
		}
	}

/*******************************************************************************************************/

//***************************************************************
// Preinstantiate Objects										*
//***************************************************************

	eHealthClass eHealth = eHealthClass();





