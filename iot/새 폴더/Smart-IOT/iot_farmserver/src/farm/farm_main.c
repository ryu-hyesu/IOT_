
#include <stdio.h>			
#include <stdlib.h>		
#include <string.h>			
#include <unistd.h>		
#include <wiringPi.h>		
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>	
#include <arpa/inet.h>		

#include "my_types.h"

#define PUMP 	21
#define FAN 		22

void 	initialize();
void 	ClientRecv(int);
int 	parse_message(char * data, Message * msg);
int 	parse_data(char * parameter, Data * _data);


int main(int argc, char *argv[])
{
	int 	servSock, clntSock; 
	struct 	sockaddr_in echoServAddr;
	struct 	sockaddr_in echoClntAddr;
	unsigned short 	echoServPort;
	unsigned int 	clntLen;
	int 	iRet;

	initialize();

	if( argc == 2 )
		echoServPort = atoi( argv[1] );
	else	echoServPort = 11000;


	servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if( 0 > servSock )
	{ 	printf("socket() failed"); return 0; 	}

	memset(&echoServAddr, 0, sizeof(echoServAddr));
	echoServAddr.sin_family = AF_INET;
	echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	echoServAddr.sin_port = htons(echoServPort);

	iRet = bind(servSock, (struct sockaddr *)&echoServAddr, sizeof(echoServAddr));
	printf("iRet : %d\n", iRet);
	if( iRet < 0 )
	{ 	printf("bind() failed\n"); close(servSock); return 0; }

	iRet = listen(servSock, 5);
	if( iRet < 0 )
	{ 	printf("listen() failed\n"); close(servSock); return 0; }

	clntLen = sizeof( echoClntAddr );

	printf("starting server...\n");
	clntSock = accept(servSock, (struct sockaddr *)&echoClntAddr, &clntLen);
	if( clntSock < 0 )			
	{ 	printf("accept() failed \n"); 	}	//continue;

	printf("Handling client ip : %s\n", inet_ntoa(echoClntAddr.sin_addr));
	ClientRecv( clntSock );

	close( clntSock );
	close( servSock );
	return 0;
}


void  initialize( )
{
	if( wiringPiSetup() == -1 )
	{ 	printf(" Fail to start wiringPi.. \n"); exit( 0 ); }

	pinMode( PUMP, OUTPUT );
}


void  ClientRecv( int  clntSock )						
{
	unsigned char	ucBuff[500];
	unsigned char	ucSBuff[500];
	int		iRet;

	Data *_data;
    	Message *_message;

	_data = (Data *)malloc(sizeof(Data));
    	_message = (Message *)malloc(sizeof(Message));

	while( 1 )
	{
    		memset(ucBuff, 0x0, sizeof(ucBuff));
		iRet = read( clntSock, ucBuff, 500);
		if( iRet < 1 ) 	break;

		memset(_message, 0x0, sizeof(Message));
        		iRet = parse_message(ucBuff, _message);

        		printf(" App type : 0x%04x \n", _message->_apptype);
        		printf(" Command : 0x%04x \n", _message->_command);
        		printf(" Data : %s \n", _message->_data);

		if( _message->_command == GET_SETTING )
		{
			printf(" Arrive a setting message.. \n");
			char *_result = (char *)malloc(sizeof(char)*4096);
			memset( _result, 0x0, 4096);
			strcpy( _result, "12,0,0");

			memset( ucSBuff, 0x0, sizeof(ucSBuff));
			iRet = sprintf(ucSBuff, "%d|%d|%s,%d,%d,%d,%d", _message->_apptype, _message->_command, _result, 0, 0, 0, 0 );
			printf("Send: %s \n", ucSBuff);
			write( clntSock, ucSBuff, iRet );
		}
		else if( _message->_command == WATER_SUPPLY )
		{
			printf(" Arrive a message for PUMP.. \n");
			memset(_data, 0x0, sizeof(Data));
			iRet = parse_data(_message->_data, _data);
			digitalWrite( PUMP, _data->_data1 );

			iRet = sprintf(ucSBuff, "%d|%d", _message->_apptype, _message->_command );
			write( clntSock, ucSBuff, iRet );
		}
		else if( _message->_command == WATER_FAN )
		{
			printf(" Arrive a message for FAN.. \n");
			memset(_data, 0x0, sizeof(Data));
			iRet = parse_data(_message->_data, _data);
			digitalWrite( FAN, _data->_data1 );

			iRet = sprintf(ucSBuff, "%d|%d", _message->_apptype, _message->_command );
			printf(" %s \n", ucSBuff);
			write( clntSock, ucSBuff, iRet );
		}
		else if( _message->_command == WATER_MOTOR )
		{
			printf(" Arrive a message for MOTOR.. \n");
		}
		else if( _message->_command == WATER_LED )
		{
			printf(" Arrive a message for LED.. \n");
		}
		else 
		{
			printf(" Arrive an unkown message.. \n");
		}

		sleep(1);
	}

	free( _data );
	free( _message );
}


int parse_message(char * data, Message * msg)
{
    char  tmp[128]   = {0};
    char* _parsed_data;
    memcpy(tmp, data, strlen(data));
    
    if(strlen(data) <= 0)
    {
        printf("no data to parse\n");
        return ERROR_PARSE;
    }

    printf("message parsing start :%s \n", tmp);

    _parsed_data = strtok(tmp, "|");
    msg->_apptype = atoi(_parsed_data);

    if(_parsed_data = strtok(NULL, "|"))
        msg->_command = atoi(_parsed_data);
    if(_parsed_data = strtok(NULL, "|"))
        memcpy(msg->_data, _parsed_data, strlen( _parsed_data));

    return TYPE_TRUE;
}


int parse_data(char * parameter, Data * _data)
{
    char* _parsed_data;
    char  tmp[128]   = {0};

    memcpy(tmp, parameter, strlen(parameter));

    if(strlen(parameter) <= 0)
    {
        printf("no data to parse\n");
        return ERROR_PARSE;
    }

    printf("data parsing start :%s \n", parameter);

    _parsed_data = strtok(tmp, ","); _data->_data1 = atoi(_parsed_data);
    if(_parsed_data = strtok(NULL, ",")) _data->_data2 = atoi(_parsed_data);
    if(_parsed_data = strtok(NULL, ",")) _data->_data3 = atoi(_parsed_data);
    if(_parsed_data = strtok(NULL, ",")) _data->_data4 = atoi(_parsed_data);
    if(_parsed_data = strtok(NULL, ",")) _data->_data5 = atoi(_parsed_data);
    if(_parsed_data = strtok(NULL, ",")) _data->_data6 = atoi(_parsed_data);

    return TYPE_TRUE;
}


