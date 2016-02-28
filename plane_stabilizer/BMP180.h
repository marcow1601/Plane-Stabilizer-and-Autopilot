// BMP180.h


#ifndef _BMP180_h
#define _BMP180_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#endif

#include <SFE_BMP180.h>
#include "sensors.h"

SFE_BMP180 BMP180;

//uint8_t OSS = ;           // maximum pressure resolution

double baseline; // baseline pressure (at launch site)
float temperature, pressure;

double getPressure();

