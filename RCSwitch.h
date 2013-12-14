/*
  RCSwitch - Arduino libary for remote control outlet switches
  Copyright (c) 2011 Suat Özgür.  All right reserved.

  Contributors:
  - Andre Koehler / info(at)tomate-online(dot)de
  - Gordeev Andrey Vladimirovich / gordeev(at)openpyro(dot)com
  
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
#ifndef _RCSwitch_h
#define _RCSwitch_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include <wiringPi.h>
    #include <stdint.h>
    #define NULL 0
    #define CHANGE 1
#ifdef __cplusplus
extern "C"{
#endif
typedef uint8_t boolean;
typedef uint8_t byte;

#if !defined(NULL)
#endif
#ifdef __cplusplus
}
#endif
#endif


// Number of maximum High/Low changes per packet.
// We can handle up to (unsigned long) => 32 bit * 2 H/L changes per bit + 2 for sync
// #define RCSWITCH_MAX_CHANGES 67
// Modified By DF4ze : 	unsigned long long => 64 * 2 + 2 for sync ... +1...? \0 fin de chaine?
//						Only send method
#define RCSWITCH_MAX_CHANGES 131


class RCSwitch {

  public:
    RCSwitch();
    
    // void switchOn(char* sCommand);
    void send(char* sCommand);

    void sendTriState(char* Code);

 	// char * get_BinarySent();
 	// void set_BinarySent( char* nBinarySent);
 
    void enableTransmit(int nTransmitterPin);
    void disableTransmit();
    void setPulseLength(int nPulseLength);
    void setRepeatTransmit(int nRepeatTransmit);
    void setReceiveTolerance(int nPercent);
	void setProtocol(int nProtocol);
	void setProtocol(int nProtocol, int nPulseLength);

	char nBinarySent[64];
	
  private:
    char* getCodeWordA(char* sCommand, char * sReturn ); 

    void send0();
    void send1();
    void sendSync();
    void transmit(int nHighPulses, int nLowPulses);

    static char* dec2binWzerofill(unsigned long dec, unsigned int length);
	char * ulltostr(unsigned long long value, char *ptr, int base);
	char * ultostr(unsigned long value, char *ptr, int base);
	
	static void handleInterrupt();

    int nReceiverInterrupt;
    int nTransmitterPin;
    int nPulseLength;
    int nRepeatTransmit;
	char nProtocol;
	

    static unsigned int timings[RCSWITCH_MAX_CHANGES];

    
};

#endif
