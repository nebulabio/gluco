/*
	PinChangeInt.h

	See google code project for latest, bugs and info http://code.google.com/p/arduino-pinchangeint/
	For more information Refer to avr-gcc header files, arduino source and atmega datasheet.

	This library was inspired by and derived from "johnboiles" (it seems) 
	PCInt Arduino Playground example here: http://www.arduino.cc/playground/Main/PcInt
	If you are the original author, please let us know at the google code page
	
	It provides an extension to the interrupt support for arduino by
	adding pin change interrupts, giving a way for users to have
	interrupts drive off of any pin.

	Please make any configuration changes in the accompanying PinChangeIntConfig.h file.
	This will help avoid having to reset your config in the event of changes to the 
	library code (just don't replace that file when you update).

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.

	*/

#ifndef _STDDEF_H
#include "stddef.h"
#endif

#ifndef PinChangeInt_h
#define	PinChangeInt_h

#include "PinChangeIntConfig.h"
#ifndef Arduino_h
#include "Arduino.h"
#endif

/*
* Theory: all IO pins on Atmega168 are covered by Pin Change Interrupts.
* The PCINT corresponding to the pin must be enabled and masked, and
* an ISR routine provided.  Since PCINTs are per port, not per pin, the ISR
* must use some logic to actually implement a per-pin interrupt service.
*/

/* Pin to interrupt map:
* D0-D7 = PCINT 16-23 = PCIR2 = PD = PCIE2 = pcmsk2
* D8-D13 = PCINT 0-5 = PCIR0 = PB = PCIE0 = pcmsk0
* A0-A5 (D14-D19) = PCINT 8-13 = PCIR1 = PC = PCIE1 = pcmsk1
*/

#ifndef MAX_PIN_CHANGE_PINS
#define	MAX_PIN_CHANGE_PINS 1
#endif

// You can reduce the memory footprint of this handler by declaring that there will be no pin change interrupts
// on any of the three ports.
// define NO_PORTB_PINCHANGES to indicate that port b will not be used for pin change interrupts
// define NO_PORTC_PINCHANGES to indicate that port c will not be used for pin change interrupts
// define NO_PORTD_PINCHANGES to indicate that port d will not be used for pin change interrupts
// If only a single port remains, the handler will be declared inline reducing the size and latency 
// of the handler.

// if their is only one PCInt vector in use the code can be inlined
// reducing latecncy and code size
#define	INLINE_PCINT
#if ((defined(NO_PORTB_PINCHANGES) && defined(NO_PORTC_PINCHANGES)) || \
			(defined(NO_PORTC_PINCHANGES) && defined(NO_PORTD_PINCHANGES)) || \
			(defined(NO_PORTD_PINCHANGES) && defined(NO_PORTB_PINCHANGES)))
#undef INLINE_PCINT
#define	INLINE_PCINT inline
#endif

// Provide drop in compatibility with johnboiles PCInt project at
// http://www.arduino.cc/playground/Main/PcInt
#define	PCdetachInterrupt(pin)	PCintPort::detachInterrupt(pin)
#define	PCattachInterrupt(pin,userFunc,mode) PCintPort::attachInterrupt(pin, userFunc,mode)


typedef void (*PCIntvoidFuncPtr)(void);

class PCintPort {
	PCintPort(int index,volatile uint8_t& maskReg) :
	portInputReg(*portInputRegister(index + 2)),
	pcmask(maskReg),
	PCICRbit(1 << index),
	PCintLast(0)	{
		for (int i = 0; i < 9; i++) {
			pcIntPins[i] = NULL;
		}
	}
public:
	static		void attachInterrupt(uint8_t pin, PCIntvoidFuncPtr userFunc, int mode);
	static		void detachInterrupt(uint8_t pin);
	INLINE_PCINT void PCint();
	static		PCintPort	pcIntPorts[];
	
protected:
	class PCintPin {
	public:
		PCintPin() :
		PCintFunc((PCIntvoidFuncPtr)NULL),
		PCintMode(0) {}
		PCIntvoidFuncPtr PCintFunc;
		uint8_t 	PCintMode;
		uint8_t		PCIntMask;
		static PCintPin	pinDataAlloc[MAX_PIN_CHANGE_PINS];
	};
	void		addPin(uint8_t mode,uint8_t mask,PCIntvoidFuncPtr userFunc);
	void		delPin(uint8_t mask);
	volatile	uint8_t&		portInputReg;
	volatile	uint8_t&		pcmask;
	const		uint8_t			PCICRbit;
	uint8_t			PCintLast;
	PCintPin*	pcIntPins[9];	// extra entry is a barrier
};
#endif
