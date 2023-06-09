#include<stdio.h>
#include<wiringPi.h>
#include<unistd.h>

#define trigPin 1	// BCM Pin 18
#define echoPin 29	// BCM Pin 21

int main(void)
{
	int distance=0;
	int pulse = 0;
	
	long startTime;
	long travelTime;
	
	if(wiringPiSetup () == -1)
	{
		printf("Unable GPIO Setup"); 
		return 1;
	}
		
	pinMode (trigPin, OUTPUT);
	pinMode (echoPin, INPUT);
	
	for(;;)
	{
		digitalWrite (trigPin, LOW);
		usleep(2);
		digitalWrite (trigPin, HIGH);
		usleep(20);
		digitalWrite (trigPin, LOW);
		
		while(digitalRead(echoPin) == LOW);
		
		startTime = micros();
		
		while(digitalRead(echoPin) == HIGH);
		travelTime = micros() - startTime;
		
		int distance = travelTime / 58;
		
		printf( "Distance: %dcm\n", distance);
		delay(200);
	}
}
