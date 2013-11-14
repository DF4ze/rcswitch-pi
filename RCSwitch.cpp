/*
  RCSwitch - Arduino libary for remote control outlet switches
  Copyright (c) 2011 Suat Özgür.  All right reserved.
  
  Contributors:
  - Andre Koehler / info(at)tomate-online(dot)de
  - Gordeev Andrey Vladimirovich / gordeev(at)openpyro(dot)com
  - Skineffect / http://forum.ardumote.com/viewtopic.php?f=2&t=48
  
  Project home: http://code.google.com/p/rc-switch/

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "RCSwitch.h"
#include <string.h>
#include <stdlib.h>
// #include <stdio.h> 

unsigned int RCSwitch::timings[RCSWITCH_MAX_CHANGES];

RCSwitch::RCSwitch() {
  this->nTransmitterPin = -1;
  this->setPulseLength(350);
  this->setRepeatTransmit(10);
  this->setProtocol(1);
  // this->nBinarySent = "";
}

/**
  * Sets the protocol to send.
  */
void RCSwitch::setProtocol(int nProtocol) {
  this->nProtocol = nProtocol;
  if (nProtocol == 1){
	  this->setPulseLength(350);
  }
  else if (nProtocol == 2) {
	  this->setPulseLength(650);
  }
}

/**
  * Sets the protocol to send with pulse length in microseconds.
  */
void RCSwitch::setProtocol(int nProtocol, int nPulseLength) {
  this->nProtocol = nProtocol;
  if (nProtocol == 1){
	  this->setPulseLength(nPulseLength);
  }
  else if (nProtocol == 2) {
	  this->setPulseLength(nPulseLength);
  }
}


/**
  * Sets pulse length in microseconds
  */
void RCSwitch::setPulseLength(int nPulseLength) {
  this->nPulseLength = nPulseLength;
}

/**
 * Sets Repeat Transmits
 */
void RCSwitch::setRepeatTransmit(int nRepeatTransmit) {
  this->nRepeatTransmit = nRepeatTransmit;
}


/**
 * Enable transmissions
 *
 * @param nTransmitterPin    Arduino Pin to which the sender is connected to
 */
void RCSwitch::enableTransmit(int nTransmitterPin) {
  this->nTransmitterPin = nTransmitterPin;
  pinMode(this->nTransmitterPin, OUTPUT);
}

/**
  * Disable transmissions
  */
void RCSwitch::disableTransmit() {
  this->nTransmitterPin = -1;
}




/**
  * Turns a decimal value to its binary representation
  */
char* RCSwitch::dec2binWzerofill( unsigned long Dec, unsigned int bitLength){
  static char bin[64];
  unsigned int i=0;

  while (Dec > 0) {
    bin[32+i++] = ((Dec & 1) > 0) ? '1' : '0';
    Dec = Dec >> 1;
  }

  for (unsigned int j = 0; j< bitLength; j++) {
    if (j >= bitLength - i) {
      bin[j] = bin[ 31 + i - (j - (bitLength - i)) ];
    }else {
      bin[j] = '0';
    }
  }
  bin[bitLength] = '\0';
 
  return bin;
}

/**
  * Turns a ull value to its binary representation... or anything else (base : 2, 6, 8, 10, 12, 14, 16...)
  */
char * RCSwitch::ulltostr(unsigned long long value, char *ptr, int base)
{
  unsigned long long t = 0, res = 0;
  unsigned long long tmp = value;
  int count = 0;

  if (NULL == ptr)
    return NULL;
  
  if (tmp == 0)
    count++;


  while(tmp > 0)  {
    tmp = tmp/base;
    count++;
  }

  ptr += count;
  *ptr = '\0';

  do{
    res = value - base * (t = value / base);
    if (res < 10)
      * --ptr = '0' + res;
    else if ((res >= 10) && (res < 16))
        * -- ptr = 'A' - 10 + res;
  } while ((value = t) != 0);

  return(ptr);
}
char * RCSwitch::ultostr(unsigned long value, char *ptr, int base)
{
  unsigned long t = 0, res = 0;
  unsigned long tmp = value;
  int count = 0;

  if (NULL == ptr)
    return NULL;
  
  if (tmp == 0)
    count++;
  
  while(tmp > 0){
    tmp = tmp/base;
    count++;
  }

  ptr += count;
  *ptr = '\0';

  do{
    res = value - base * (t = value / base);
    if (res < 10)
      * -- ptr = '0' + res;
    else if ((res >= 10) && (res < 16))
      * --ptr = 'A' - 10 + res;
  } while ((value = t) != 0);

  return(ptr);
}


