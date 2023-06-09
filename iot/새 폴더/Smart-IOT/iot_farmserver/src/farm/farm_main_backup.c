
/***************************************************************************** *
 * File Name: main.c
 *
 * Copyright (C) 2014 HUINS,Inc.
 * Programmed by Kim suhak
 * email : shkim@huins.com
 * file creation : 2014/12/11
 *
 *****************************************************************************/

#include <stdio.h>			
#include <sys/socket.h>	
#include <arpa/inet.h>		
#include <stdlib.h>		
#include <string.h>			
#include <unistd.h>		
#include <sys/time.h>
#include <sys/types.h>
#include "iot_db.h"
#include "types.h"
#include "parser.h"
#include "farm_sensor.h"
#include "farm_query.h"

#define MAXCLIENT   10
#define MAXPENDING 	5
#define DB_NAME "iotfarm"
#define SENSOR_LOG_INTERVAL 1*60*10
#define ACTUATOR_LOG_INTERVAL 1*60*10

typedef struct _TInfo
{
	unsigned int	uiUser;
	int				iSock;
	pthread_t		tID;
} TInfo;



#ifndef TEST_MODE
int get_temperature_sensor();
#endif

void *SensorInterruptLoop(void *);

void *ClientRecv(void *);
int parse_message(char * data, Message * msg);
int parse_data(char * parameter, Data * _data);

unsigned int		uiUser;
TInfo *             stpLink[MAXCLIENT];
pthread_mutex_t		MLock;

int _use_pump = 0;
int _use_fan = 0;
int _use_dcmotor = 0;
int _use_rgbled = 0;

int main(int iArg, char *cpArg[])
{
	int servSock; 
	TInfo stTempInfo;
	struct sockaddr_in echoServAddr;
	struct sockaddr_in echoClntAddr;
	unsigned short echoServPort;
	unsigned int clntLen;
	int iRet;
	char _query[128] = {0};

//sensor
	int sensor_loop;
	int int_loop_result;

	char _choice;

    //DB control 
    iot_connect_to_db(DB_NAME);

	/*
	printf("Do you want to create a new database? (y/n)\n");
	getc(&_choice);

	if(_choice == 'y')
	{
	*/
	printf("validate database...\n");
	iot_send_query(QUERY_CREATE_TABLE_SENSOR_VALUE);
	iot_send_query(QUERY_CREATE_TABLE_SENSOR_CHECK);
	iot_send_query(QUERY_CREATE_TABLE_ACTURATOR_VALUE);
	iot_send_query(QUERY_CREATE_TABLE_ACTURATOR_CHECK);
	iot_send_query(QUERY_CREATE_TABLE_SETTING);
	printf("done\n");
	//}
	//check setting

	iRet = iot_count_setting_data_from_db(QUERY_SELECT_COUNT_SETTING);

	printf("validate setting data... : %d \n", iRet);
	if(iRet == 0)
	{
		sprintf(_query,QUERY_INSERT_SETTING,12,0,0);
		iot_insert_data_from_db(_query);
	}

	if (1 == iArg)
	{
		echoServPort = 11000;
	}
	else if (2 == iArg)
	{
		echoServPort = atoi(cpArg[1]);
	}


	servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(0 > servSock)
	{
		printf("socket() failed");

		return 0;
	}

	memset(&echoServAddr, 0, sizeof(echoServAddr));
	echoServAddr.sin_family = AF_INET;
	echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	echoServAddr.sin_port = htons(echoServPort);

	iRet = bind(servSock, (struct sockaddr *)&echoServAddr, sizeof(echoServAddr));
	if(0 > iRet)
	{
		close(servSock);
		printf("bind() failed\n");

		return 0;
	}

	iRet = listen(servSock, MAXPENDING);
	if(0 > iRet)
	{
		close(servSock);
		printf("listen() failed\n");

		return 0;
	}

	clntLen = sizeof(echoClntAddr);

	uiUser = 0;

	iRet = pthread_mutex_init(&MLock, NULL);			
    if( iRet != 0)
    {
        close(servSock);
        printf("pthread init failed\n");

		return 0;
	}

	pthread_mutex_lock (&MLock);		
	pthread_create (&sensor_loop, 0, SensorInterruptLoop, &int_loop_result);
	pthread_mutex_unlock (&MLock);

	printf("starting server...\n");
	while(1)
	{
		stTempInfo.iSock = accept(servSock, (struct sockaddr *)&echoClntAddr, &clntLen);
		if(0 > stTempInfo.iSock)			
		{
			printf("accept() failed \n");
			continue;	
		}	
		printf("Handling client ip : %s\n", inet_ntoa(echoClntAddr.sin_addr));
		printf("Handling client port : %d\n", ntohs(echoClntAddr.sin_port));
		printf("Handling client socket number : %d\n", stTempInfo.iSock);
		pthread_mutex_lock (&MLock);		
		stTempInfo.uiUser = uiUser;
		pthread_create (&stTempInfo.tID, 0, ClientRecv, &stTempInfo);
		++uiUser;								
		pthread_mutex_unlock (&MLock);

		while (0 != stTempInfo.iSock);			
        printf("connected user  : %d\n", uiUser);
	}

	close(servSock);

    //DB control
    iot_disconnect_from_db();

	return 0;
}


