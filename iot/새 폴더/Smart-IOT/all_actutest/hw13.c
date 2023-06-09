#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

int alarmFlag = 1;

void customHandler(){
	printf("사용자 정의 함수");
	exit(0);
}

void alarmHandler(){
	printf("3초 지났음!\n");
	signal(SIGINT, customHandler);
}

int main(void){
	signal(SIGALRM, alarmHandler);
	int x = alarm(3);
	printf("%d", x);
	while(1){
		//pause();
	}
}


