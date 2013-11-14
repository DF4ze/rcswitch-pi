/*
 Usage: ./send <sChain>
	// la ligne ci-dessus permet de limiter l'utilisation du './send'
 */

#include "RCSwitch.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    
 	if( argc != 2 ){
		printf( "Mauvais usage : ./send <Chaine>\n" );
		return -1;
	}

	/*
     output PIN is hardcoded for testing purposes
     see https://projects.drogon.net/raspberry-pi/wiringpi/pins/
     for pin mapping of the raspberry pi GPIO connector
     */
    int PIN = 2;
	
	char* sChain = argv[1]; 
	

	///////////////////////////////////////////////////////
	// Si Wiring n'est pas installé, on s'arrete
	// printf( "Verif si WiringPI installed\n" );
    if (wiringPiSetup () == -1){
		printf( "Erreur : WiringPI n'est pas installed\n" );
		return 1;
	}
	
    RCSwitch mySwitch = RCSwitch();
	mySwitch.enableTransmit(PIN);
  
  	printf("Sending...\n");
	mySwitch.send( sChain ); 
	printf( "Binaire envoyé : %s\n", mySwitch.nBinarySent );
	printf("Sent OK\n");
	
	
	return 0;
}
