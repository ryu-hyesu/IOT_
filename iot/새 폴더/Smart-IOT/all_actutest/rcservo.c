#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

#include "softServo.h"

#define RCSERVO  23	//GPIO 23

#define servopwron 27 //27

int main(void)
{
  if (wiringPiSetupGpio() == -1)
    return 1 ;

  softServoSetup(RCSERVO,-1,-1,-1,-1,-1,-1,-1);
      
  int i=0;
  
  pinMode(servopwron, OUTPUT);
  delay(1000);
  
  digitalWrite(servopwron, 1);
  
  for( i= 0; i<10 ; i++){
		printf("rc servo on \n");
        delay(1000);
		softServoWrite(RCSERVO, -250);
       
		delay(3000);
        
        printf("rc servo off \n");
  
		softServoWrite(RCSERVO, 1250);
		delay(3000);    
        
  
  };
  
  digitalWrite(servopwron, 0);
}
