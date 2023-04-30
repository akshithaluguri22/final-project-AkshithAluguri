/* Author      : Ruthvik R Chanda
 * Description : Humidity Sensor Application Code with Raspberry Pi
 * References  : https://www.uugear.com/portfolio/read-dht1122-temperature-humidity-sensor-from-raspberry-pi
 */

/*Headers*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "wiringPi.h"

/*Macro Definitions*/ 
#define HUMIDITY_PIN 11                 	  
#define HIGH 1
#define LOW 0

char Concatenate[100];
char Buff_Python[150]; 
int Buff_Humidity[1] = { 0 };
int Buff_Temp[1] = { 0 };

/*Function Declarations*/
void Humidity_Sensor(void);

/*Main Function*/
int main(int argc, char* argv[])
{   
	printf( "Humidity Sensor Data\n" );
	wiringPiSetup();

	while (true)
	{
		Humidity_Sensor();
		delayMicroseconds(15);
	}
	return(0);
}
 
/*Humidity Function*/
void Humidity_Sensor()
{
        uint8_t i;
        uint8_t j = 0;
	uint8_t State = HIGH;
	uint8_t Count = 0;

	pinMode(HUMIDITY_PIN,OUTPUT);
	digitalWrite( HUMIDITY_PIN, LOW );
	delayMicroseconds(20);
        digitalWrite( HUMIDITY_PIN, HIGH );
        delayMicroseconds(40);
	pinMode(HUMIDITY_PIN,INPUT);

	Buff_Humidity[0]  = 0;
    
	for ( i = 0; i < 50; i++ )
	{
		Count = 0;
		
		while ( digitalRead(HUMIDITY_PIN) == State )
		{
			Count++;

			delayMicroseconds(1);

			if ( Count == 255 )
			{
				break;
			}
		}
		
		State = digitalRead(HUMIDITY_PIN);
 
		if ( Count == 255 )
		{
			break;
		}

		if ( (i >= 4) && (i % 2 == 0) )
		{
			Buff_Humidity[j / 8] <<= 1;
			if ( Count > 50 )
			{
				Buff_Humidity[j / 8] |= 1;
			}
			j++;
		}
	}

	Buff_Temp[0]=Buff_Humidity[0];

	printf("Humidity = %.1d \n", Buff_Humidity[0]);


        snprintf(Buff_Python,sizeof(Buff_Python),"python3 /bin/mqtt-client.py Humiditypercentage:%.1d", Buff_Humidity[0]);
        int update = system(Buff_Python);
    
        if(update)
        {
 	    printf(" ERROR %d", update);
        }  

}
