#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <wiringPi.h>


// Use GPIO Pin 0, which is BCM Pin 17

#define LIGHTSEN_OUT 0  //  J12 connect
#define COLLISION 3
int alert_flag = 0;
int n = 0;
void myInterrupt(void){
		printf("dark \n");
		
}
void myInterrupt2(void){
		printf("근접 \n");
		
}

// -------------------------------------------------------------------------
// main
int main(void) 
{
	// sets up the wiringPi library
	if (wiringPiSetup () < 0) 
	{
		fprintf (stderr, "Unable to setup wiringPi: %s\n", strerror (errno));
		return 1;
	}
	
	pinMode(LIGHTSEN_OUT, INPUT);
	pinMode(COLLISION, INPUT);
	// set Pin 17/0 generate an interrupt on high-to-low transitions
	// and attach myInterrupt() to the interrupt

	if ( wiringPiISR (LIGHTSEN_OUT, INT_EDGE_RISING, &myInterrupt) < 0 ) 
	{
		fprintf (stderr, "Unable to setup ISR: %s\n", strerror (errno));
		return 1;
	}

	if ( wiringPiISR (COLLISION, INT_EDGE_RISING, &myInterrupt2) < 0 ) 
	{
		fprintf (stderr, "Unable to setup ISR: %s\n", strerror (errno));
		return 1;
	}
	while(1) {
		//if(alert_flag == 1)
		//	alert_flag = 0;
	}

	// display counter value every second.
	/*while ( 1 ) 
	{
		//printf( "%d\n", eventCounter );
		//eventCounter = 0;
		
		if(digitalRead(LIGHTSEN_OUT) == 0)
			printf("light full ! \n");
		if(digitalRead(LIGHTSEN_OUT) == 1)
			printf("dark \n");		
	
		delay( 200 ); // wait 1 second
	}
	*/
	return 0;
}