void *SensorInterruptLoop(void *vp)
{
	char _query[256] = {0};

	float _temperature_sensor = 1;
	float _humid_sensor = 1;
	int _light_sensor = 1;
	

	while(1)
	{
#ifndef TEST_MODE
		_temperature_sensor = get_temperature_sensor();
		_humid_sensor = get_humidity_sensor();
		_light_sensor = get_light_sensor();
#endif

		sprintf(_query,QUERY_INSERT_SENSOR_DATA, _temperature_sensor, _humid_sensor, _light_sensor);
		printf("%s : write to DB - %s\n", __func__, _query);
		iot_insert_data_from_db(_query);
		sprintf(_query,QUERY_INSERT_SENSOR_CHECK, _temperature_sensor, _humid_sensor, _light_sensor);
		printf("%s : write to DB - %s\n", __func__, _query);
		iot_insert_data_from_db(_query);
		sprintf(_query,QUERY_INSERT_ACTUATOR_VALUE, _use_pump, _use_fan, _use_dcmotor, _use_rgbled);
		printf("%s : write to DB - %s\n", __func__, _query);
		iot_insert_data_from_db(_query);
#ifndef TEST_MODE
		sprintf(_query,QUERY_INSERT_ACTUATOR_CHECK, get_pump_functionality(), get_fan_functionality(), get_dcmotor_functionality(), get_rgbled_functionality());
		printf("%s : write to DB - %s\n", __func__, _query);
		iot_insert_data_from_db(_query);
#endif
		sleep(SENSOR_LOG_INTERVAL);
	}
}

