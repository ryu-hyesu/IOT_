
/*****************************************************************************
 *
 * File Name: types.h
 *
 * Copyright (C) 2014 HUINS,Inc.
 * Programmed by Kim suhak
 * email : shkim@huins.com
 * file creation : 2014/12/15
 *
 *****************************************************************************/

#ifndef _types_H_
#define _types_H_

#define TYPE_TRUE   1
#define TYPE_FALSE  0
#define ERROR_PARSE -1
#define ERROR_NULL  -2

enum APP_TYPE {FARM, SECRET, FIRE, SMARTHOME, TOY, PET};

#define USE_FARM
#define USE_INTRUDER
#define USE_FIRE
#define USE_SMARTHOME
#define USE_TOY
#define USE_PET

//messages 
#define GET_STATUS      0x101
#define GET_SETTING     0x102
#define SET_SETTING     0x103
#define GET_HISTORY     0x104
//0. farm
#ifdef USE_FARM
#define WATER_SUPPLY    0x110
#define WATER_FAN       0x111
#define WATER_MOTOR     0x112
#define WATER_LED       0x113
#endif 
//1.fire
#ifdef USE_FIRE
#define FIRE_PUMP            0x110
#define FIRE_BUZZER          0x111
#define FIRE_MOTOR           0x112
#define FIRE_LED             0x113
#define FIRE_SENSOR          0x114
#define FIRE_EMERGENCY       0x115
#endif 
//2.secret
#ifdef USE_INTRUDER
#define INTRUDER_BUZZER      0x110
#define INTRUDER_MOTOR       0x111
#define INTRUDER_LED         0x112
#define INTRUDER_LIGHT       0x113
#define INTRUDER_SENSOR      0x114
#define INTRUDER_EMERGENCY   0x115
#endif
//3.smarthome
#ifdef USE_SMARTHOME
#define SMARTHOME_LANTERN     0x110
#define SMARTHOME_DOOR        0x111
#define SMARTHOME_GAS         0x112
#define SMARTHOME_LIGHT       0x113
#define SMARTHOME_WATER       0x114
#endif
//4.toy
#ifdef USE_TOY
#define TOY_SERVOMOTOR     0x110
#define TOY_LEDBAR         0x111
#define TOY_DCMOTOR        0x112
#define TOY_RGBLED         0x113
#define TOY_UP             0x114
#define TOY_DOWN           0x115
#define TOY_LEFT           0x116
#define TOY_RIGHT          0x117
#define TOY_STOP           0x118
#endif
//5.pet
#ifdef USE_PET
#define PET_SERVOMOTOR     0x110
#define PET_LEDBAR         0x111
#define PET_DCMOTOR        0x112
#define PET_RGBLED         0x113
#define PET_SETTING_TIME   0x114
#endif

typedef struct _Data
{
    int _data1;
    int _data2;
    int _data3;
    int _data4;
    int _data5;
    int _data6;
}Data;

typedef struct _DataSTR
{
    char _data1[16];
    char _data2[16];
    char _data3[16];
    char _data4[16];
    char _data5[16];
    char _data6[16];
}DataSTR;


typedef struct _Message
{
    enum APP_TYPE _apptype;
    unsigned int _command;
    char      _data[128];
}Message;

#endif

