#include <signal.h> //Signal 사용 헤더파일
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h> //exit() 사용 헤더파일

#include <wiringPi.h>

#define LIGHTCONTROL  21	//GPIO 5

void sig_handler(int signo); // SIGINT 핸들러 함수

int main (void)
{
	if(wiringPicheck()) printf("Fail\n");
		
	signal(SIGINT, (void *)sig_handler);	
	
	int i;
	while(1)
	{	
		printf("here \n");
		
		pinMode (LIGHTCONTROL, OUTPUT);
		
		//delay(1000);
			
		digitalWrite(LIGHTCONTROL, 1);
		delay(1000);
		digitalWrite(LIGHTCONTROL, 0);
		delay(1000);		
	}	
  
  return 0 ;
}

int wiringPicheck(void)
{
	if (wiringPiSetup () == -1)
	{
		fprintf(stdout, "Unable to start wiringPi: %s\n", strerror(errno));
		return 1 ;
	}
}

void Bpluspinmodeset(void)
{
	pinMode (LIGHTCONTROL, OUTPUT);
	
}

void sig_handler(int signo) // ctrl-c 로 종료시 실행되는 함수
{
    printf("process stop\n"); 	
	digitalWrite(LIGHTCONTROL, 0);
	exit(0);
}


