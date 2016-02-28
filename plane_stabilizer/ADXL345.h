// ADXL345.h

#ifndef _ADXL345_h
#define _ADXL345_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#endif

// ADXL345 registers
#define ADXL345_ADDRESS					(0x53)    // Assumes ALT address pin low

#define ADXL345_DEVID					(0x00)    // Device ID
#define ADXL345_THRESH_TAP				(0x1D)    // Tap threshold
#define ADXL345_OFSX					(0x1E)    // X-axis offset
#define ADXL345_OFSY					(0x1F)    // Y-axis offset
#define ADXL345_OFSZ					(0x20)    // Z-axis offset
#define ADXL345_DUR						(0x21)    // Tap duration
#define ADXL345_LATENT					(0x22)    // Tap latency
#define ADXL345_WINDOW					(0x23)    // Tap window
#define ADXL345_THRESH_ACT				(0x24)    // Activity threshold
#define ADXL345_THRESH_INACT			(0x25)    // Inactivity threshold
#define ADXL345_TIME_INACT				(0x26)    // Inactivity time
#define ADXL345_ACT_INACT_CTL			(0x27)    // Axis enable control for activity and inactivity detection
#define ADXL345_THRESH_FF				(0x28)    // Free-fall threshold
#define ADXL345_TIME_FF					(0x29)    // Free-fall time
#define ADXL345_TAP_AXES				(0x2A)    // Axis control for single/double tap
#define ADXL345_ACT_TAP_STATUS			(0x2B)    // Source for single/double tap
#define ADXL345_BW_RATE					(0x2C)    // Data rate and power mode control
#define ADXL345_POWER_CTL				(0x2D)    // Power-saving features control
#define ADXL345_INT_ENABLE				(0x2E)    // Interrupt enable control
#define ADXL345_INT_MAP					(0x2F)    // Interrupt mapping control
#define ADXL345_INT_SOURCE				(0x30)    // Source of interrupts
#define ADXL345_DATA_FORMAT				(0x31)    // Data format control
#define ADXL345_DATAX0					(0x32)    // X-axis data 0
#define ADXL345_DATAX1					(0x33)    // X-axis data 1
#define ADXL345_DATAY0					(0x34)    // Y-axis data 0
#define ADXL345_DATAY1					(0x35)    // Y-axis data 1
#define ADXL345_DATAZ0					(0x36)    // Z-axis data 0
#define ADXL345_DATAZ1					(0x37)    // Z-axis data 1
#define ADXL345_FIFO_CTL				(0x38)    // FIFO control
#define ADXL345_FIFO_STATUS				(0x39)    // FIFO status

#define ADXL345_MG2G_MULTIPLIER (0.004)  // 4mg per lsb


/* Used with register 0x2C (ADXL345_BW_RATE) to set bandwidth and ODR */
enum Arate {
	ARTBW_010_005 = 0, // 0.1 Hz ODR, 0.05Hz bandwidth
	ARTBW_020_010,
	ARTBW_039_020,
	ARTBW_078_039,
	ARTBW_156_078,
	ARTBW_313_156,
	ARTBW_125_625,
	ARTBW_25_125,
	ARTBW_50_25,
	ARTBW_100_50,
	ARTBW_200_100,
	ARTBW_400_200,
	ARTBW_800_400,
	ARTBW_1600_800,
	ARTBW_3200_1600  // 3200 Hz ODR, 1600 Hz bandwidth
};

/* Used with register 0x31 (ADXL345_DATA_FORMAT) to set g range */
enum Ascale {
	AFS_2G = 0,		// +/- 2g (default value)
	AFS_4G,			// +/- 4g
	AFS_8G,			// +/- 8g
	AFS_16G			// +/- 16g
};


void getAres();
void readAccelData(int16_t * destination);
void initADXL345();
void calADXL345();

