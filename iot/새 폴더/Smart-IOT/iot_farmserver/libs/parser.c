
/*****************************************************************************
 *
 * File Name: parser.c
 *
 * Copyright (C) 2014 HUINS,Inc.
 * Programmed by Kim suhak
 * email : shkim@huins.com
 * file creation : 2014/12/15
 *
 *****************************************************************************/
#include <stdio.h>			
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "parser.h"


int parse_data_str(char * parameter, DataSTR * _data)
{
    char* _parsed_data;
    char  tmp[128]   = {0};

    memcpy(tmp, parameter, strlen(parameter));

    //1. valified 
    if(strlen(parameter) <= 0)
    {
        printf("no data to parse\n");
        return ERROR_PARSE;
    }

    printf("data parsing start :%s \n", parameter);

    _parsed_data = strtok(tmp, ","); memcpy(_data->_data1, _parsed_data, strlen(_parsed_data));
    if(_parsed_data = strtok(NULL, ",")) memcpy(_data->_data2, _parsed_data, strlen(_parsed_data));
    if(_parsed_data = strtok(NULL, ",")) memcpy(_data->_data3, _parsed_data, strlen(_parsed_data));
    if(_parsed_data = strtok(NULL, ",")) memcpy(_data->_data4, _parsed_data, strlen(_parsed_data));
    if(_parsed_data = strtok(NULL, ",")) memcpy(_data->_data5, _parsed_data, strlen(_parsed_data));
    if(_parsed_data = strtok(NULL, ",")) memcpy(_data->_data6, _parsed_data, strlen(_parsed_data));

    return TYPE_TRUE;

}



int parse_data(char * parameter, Data * _data)
{
    char* _parsed_data;
    char  tmp[128]   = {0};

    memcpy(tmp, parameter, strlen(parameter));

    //1. valified 
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

int parse_message(char * data, Message * msg)
{
    char  tmp[128]   = {0};
    char* _parsed_data;
    memcpy(tmp, data, strlen(data));
    
   //1. valified 
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