/**
 * Switch a remote switch on (Type A with 10 pole DIP switches)
 *
 * @param sGroup        Code of the switch group (refers to DIP switches 1..5 where "1" = on and "0" = off, if all DIP switches are on it's "11111")
 * @param nChannelCode  Number of the switch itself (1..4)
 */
// void RCSwitch::switchOn( char* sCommand ) {
void RCSwitch::send( char* sCommand ) {
  this->sendTriState( this->getCodeWordA( sCommand ) );
}

/**
 * Like getCodeWord  (Type A)
 */

char* RCSwitch::getCodeWordA(char* sCommand) {
   
 	unsigned int iTaille =4;
	// char* sReturn;
	char sReturn[64];
	unsigned long ulCommand = 0;
	unsigned long long ullCommand = 0;
   
 	ullCommand = strtoull( sCommand, NULL, 0 );
   
	// Apparemment obligé d'initialiser sReturn avant de le passer a ulltostr....
	// sReturn = this->dec2binWzerofill( ulCommand, iTaille );
 	this->ulltostr(ullCommand, sReturn, 2);
   
	strcpy( this->nBinarySent, sReturn );

	return sReturn;
}


/**
 * Sends a Code Word
 * @param sCodeWord   /^[10FS]*$/  -> see getCodeWord
 */
void RCSwitch::sendTriState(char* sCodeWord) {
  for (int nRepeat=0; nRepeat<nRepeatTransmit; nRepeat++) {
    int i = 0;
    while (sCodeWord[i] != '\0') {
      switch(sCodeWord[i]) {
        case '0':
          this->send0();
        break;
        case '1':
          this->send1();
        break;
      }
      i++;
    }
    this->sendSync();    
  }
}

void RCSwitch::transmit(int nHighPulses, int nLowPulses) {
    // boolean disabled_Receive = false;
    // int nReceiverInterrupt_backup = nReceiverInterrupt;
    if (this->nTransmitterPin != -1) {
        // if (this->nReceiverInterrupt != -1) {
            // this->disableReceive();
            // disabled_Receive = true;
        // }
        digitalWrite(this->nTransmitterPin, HIGH);
        delayMicroseconds( this->nPulseLength * nHighPulses);
        digitalWrite(this->nTransmitterPin, LOW);
        delayMicroseconds( this->nPulseLength * nLowPulses);
        // if(disabled_Receive){
            // this->enableReceive(nReceiverInterrupt_backup);
        // }
    }
}
/**
 * Sends a "0" Bit
 *                       _    
 * Waveform Protocol 1: | |___
 *                       _  
 * Waveform Protocol 2: | |__
 */
void RCSwitch::send0() {
	if (this->nProtocol == 1){
		this->transmit(1,3);
	}
	else if (this->nProtocol == 2) {
		this->transmit(1,2);
	}
}

/**
 * Sends a "1" Bit
 *                       ___  
 * Waveform Protocol 1: |   |_
 *                       __  
 * Waveform Protocol 2: |  |_
 */
void RCSwitch::send1() {
  	if (this->nProtocol == 1){
		this->transmit(3,1);
	}
	else if (this->nProtocol == 2) {
		this->transmit(2,1);
	}
}


/**
 * Sends a "Sync" Bit
 *                       _
 * Waveform Protocol 1: | |_______________________________
 *                       _
 * Waveform Protocol 2: | |__________
 */
void RCSwitch::sendSync() {

    if (this->nProtocol == 1){
		this->transmit(1,31);
	}
	else if (this->nProtocol == 2) {
		this->transmit(1,10);
	}
}


/**
 * Binaire qui a été envoyé
**/
/* char * get_BinarySent(){
	return this->nBinarySent;
}
void set_BinarySent( char * nBinarySent ){
	this->nBinarySent = nBinarySent;
} */

