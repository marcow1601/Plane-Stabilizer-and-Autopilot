#include <Wire.h>
#include <sensors.h>

/*=========================================================================
I2C ADDRESS/BITS
-----------------------------------------------------------------------*/
#define ADXL345_ADDRESS					(0x53)    // Assumes ALT address pin low
/*=========================================================================*/

/*=========================================================================
REGISTERS
-----------------------------------------------------------------------*/
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
/*=========================================================================*/

/*=========================================================================
REGISTERS
-----------------------------------------------------------------------*/
#define ADXL345_MG2G_MULTIPLIER (0.004)  // 4mg per lsb
/*=========================================================================*/

/* Used with register 0x2C (ADXL345_BW_RATE) to set bandwidth */
// Set accelerometer ODR and Bandwidth
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

/* Used with register 0x31 (ADXL345_REG_DATA_FORMAT) to set g range */
enum Ascale {
	AFS_2G = 0,		// +/- 2g (default value)
	AFS_4G,			// +/- 4g
	AFS_8G,			// +/- 8g
	AFS_16G			// +/- 16g
};


/* ###################################################
*  ##### Useful functions for accelerometer ##########
*  ###################################################
*/

void getAres() {
	switch (Ascale)
	{
		// Possible accelerometer scales (and their register bit settings) are:
		// 2 Gs (00), 4 Gs (01), 8 Gs (10), and 16 Gs  (11). 
	case AFS_2G:
		aRes = 2.0 / (512.*64.);   // 10-bit 2s-complement
		break;
	case AFS_4G:
		aRes = 4.0 / (1024.*32.);  // 11-bit 2s-complement
		break;
	case AFS_8G:
		aRes = 8.0 / (2048.*16.);  // 12-bit 2s-complement
		break;
	case AFS_16G:
		aRes = 16.0 / (4096.*8.);  // 13-bit 2s-complement
		break;
	}
}

void readAccelData(int16_t * destination)
{
	uint8_t rawData[6];  // x/y/z accel register data stored here
	readBytes(ADXL345_ADDRESS, ADXL345_DATAX0, 6, &rawData[0]);  // Read the six raw data registers into data array
	destination[0] = ((int16_t)rawData[1] << 8) | rawData[0];  // Turn the MSB and LSB into a signed 16-bit value
	destination[1] = ((int16_t)rawData[3] << 8) | rawData[2];
	destination[2] = ((int16_t)rawData[5] << 8) | rawData[4];
}

void initADXL345()
{
	// wake up device
	writeByte(ADXL345_ADDRESS, ADXL345_POWER_CTL, 0x00); // Put device in standby mode and clear sleep bit 2
	delay(100);  // Let device settle down
	writeByte(ADXL345_ADDRESS, ADXL345_POWER_CTL, 0x08); // Put device in normal mode

														 // Set accelerometer configuration; interrupt active high, left justify MSB
	writeByte(ADXL345_ADDRESS, ADXL345_DATA_FORMAT, 0x04 | Ascale); // Set full scale range for the accelerometer 

																	// Choose ODR and bandwidth
	writeByte(ADXL345_ADDRESS, ADXL345_BW_RATE, Arate); // Select normal power operation, and ODR and bandwidth

	writeByte(ADXL345_ADDRESS, ADXL345_INT_ENABLE, 0x80);  // Enable data ready interrupt
	writeByte(ADXL345_ADDRESS, ADXL345_INT_MAP, 0x00);     // Enable data ready interrupt on INT_1

	writeByte(ADXL345_ADDRESS, ADXL345_FIFO_CTL, 0x00);    // Bypass FIFO
}

void calADXL345()
{
	uint8_t data[6] = { 0, 0, 0, 0, 0, 0 };
	int abias[3] = { 0, 0, 0 };
	int16_t accel_bias[3] = { 0, 0, 0 };
	int samples, ii;

	// wake up device
	writeByte(ADXL345_ADDRESS, ADXL345_POWER_CTL, 0x00); // Put device in standby mode and clear sleep bit 2
	delay(10);  // Let device settle down
	writeByte(ADXL345_ADDRESS, ADXL345_POWER_CTL, 0x08); // Put device in normal mode
	delay(10);

	// Set accelerometer configuration; interrupt active high, left justify MSB
	writeByte(ADXL345_ADDRESS, ADXL345_DATA_FORMAT, 0x04 | 0x00); // Set full scale range to 2g for the bias calculation 
	uint16_t  accelsensitivity = 256;  // = 256 LSB/g at 2g full scale

									   // Choose ODR and bandwidth
	writeByte(ADXL345_ADDRESS, ADXL345_BW_RATE, 0x09); // Select normal power operation, and 100 Hz ODR and 50 Hz bandwidth
	delay(10);

	writeByte(ADXL345_ADDRESS, ADXL345_FIFO_CTL, 0x40 | 0x2F);    // Enable FIFO stream mode | collect 32 FIFO samples
	delay(1000);  // delay 1000 milliseconds to collect FIFO samples

	samples = readByte(ADXL345_ADDRESS, ADXL345_FIFO_STATUS);
	for (ii = 0; ii < samples; ii++) {
		readBytes(ADXL345_ADDRESS, ADXL345_DATAX0, 6, &data[0]);
		accel_bias[0] += (((int16_t)data[1] << 8) | data[0]) >> 6;
		accel_bias[1] += (((int16_t)data[3] << 8) | data[2]) >> 6;
		accel_bias[2] += (((int16_t)data[5] << 8) | data[4]) >> 6;
	}

	accel_bias[0] /= samples; // average the data
	accel_bias[1] /= samples;
	accel_bias[2] /= samples;

	// Remove gravity from z-axis accelerometer bias value
	if (accel_bias[2] > 0) {
		accel_bias[2] -= accelsensitivity;
	}
	else {
		accel_bias[2] += accelsensitivity;
	}

	abias[0] = (int)((-accel_bias[0] / 4) & 0xFF); // offset register are 8 bit 2s-complement, so have sensitivity 1/4 of 2g full scale
	abias[1] = (int)((-accel_bias[1] / 4) & 0xFF);
	abias[2] = (int)((-accel_bias[2] / 4) & 0xFF);

	writeByte(ADXL345_ADDRESS, ADXL345_OFSX, abias[0]);
	writeByte(ADXL345_ADDRESS, ADXL345_OFSY, abias[1]);
	writeByte(ADXL345_ADDRESS, ADXL345_OFSZ, abias[2]);
}

