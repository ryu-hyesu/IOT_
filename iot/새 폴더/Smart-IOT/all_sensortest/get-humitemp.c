/*
 *      dht22.c:
 *	Simple test program to test the wiringPi functions
 *	Based on the existing dht11.c
 *	Amended by technion@lolware.net
 */

#include <wiringPi.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>

#define MAXTIMINGS 85

// Use GPIO Pin 11, which is BCM Pin 7

static int DHTPIN = 11;
static int dht22_dat[5] = {0,0,0,0,0};

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

static int read_dht22_dat()
{
  uint8_t laststate = HIGH;
  uint8_t counter = 0;
  uint8_t j = 0, i;

  dht22_dat[0] = dht22_dat[1] = dht22_dat[2] = dht22_dat[3] = dht22_dat[4] = 0;

  // Set internal gpio input to pull-up
  // Go into high impedence state to let pull-up raise data line
  pinMode(DHTPIN, INPUT);
  pullUpDnControl (DHTPIN, PUD_UP);
  delay(1);
  
  // DHT22, DHT21: pull pin down for 1 milliseconds at least (for safe, 1.1ms)
  // DHT11, Default: pull pin down for 18 milliseconds at least (for safe, 20ms)
  // http://www.adafruit.com/datasheets/Digital%20humidity%20and%20temperature%20sensor%20AM2302.pdf
  pinMode(DHTPIN, OUTPUT);
  digitalWrite(DHTPIN, LOW);
  delayMicroseconds(1100);

  // prepare to read the pin
  pinMode(DHTPIN, INPUT);

  // detect change and read data
  // Now read the 40 bits sent by the sensor.  Each bit is sent as a 50
  // microsecond low pulse followed by a variable length high pulse.  If the
  // high pulse is ~28 microseconds then it's a 0 and if it's ~70 microseconds
  // then it's a 1.  We measure the cycle count of the initial 50us low pulse
  // and use that to compare to the cycle count of the high pulse to determine
  // if the bit is a 0 (high state cycle count < low state cycle count), or a
  // 1 (high state cycle count > low state cycle count).
  
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
	  // Change counter > 16 into counter > 60 by Ryan
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
		// DHT11, DHT12: data[0] + data[1] * 0.1
		// DHT22, DHT21: (data[0] << 8 | data[1]) * 0.1
        h = (float)((dht22_dat[0] << 8) + dht22_dat[1])/10;

		// DHT11 : 
		// t = data[2];
        //     if (data[3] & 0x80) {
        //         t = -1 - f;
        //     }
        //     t += (data[3] & 0x0f) * 0.1;
		
		// DHT12: 
		// t = data[2];
        //    t += (data[3] & 0x0f) * 0.1;
        //    if (data[2] & 0x80) {
        //        t *= -1;
        //    }
		
		// DHT22, DHT21:
		// t = ((word)(data[2] & 0x7F)) << 8 | data[3];
        // t *= 0.1;
        // if (data[2] & 0x80) {
        //    t *= -1;
        // }

        t = (float)(((dht22_dat[2] & 0x7F) << 8) + dht22_dat[3])/10;
        if (dht22_dat[2] & 0x80) t = -t;

        printf("Humidity = %.2f %% Temperature = %.2f *C \n", h, t );
        return 1;
  }
  else
  {
    printf("Data not good, skip\n");
    return 0;
  }
}

//int main (int argc, char *argv[])
int main (void)
{
  
  if (wiringPiSetup() == -1)
    exit(EXIT_FAILURE) ;
	
  if (setuid(getuid()) < 0)
  {
    perror("Dropping privileges failed\n");
    exit(EXIT_FAILURE);
  }

  while (read_dht22_dat() == 0) 
  {
//     delay(2000); // wait 1sec to refresh
     sleep(2);
  }

  return 0 ;
}
