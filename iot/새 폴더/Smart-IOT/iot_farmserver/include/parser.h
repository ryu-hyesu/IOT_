
/*****************************************************************************
 *
 * File Name: parser.h
 *
 * Copyright (C) 2014 HUINS,Inc.
 * Programmed by Kim suhak
 * email : shkim@huins.com
 * file creation : 2014/12/15
 *
 *****************************************************************************/
#ifndef _parser_H_
#define _parser_H_
#include "types.h"

extern int parse_data(char * _parameter, Data * _data);
extern int parse_data_str(char * _parameter, DataSTR * _data);
extern int parse_message(char * data, Message * msg);

#endif

