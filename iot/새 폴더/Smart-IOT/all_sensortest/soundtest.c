#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <wiringPi.h>

// Use GPIO Pin 0, which is Pin 17 for wiringPi library

#define SOUND_IN 0  // J12 Connector

unsigned char soundFlag = 0;
// -------------------------------------------------------------------------
// myInterrupt:  called every time an event occurs
void myInterrupt(void) {
	soundFlag = 1;

}

// -------------------------------------------------------------------------
// main
int main(void) {
	// sets up the wiringPi library
	if (wiringPiSetup () < 0) {
		fprintf (stderr, "Unable to setup wiringPi: %s\n", strerror (errno));
		return 1;
	}

	// set Pin 17/0 generate an interrupt on high-to-low transitions
	// and attach myInterrupt() to the interrupt
	if ( wiringPiISR (SOUND_IN, INT_EDGE_RISING, &myInterrupt) < 0 ) {
		fprintf (stderr, "Unable to setup ISR: %s\n", strerror (errno));
		return 1;
	}
	
	while(1)
	{
		if(soundFlag == 1){
			printf( "SoundDetect \n");
			soundFlag = 0;
		}
		else
		{
			printf("No Sound \n");
			soundFlag = 0;
		}
		delay(100);
	}
	return 0;
}
