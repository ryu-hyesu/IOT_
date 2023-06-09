
/*****************************************************************************
 *
 * File Name: sensor.c
 *
 * Copyright (C) 2014 HUINS,Inc.
 * Programmed by Kim suhak
 * email : shkim@huins.com
 * file creation : 2014/12/18
 *
 *****************************************************************************/
#include "farm_sensor.h"
#include <wiringPi.h>

#define RETRY 5

int ret_humid, ret_temp;

static int DHTPIN = 7;
static int dht22_dat[5] = {0,0,0,0,0};

int get_temp_functionality()
{
	return 1;
}

int get_humid_functionality()
{
	return 1;
}

int get_light_functionality()
{
	return 1;
}

int get_pump_functionality()
{
	return 1;
}

int get_fan_functionality()
{
	return 1;
}

int get_dcmotor_functionality()
{
	return 1;
}

int get_rgbled_functionality()
{
	return 1;
}

int get_temperature_sensor()
{
	int received_temp;
	int _retry = RETRY;

	DHTPIN = 11;

	if (wiringPiSetup() == -1)
		exit(EXIT_FAILURE) ;
	
	if (setuid(getuid()) < 0)
	{
		perror("Dropping privileges failed\n");
		exit(EXIT_FAILURE);
	}
	while (read_dht22_dat_temp() == 0 && _retry--)
	{
		delay(500); // wait 1sec to refresh
	}
	received_temp = ret_temp ;
	printf("Temperature = %d\n", received_temp);
	
	return received_temp;
}

static uint8_t sizecvt(const int read)
{
  /* digitalRead() and friends from wiringpi are defined as returning a value
  < 256. However, they are returned as int() types. This is a safety function */

  if (read > 255 || read < 0)
  {
    printf("Invalid data from wiringPi library\n");
    exit(EXIT_FAILURE);
  }
  return (uint8_t)read;
}

int read_dht22_dat_temp()
{
  uint8_t laststate = HIGH;
  uint8_t counter = 0;
  uint8_t j = 0, i;

  dht22_dat[0] = dht22_dat[1] = dht22_dat[2] = dht22_dat[3] = dht22_dat[4] = 0;

  pinMode(DHTPIN, INPUT);
  pullUpDnControl (DHTPIN, PUD_UP);
  delay(1);
  pinMode(DHTPIN, OUTPUT);
  digitalWrite(DHTPIN, LOW);
  delayMicroseconds(1100);
  
  pinMode(DHTPIN, INPUT);

  // detect change and read data
  for ( i=0; i< MAXTIMINGS; i++) {
    counter = 0;
    while (sizecvt(digitalRead(DHTPIN)) == laststate) {
      counter++;
      delayMicroseconds(1);
      if (counter == 255) {
        break;
      }
    }
    laststate = sizecvt(digitalRead(DHTPIN));

    if (counter == 255) break;

    // ignore first 3 transitions
    if ((i >= 4) && (i%2 == 0)) {
      // shove each bit into the storage bytes
      dht22_dat[j/8] <<= 1;
      if (counter > 16)
        dht22_dat[j/8] |= 1;
      j++;
    }
  }

  // check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
  // print it out if data is good
  if ((j >= 40) && 
      (dht22_dat[4] == ((dht22_dat[0] + dht22_dat[1] + dht22_dat[2] + dht22_dat[3]) & 0xFF)) ) {
        float t, h;
		
        h = (float)dht22_dat[0] * 256 + (float)dht22_dat[1];
        h /= 10;
        t = (float)(dht22_dat[2] & 0x7F)* 256 + (float)dht22_dat[3];
        t /= 10.0;
        if ((dht22_dat[2] & 0x80) != 0)  t *= -1;
		
		ret_humid = (int)h;
		ret_temp = (int)t;
		printf("Humidity = %.2f %% Temperature = %.2f *C \n", h, t );
		printf("Humidity = %d Temperature = %d\n", ret_humid, ret_temp);
		
    return ret_temp;
  }
  else
  {
    printf("Data not good, skip\n");
    return 0;
  }
}


int get_humidity_sensor()
{
	int received_humid;
	int _retry = RETRY;

	DHTPIN = 11;

	if (wiringPiSetup() == -1)
		exit(EXIT_FAILURE) ;
	
	if (setuid(getuid()) < 0)
	{
		perror("Dropping privileges failed\n");
		exit(EXIT_FAILURE);
	}

	while (read_dht22_dat_humid() == 0 && _retry--) 
	{
		delay(500); // wait 1sec to refresh
	}

	received_humid = ret_humid;
	printf("Humidity = %d\n", received_humid);
	
	return received_humid;
}

