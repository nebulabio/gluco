/*
	PinChangeInt.cpp
	
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

	see google code project for latest, bugs and info http://code.google.com/p/arduino-pinchangeint/
*/
#ifndef WProgram_h
#include "Arduino.h"
#endif
#ifndef PinChangeInt_h
#include <PinChangeInt.h>
#endif

PCintPort::PCintPin PCintPort::PCintPin::pinDataAlloc[MAX_PIN_CHANGE_PINS];


PCintPort PCintPort::pcIntPorts[] = {
	PCintPort(0,PCMSK0),
	PCintPort(1,PCMSK1),
	PCintPort(2,PCMSK2)
};

void PCintPort::addPin(uint8_t mode,uint8_t mask,PCIntvoidFuncPtr userFunc)
{
	int i = 0;
	PCintPin* p = PCintPin::pinDataAlloc;
	do {
		if (!p->PCintMode) { // found an available pin data structure
			// now find the next slot to put it in
			PCintPin** t = &pcIntPins[0];
			do {
				if (!*t) {	// we have a slot
					// fill in the data
					p->PCintMode = mode;
					*t = p;
					p->PCintFunc = userFunc;
					// set the mask
					pcmask |= p->PCIntMask = mask;
					// enable the interrupt
					PCICR |= PCICRbit;
					return;
				}
			}	 while (int(++t) < int(&pcIntPins[8]));
		}
	} while (int(++p) < int(&PCintPin::pinDataAlloc[MAX_PIN_CHANGE_PINS]));
}

void PCintPort::delPin(uint8_t mask)
{
	PCintPin** t = pcIntPins;
	while (*t) {
		PCintPin& p = **t;
		if (p.PCIntMask == mask) {	// found the target
			uint8_t oldSREG = SREG;
			cli();
			// disable the mask.
			pcmask &= ~mask;
			// if that's the last one, disable the interrupt.
			if (pcmask == 0) {
				PCICR &= ~(PCICRbit);
			}
			p.PCIntMask = 0;
			p.PCintMode = 0;
			p.PCintFunc = NULL;
			do { // shuffle down as we pass through the list, filling the hole
				*t = t[1];
			} while (*t);
			SREG = oldSREG;
			return;
		}
		t++;
	}
}

/*
 * attach an interrupt to a specific pin using pin change interrupts.
 */
void PCintPort::attachInterrupt(uint8_t pin, PCIntvoidFuncPtr userFunc, int mode)
{
	uint8_t portNum = digitalPinToPort(pin);
	if ((portNum == NOT_A_PORT) || (userFunc == NULL)) {
		return;
	}
	// map pin to PCIR register
	uint8_t portIndex = portNum - 2;
	PCintPort& port = PCintPort::pcIntPorts[portIndex];
	port.addPin(mode,digitalPinToBitMask(pin),userFunc);
}

void PCintPort::detachInterrupt(uint8_t pin)
{
	uint8_t portNum = digitalPinToPort(pin);
	if (portNum == NOT_A_PORT) {
		//Serial.println("NOT_A_PORT det");
		return;
	} 
	PCintPort& port = PCintPort::pcIntPorts[portNum - 2];
	port.delPin(digitalPinToBitMask(pin));
}

// common code for isr handler. "port" is the PCINT number.
// there isn't really a good way to back-map ports and masks to pins.
void PCintPort::PCint() {
	#ifndef DISABLE_PCINT_MULTI_SERVICE
	uint8_t pcifr;
	do {
	#endif
		uint8_t curr = portInputReg;
		uint8_t mask = curr ^ PCintLast;
		// get the pin states for the indicated port.
		// mask is pins that have changed. screen out non pcint pins.
		if ((mask &= pcmask) == 0) {
			return;
		}
		PCintLast = curr;
		PCintPin** t = pcIntPins;
		while (*t) {
			PCintPin& p = **t;
			if (p.PCIntMask & mask) { // a changed bit
				// Trigger interrupt if mode is CHANGE, or if mode is RISING and
				// the bit is currently high, or if mode is FALLING and bit is low.
				if (p.PCintMode == CHANGE
					|| ((p.PCintMode == FALLING) && !(curr & p.PCIntMask))
					|| ((p.PCintMode == RISING) && (curr & p.PCIntMask))
					) {
					p.PCintFunc();
				}
			}
			t++;
		}
	#ifndef DISABLE_PCINT_MULTI_SERVICE
		pcifr = PCIFR & PCICRbit;
		PCIFR = pcifr;	// clear the interrupt if we will process is (no effect if bit is zero)
	} while(pcifr);
	#endif
}

#ifndef NO_PORTB_PINCHANGES
ISR(PCINT0_vect) {
	PCintPort::pcIntPorts[0].PCint();
}
#endif

#ifndef NO_PORTC_PINCHANGES
ISR(PCINT1_vect) {
	PCintPort::pcIntPorts[1].PCint();
}
#endif

#ifndef NO_PORTD_PINCHANGES
ISR(PCINT2_vect) {
	PCintPort::pcIntPorts[2].PCint();
}
#endif