void *ClientRecv(void *vp)						
{
	unsigned char	ucBuff[500];
	unsigned char	ucSBuff[500];
	unsigned int	uiCnt;
	int				iRet;
	TInfo			stMyInfo = *((TInfo *)vp);

    Message * _message;
	Data * _data;

	_data = (Data *)malloc(sizeof(Data));
    _message = (Message *)malloc(sizeof(Message));

	memset(_data, 0x0, sizeof(Data));
    memset(_message, 0x0, sizeof(Message));

	stpLink[stMyInfo.uiUser] = &stMyInfo;
	((TInfo *)vp)->iSock = 0;					
												
	while(1)
	{
		iRet = read (stMyInfo.iSock, ucBuff, 500);
		if (1 > iRet)
		{
			break;
		}
		printf ("[%dSock][MyUserNum:%d]:[%s]\n", stMyInfo.iSock, stMyInfo.uiUser, ucBuff);

		memset(_message, 0x0, sizeof(Message));
        iRet = parse_message(ucBuff, _message);

        printf(" app type : 0x%04x\n", _message->_apptype);
        printf(" command : 0x%04x\n", _message->_command);
        printf(" data : %s\n", _message->_data);

		switch(_message->_apptype)
		{
			case FARM :
				if(_message->_command == GET_STATUS)
				{
					int _temperature_sensor = 1;
					int _humid_sensor = 1;
					int _light_sensor = 1;
					//TODO:: get sensor data
					//
					_temperature_sensor = get_temperature_sensor();
					_humid_sensor = get_humidity_sensor();
					_light_sensor = get_light_sensor();

					iRet = sprintf (ucSBuff, "%d|%d|%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", _message->_apptype, _message->_command, _temperature_sensor, _humid_sensor, _light_sensor, get_light_functionality(), get_humid_functionality(), get_temp_functionality(), get_pump_functionality(), get_fan_functionality(), get_dcmotor_functionality(), get_rgbled_functionality());
					write (stMyInfo.iSock, ucSBuff, iRet);
				}
				else if(_message->_command == GET_SETTING)
				{
					int _period = 1;
					int _everytime = 14;
					int _watertime = 30;
					//TODO:: get period for water supply
					char * _result = (char *)malloc(sizeof(char)*4096);
					memset(_result, 0x0, 4096);
					iot_get_setting_data_from_db(QUERY_SELECT_SETTING, _result);
					iRet = sprintf (ucSBuff, "%d|%d|%s,%d,%d,%d,%d", _message->_apptype, _message->_command, _result, _use_pump, _use_fan, _use_dcmotor, _use_rgbled);
					free(_result);
					write (stMyInfo.iSock, ucSBuff, iRet);
				}
				else if(_message->_command == SET_SETTING)
				{
					memset(_data, 0x0, sizeof(Data));
					iRet = parse_data(_message->_data, _data);

					sprintf(ucSBuff,QUERY_UPDATE_SETTING,_data->_data1,_data->_data2,_data->_data3);
					iot_update_data_from_db(ucSBuff);

					iRet = sprintf (ucSBuff, "%d|%d", _message->_apptype, _message->_command );
					write (stMyInfo.iSock, ucSBuff, iRet);
				}
				else if(_message->_command == GET_HISTORY)
				{
					char * _result = (char *)malloc(sizeof(char)*4096);

					memset(_result, 0x0, 4096);
					iot_get_history_data_from_db(QUERY_SELECT_SENSOR_DATA, _result);
					iRet = sprintf (ucSBuff, "%d|%d|%s", _message->_apptype, _message->_command, _result);
					//iRet = sprintf (ucSBuff, "%d|%d", _message->_apptype, _message->_command);
					printf("%s\n",ucSBuff);
					free(_result);
					write (stMyInfo.iSock, ucSBuff, iRet);
				}
				else if(_message->_command == WATER_SUPPLY)
				{
					memset(_data, 0x0, sizeof(Data));
					iRet = parse_data(_message->_data, _data);
					_use_pump = _data->_data1;
					set_waterpump_active(_data->_data1);
					iRet = sprintf (ucSBuff, "%d|%d", _message->_apptype, _message->_command );
					write (stMyInfo.iSock, ucSBuff, iRet);
				}
				else if(_message->_command == WATER_FAN)
				{
					memset(_data, 0x0, sizeof(Data));
					iRet = parse_data(_message->_data, _data); 
					_use_fan = _data->_data1;
					set_fan_active(_data->_data1);
					iRet = sprintf (ucSBuff, "%d|%d", _message->_apptype, _message->_command );
					write (stMyInfo.iSock, ucSBuff, iRet);

				}
				else if(_message->_command == WATER_MOTOR)
				{
					memset(_data, 0x0, sizeof(Data));
					iRet = parse_data(_message->_data, _data); 
					_use_dcmotor = _data->_data1;
					set_dcmotor_active(_data->_data1);
					iRet = sprintf (ucSBuff, "%d|%d", _message->_apptype, _message->_command );
					write (stMyInfo.iSock, ucSBuff, iRet);

				}
				else if(_message->_command == WATER_LED)
				{
					memset(_data, 0x0, sizeof(Data));
					iRet = parse_data(_message->_data, _data); 
					_use_rgbled = _data->_data1;
                    printf("RGBled on/off \n");
					set_rgbled_active(_data->_data1);
					iRet = sprintf (ucSBuff, "%d|%d", _message->_apptype, _message->_command );
					write (stMyInfo.iSock, ucSBuff, iRet);

				}
				break;
		}


#ifdef USE_BROADCAST
		for (uiCnt=0 ; uiUser>uiCnt; ++uiCnt)
		{
			if (&stMyInfo == stpLink[uiCnt])
			{
				continue;
			}
			write (stpLink[uiCnt]->iSock, ucSBuff, iRet);
			flush();
		}
#endif
		sleep(1);
	}

	free(_data);
	free(_message);
	pthread_mutex_lock (&MLock);			
	--uiUser;	
	stpLink[stMyInfo.uiUser] = stpLink[uiUser];
	stpLink[stMyInfo.uiUser]->uiUser = stMyInfo.uiUser;
	pthread_mutex_unlock (&MLock);
	close(stMyInfo.iSock);
	return 0;
}

