#include <signal.h> //Signal 사용 헤더파일
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h> //exit() 사용 헤더파일

#include <wiringPi.h>
#include <softPwm.h>

//#define MOTORCONTROL  22	//GPIO 6 - Motor

#define LEDCONTROL  21// LED Bar
//const int LEDCONTROL 21 //  

void sig_handler(int signo); // SIGINT 핸들러 함수

int main (void)
{
	if(wiringPicheck()) printf("Fail\n");
		
	signal(SIGINT, (void *)sig_handler);	
	
	pinMode (LEDCONTROL, OUTPUT);
	
	softPwmCreate(LEDCONTROL, 0, 100);
	
	int i;
	int delaytime = 300;
	while(1)
	{	
		printf("here \n");	
		
		softPwmWrite(LEDCONTROL, 10);
		delay(delaytime);
		softPwmWrite(LEDCONTROL, 20);
		delay(delaytime);
		softPwmWrite(LEDCONTROL, 30);
		delay(delaytime);
		softPwmWrite(LEDCONTROL, 40);
		delay(delaytime);
		softPwmWrite(LEDCONTROL, 50);
		delay(delaytime);
		softPwmWrite(LEDCONTROL, 60);
		delay(delaytime);
		softPwmWrite(LEDCONTROL, 70);
		delay(delaytime);
		softPwmWrite(LEDCONTROL, 80);
		delay(delaytime);
		softPwmWrite(LEDCONTROL, 90);
		delay(delaytime);
		softPwmWrite(LEDCONTROL, 100);
		delay(delaytime);
		softPwmWrite(LEDCONTROL, 0);
		delay(delaytime);		
	}	
  
  return 0 ;
}

int wiringPicheck(void)
{
	if (wiringPiSetup () == -1)
	{
		fprintf(stdout, "Unable to start wiringPi: %s\n", strerror(errno));
		exit(1) ;
	}
}

void Bpluspinmodeset(void)
{
	pinMode (LEDCONTROL, OUTPUT);
	
}

void sig_handler(int signo) // ctrl-c 로 종료시 실행되는 함수
{
    printf("process stop\n"); 	
	digitalWrite(LEDCONTROL, 0);
	exit(0);
}


