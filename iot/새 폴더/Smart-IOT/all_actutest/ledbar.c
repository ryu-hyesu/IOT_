#include <signal.h> //Signal 사용 헤더파일
#include <unistd.h>
#include <stdio.h> 
#include <string.h> 
#include <errno.h>
#include <stdlib.h> //exit() 사용 헤더파일

#include <wiringPi.h>

#define LEDBAR	26 // BCM_GPIO 12

void sig_handler(int signo); // 마지막 종료 함수

int main (void)
{
	signal(SIGINT, (void *)sig_handler);	//시그널 핸들러 함수
	
	if (wiringPiSetup () == -1)
	{
		fprintf(stdout, "Unable to start wiringPi: %s\n", strerror(errno));
		return 1 ;
	}

	pinMode (LEDBAR, OUTPUT) ;

	for (;;)
	{
		printf("here - Led Bar on on\n");
		digitalWrite (LEDBAR, 1) ; // On
		
		delay (200) ; // ms
		
		printf("here - Led Bar on on\n");
		digitalWrite (LEDBAR, 0) ; // Off
		
		delay (200) ;
		
	}
	return 0 ;
}

void sig_handler(int signo)
{
    printf("process stop\n");
	digitalWrite (LEDBAR, 0) ; // Off
	
	exit(0);
} 



