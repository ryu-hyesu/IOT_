
#define TYPE_TRUE   1
#define TYPE_FALSE  0
#define ERROR_PARSE -1

enum APP_TYPE {FARM, SECRET, FIRE, SMARTHOME, TOY, PET};

// messages 
#define GET_STATUS      	0x101
#define GET_SETTING     	0x102
#define SET_SETTING     	0x103
#define GET_HISTORY     	0x104

#define WATER_SUPPLY    	0x110
#define WATER_FAN       	0x111
#define WATER_MOTOR     0x112
#define WATER_LED       	0x113

typedef struct _Message
{
    enum APP_TYPE _apptype;
    unsigned int _command;
    char      _data[128];
} Message;


typedef struct _Data
{
    int _data1;
    int _data2;
    int _data3;
    int _data4;
    int _data5;
    int _data6;
} Data;


