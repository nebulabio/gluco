/*
 *  eHealth sensor platform for Arduino and Raspberry from Cooking-hacks.
 *
 *  Description: "The e-Health Sensor Shield allows Arduino and Raspberry Pi
 *  users to perform biometric and medical applications by using 9 different
 *  sensors: Pulse and Oxygen in Blood Sensor (SPO2), Airflow Sensor
 * (Breathing),
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

// Ensure this library description is only included once
#ifndef eHealthClass_h
#define eHealthClass_h

#include "Arduino.h"

// Library interface description
class eHealthClass
{

public:
  //***************************************************************
  // Constructor of the class
  // *
  //***************************************************************

  //! Class constructor.
  eHealthClass(void);

  //***************************************************************
  // Public Methods
  // *
  //***************************************************************

  //! Initializes the position sensor and configure some values.
  /*!
		\param void
		\return void
		*/ void initPositionSensor(void);

  //! Initializes the BloodPressureSensor sensor and configure some values
  /*!
		\param float parameter with correction value
		\return void
		*/ void initBloodPressureSensor(float parameter);

  //! Initializes the pulsioximeter sensor and configure some values.
  /*!
		\param void
		\return void
		*/ void initPulsioximeter(void);

  //! Returns the corporal temperature.
  /*!
		\param void
		\return float : The corporal temperature value.
		*/ float getTemperature(void);

  //! Returns the oxygen saturation in blood in percent.
  /*!
		\param void
		\return int : The oxygen saturation value. Normal values betwen 95-99%
		*/ int getOxygenSaturation(void);

  //! Returns the heart beats per minute.
  /*!
		\param void
		\return int : The beats per minute.
		*/ int getBPM(void);

  //! Returns the value of skin conductance.
  /*!
		\param void
		\return float : The skin conductance value.
		*/ float getSkinConductance(void);

  //! Returns the value of skin resistance.
  /*!
		\param void
		\return float : The skin resistance value.
		*/ float getSkinResistance(void);

  //! Returns the value of skin conductance in voltage.
  /*!
		\param void
		\return float : The skin conductance value in voltage (0-5v).
		*/ float getSkinConductanceVoltage(void);

  //! Returns an analogic value to represent the Electrocardiography.
  /*!
		\param void
		\return float : The analogic value (0-5V).
		*/ float getECG(void);

  //! Returns the body position.
  /*!
		\param void
		\return uint8_t : the position of the pacient.
		 *		1 == Supine position.
		 *		2 == Left lateral decubitus.
		 *		3 == Rigth lateral decubitus.
		 *		4 == Prone position.
		 *		5 == Stand or sit position
		 */ uint8_t getBodyPosition(void);

  //! Returns the  value of the systolic pressure.
  /*!
		\param void
		\return int : The systolic pressure.
		*/ int getSystolicPressure(void);

  //! Returns the  value of the diastolic pressure.
  /*!
		\param void
		\return int : The diastolic pressure.
		*/ int getDiastolicPressure(void);

  //! Returns an analogic value to represent the air flow.
  /*!
		\param void
		\return int : The value (0-1023) read from the analogic in.
		*/ int getAirFlow(void);

  //! Prints the current body position
  /*!
		\param uint8_t position : the current body position.
		\return void
		*/ void printPosition(uint8_t position);

  //! It reads a value from pulsioximeter sensor.
  /*!
		\param void
		\return void
		*/ void readPulsioximeter(void);

  //!  Prints air flow wave form in the serial monitor
  /*!
		\param int air : analogic value to print.
		\return void
		*/ void airFlowWave(int air);

  //!  Read the values stored in the glucometer.
  /*!
		\param void
		\return void
		*/ void readGlucometer(void);

  //! Returns the number of data stored in the glucometer.
  /*!
		\param void
		\return int : length of data
		*/ uint8_t getGlucometerLength(void);

  //!  Returns the library version
  /*!
		\param void
		\return int : The library version.
		*/ int version(void);

  //! Convert month variable from numeric to character.
  /*!
		 \param int month in numerical format.
		 \return String with the month characters (January, February...).
		 */ String numberToMonth(int month);

  //! Struct to store data of the glucometer.
  struct glucoseData
  {
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minutes;
    uint8_t glucose;
    uint8_t meridian;
  };

  //! Vector to store the glucometer measures and dates.
  glucoseData glucoseDataVector[8];

private:
  //***************************************************************
  // Private Methods
  // *
  //***************************************************************

  //! Initialize the MMA8452 registers
  void initMMA8452(byte fsr, byte dataRate);

  //! Sets the MMA8452 to standby mode. It must be in standby to change most
  //! register settings.
  void MMA8452Standby();

  //! Sets the MMA8452 to active mode. Needs to be in this mode to output data.
  void MMA8452Active();

  //! Read i registers sequentially, starting at address into the dest byte
  //! array
  void readRegisters(byte address, int i, byte* dest);

  //! Read a single byte from address and return it as a byte.
  byte readRegister(uint8_t address);

  //! Writes a single byte (data) into address.
  void writeRegister(unsigned char address, unsigned char data);

  //! This function will read the p/l source register and
  //! print what direction the sensor is now facing.
  void portraitLandscapeHandler();

  //! Assigns a value depending on body position.
  void bodyPosition(void);

  //! Converts from 7 segments to number.
  uint8_t segToNumber(uint8_t A, uint8_t B, uint8_t C, uint8_t D, uint8_t E,
                      uint8_t F, uint8_t G);

  //***************************************************************
  // Private Variables
  // *
  //***************************************************************

  //! It stores the systolic pressure value
  int systolic;

  //! It stores the diastolic pressure value
  int diastolic;

  //! It stores the  beats per minute value.
  int BPM;

  //! It stores blood oxigen saturation value.
  int SPO2;

  //! It stores current body position.
  uint8_t bodyPos;

  //! x/y/z accel register data store here.
  byte data[6];

  //! Stores the 12-bit signed value.
  int accelCount[3];

  //! Stores the real accel value in g's.
  float accel[3];

  //! Stores the body position in vector value.
  uint8_t position[];

  //! It stores the number of data of the glucometer.
  uint8_t length;
};

extern eHealthClass eHealth;

#endif