int read_dht22_dat_humid()
{
	uint8_t laststate = HIGH;
	uint8_t counter = 0;
	uint8_t j = 0, i;

	dht22_dat[0] = dht22_dat[1] = dht22_dat[2] = dht22_dat[3] = dht22_dat[4] = 0;

	pinMode(DHTPIN, INPUT);
	pullUpDnControl (DHTPIN, PUD_UP);
	delay(1);

	pinMode(DHTPIN, OUTPUT);
	digitalWrite(DHTPIN, LOW);
	delayMicroseconds(1100);
	
	// prepare to read the pin
	pinMode(DHTPIN, INPUT);

	// detect change and read data
	for ( i=0; i< MAXTIMINGS; i++) 
	{
		counter = 0;
		while (sizecvt(digitalRead(DHTPIN)) == laststate) 
		{
			counter++;
			delayMicroseconds(1);
			if (counter == 255) {
			break;
		}
    }
    laststate = sizecvt(digitalRead(DHTPIN));

    if (counter == 255) break;

    // ignore first 3 transitions
    if ((i >= 4) && (i%2 == 0)) 
	{
		// shove each bit into the storage bytes
		dht22_dat[j/8] <<= 1;
		if (counter > 16)
			dht22_dat[j/8] |= 1;
		j++;
    }
}

  // check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
  // print it out if data is good
	if ((j >= 40) && 
      (dht22_dat[4] == ((dht22_dat[0] + dht22_dat[1] + dht22_dat[2] + dht22_dat[3]) & 0xFF)) ) {
        float t, h;
		
        h = (float)dht22_dat[0] * 256 + (float)dht22_dat[1];
        h /= 10;
        t = (float)(dht22_dat[2] & 0x7F)* 256 + (float)dht22_dat[3];
        t /= 10.0;
        if ((dht22_dat[2] & 0x80) != 0)  t *= -1;
		
		ret_humid = (int)h;
		ret_temp = (int)t;
		printf("Humidity = %.2f %% Temperature = %.2f *C \n", h, t );
		printf("Humidity = %d Temperature = %d\n", ret_humid, ret_temp);
		
    return ret_humid;
	}
	else
	{
		printf("Data not good, skip\n");
		return 0;
	}
}

int wiringPicheck(void)
{
	if (wiringPiSetup () == -1)
	{
		fprintf(stdout, "Unable to start wiringPi: %s\n", strerror(errno));
		return 1 ;
	}
}

int get_light_sensor(void)
{
	// sets up the wiringPi library
	if (wiringPiSetup () < 0) 
	{
		fprintf (stderr, "Unable to setup wiringPi: %s\n", strerror (errno));
		return 1;
	}
	
	if(digitalRead(LIGHTSEN_OUT))	//day
		return 1;
	else //night
		return 0;

}

/*****************************************************************/
/*****sub devices control ***/
/*****************************************************************/

int set_waterpump_active(const int _use)
{
	if(wiringPicheck()) 
		printf("%s Fail (param :%d) \n", _use);

	pinMode (PUMP, OUTPUT);
	digitalWrite (PUMP, _use) ;
	
}

int set_fan_active(const int _use)
{
	if(wiringPicheck()) 
		printf("%s Fail (param :%d) \n", _use);

	pinMode (FAN, OUTPUT);
	digitalWrite (FAN, _use) ; 
	
}

int set_dcmotor_active(const int _use)
{
	if(wiringPicheck()) 
		printf("%s Fail (param :%d) \n", _use);

	pinMode (DCMOTOR, OUTPUT);
	digitalWrite(DCMOTOR, _use); 
}

int set_rgbled_active(const int _use)
{
	if(wiringPicheck()) 
		printf("%s Fail (param :%d) \n", _use);

	pinMode(RGBLEDPOWER, OUTPUT);
	pinMode(RED, OUTPUT);
	pinMode(GREEN, OUTPUT);
	pinMode(BLUE,OUTPUT);
    
    printf("rgbled on/off \n");
	
	digitalWrite(RGBLEDPOWER, _use);
	digitalWrite(BLUE, _use); 
    digitalWrite(GREEN, _use);
}


