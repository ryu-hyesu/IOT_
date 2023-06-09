#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <signal.h>

// Use GPIO Pin 3, which is BCM Pin 22
#define FAN 22
#define COLLISION 3  // J14 connect

int flag = 0;

void alarmHandler(){
	digitalWrite(FAN, 0);
	signal(SIGINT, SIG_DFL);
	flag = 0;
}


void myInterrupt(){
	flag = 1;
	signal(SIGINT, SIG_IGN);
	signal(SIGALRM, alarmHandler);
	alarm(6);
	digitalWrite(FAN, 1);
}




int main(void) 
{
	// sets up the wiringPi library
	if (wiringPiSetup () < 0) 
	{
		fprintf (stderr, "Unable to setup wiringPi: %s\n", strerror (errno));
		return 1;
	}
	
	pinMode(FAN, OUTPUT);
	pinMode(COLLISION, INPUT);
	


	if ( wiringPiISR (COLLISION , INT_EDGE_RISING, &myInterrupt ) < 0 ){
		printf("에러");
		return 1;
	}
	
	while(1){
		
		if ( flag == 0 ){
			printf("monitoring ...\n ");
			delay(3000);
		}

	}
}
