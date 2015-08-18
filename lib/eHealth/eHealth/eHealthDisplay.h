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
 *  Author: Luis Martín & Ahmad Saad
 */


// ensure this library description is only included once
#ifndef eHealthClassDisplay_h
#define eHealthClassDisplay_h

#include "Arduino.h"
#include <avr/pgmspace.h> 

// Library interface description
class eHealthDisplayClass {
	
	public: 
  
//***************************************************************
// Constructor of the class										*
//***************************************************************
  
		//! Class constructor.
		eHealthDisplayClass(void);
		 
//***************************************************************
// Public Methods												*
//***************************************************************

		//! Configure and initializes the LCD.
		/*!
		\param void
		\return void
		*/	void init(void);

		//! Configure some parameters of the values screen. 
		/*!
		\param void
		\return void
		*/	void initValuesScreen();

		//! It prints (refresh) the values of the sensors in the screen. 
		/*!
		\param void
		\return void
		*/	void printValuesScreen();

		//! Configure some parameters of the ECG screen. 
		/*!
		\param void
		\return void
		*/	void initECGScreen(void);

		//! It prints (refresh) the ECG wave in the LCD screen.
		/*!
		\param void
		\return void
		*/	void printECGScreen(void);
		
		//! Configure some parameters of the AirFlow screen. 
		/*!
		\param void
		\return void
		*/	void initAirFlowScreen();
		
		//! It prints (refresh) the AirFlow wave in the LCD screen.
		/*!
		\param void
		\return void
		*/	void printAirFlowScreen(void);


//***************************************************************
// Private Methods												*
//***************************************************************

	private:

		//! Clear all data writed in the LCD. 
		void clearLCD(void);

		//! It writes data in decimal format. 
		void writeLcdDec(int data);

		//! It prints data in the LCD.
		void writeLCD(char *data);

		//! It prints a back space in the LCD. 
		void backspace();

		/*! It changes the baud rate uart communication with the LCD. 
			"1" =  4.800bps "2" =  96,00bps
			"3" = 19,200bps "4" =  38,400bps 
			"5" = 57,600bps "6" = 115,200bps
		*/
		void changeBaudrate(char frec);

		//! It places the write pointer in the x-y position.  
		void coordinates(int x, int y);

		//! It activates or deactivates a pixel.  
		void pixel (int x, int y, int onOff);
		
		//! It draws or erases a line. 
		void line (int fromX, int fromY, int toX, int toY, int onOff);

		//! It draws or erases a circule. 
		void circle  (int x, int y, int radio, int onOff);

		//! It draws or erases a rectangle. 
		void rectangle (int fromX, int fromY,
						int toX,
						int toY,
						int onOff);

		//! It clears an area of the LCD. 
		void clearSpace (int fromX, int fromY, int toX, int toY);

		//! It draws an image stored in an array. 
		void image (int desde_x,
					int desde_y,
					int ancho,
					int alto,
					PROGMEM  prog_uint8_t *array);

		//! It prints a lung in the AirFlow screen. 
		void printLung(uint8_t onOff);

		//!It prints the apnea advise.
		void apneaAdvise(uint8_t onOff);

		//! It prints a heart in the LCD.
		void printHeart(uint8_t onOff);

		//! It prints a guy in stand/sit position.  
		void standPosition();

		//! It prints a guy in rigth lateral position.
		void rigthLateralPosition();

		//! It prints a guy in letf lateral position.  
		void leftLateralPosition();

		//! It prints a guy in supine position.
		void supinePosition();

		//! It prints a guy in prone position.
		void pronePosition();

//***************************************************************
// Private Variables											*
//***************************************************************

		//! It stores the number of breaths or beats. 
		int count;

		//! It stores the current value of the ECG or AirFlow sensor. 
		int valRead;

		//! It stores the previous value of the ECG or AirFlow sensor. 
		int prevRead;

		//! Number of counts per minute. It stores the number of
		//! breaths in air flow, and the number of beats in ECG.
		int  countsPerMinute;

		//! The previous value of time. 
		long timePrevious;

		//! It stores the time when the last measure was taked.
		long timePreviousMeassure;

		//! The current time. 
		long time;
		
		//! Previous value of count variable. 
		long countPrevious ;

		//! Vairable who indicates the current state of the wave.
		uint8_t waveState;

		//! It stores the current value of x position in the LCD.
		uint8_t x;

		//! It stores the current value of y position in the LCD.
		uint8_t y;

		//! Auxiliar variable.
		int z;

		//! Auxiliar variable.
		int q;

		//! Auxiliar variable.
		int a;

		//! The final size of the image. 
		int final;

		//! An auxiliar variable.
		unsigned char temp;

		//! It stores the current body position. 
		uint8_t bodyPosition;

		//! It stores the current blood oxygen saturation. 
		uint8_t oxygen;

		//! It stores the current value of pulse.
		uint8_t pulse; 
};

extern eHealthDisplayClass eHealthDisplay;

#endif

