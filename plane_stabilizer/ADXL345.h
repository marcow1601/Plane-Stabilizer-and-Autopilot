#include <Wire.h>

/*=========================================================================
I2C ADDRESS/BITS
-----------------------------------------------------------------------*/
#define ADXL345_ADDRESS                 (0x53)    // Assumes ALT address pin low
/*=========================================================================*/

/*=========================================================================
REGISTERS
-----------------------------------------------------------------------*/
#define ADXL345_REG_DEVID               (0x00)    // Device ID
#define ADXL345_REG_THRESH_TAP          (0x1D)    // Tap threshold
#define ADXL345_REG_OFSX                (0x1E)    // X-axis offset
#define ADXL345_REG_OFSY                (0x1F)    // Y-axis offset
#define ADXL345_REG_OFSZ                (0x20)    // Z-axis offset
#define ADXL345_REG_DUR                 (0x21)    // Tap duration
#define ADXL345_REG_LATENT              (0x22)    // Tap latency
#define ADXL345_REG_WINDOW              (0x23)    // Tap window
#define ADXL345_REG_THRESH_ACT          (0x24)    // Activity threshold
#define ADXL345_REG_THRESH_INACT        (0x25)    // Inactivity threshold
#define ADXL345_REG_TIME_INACT          (0x26)    // Inactivity time
#define ADXL345_REG_ACT_INACT_CTL       (0x27)    // Axis enable control for activity and inactivity detection
#define ADXL345_REG_THRESH_FF           (0x28)    // Free-fall threshold
#define ADXL345_REG_TIME_FF             (0x29)    // Free-fall time
#define ADXL345_REG_TAP_AXES            (0x2A)    // Axis control for single/double tap
#define ADXL345_REG_ACT_TAP_STATUS      (0x2B)    // Source for single/double tap
#define ADXL345_REG_BW_RATE             (0x2C)    // Data rate and power mode control
#define ADXL345_REG_POWER_CTL           (0x2D)    // Power-saving features control
#define ADXL345_REG_INT_ENABLE          (0x2E)    // Interrupt enable control
#define ADXL345_REG_INT_MAP             (0x2F)    // Interrupt mapping control
#define ADXL345_REG_INT_SOURCE          (0x30)    // Source of interrupts
#define ADXL345_REG_DATA_FORMAT         (0x31)    // Data format control
#define ADXL345_REG_DATAX0              (0x32)    // X-axis data 0
#define ADXL345_REG_DATAX1              (0x33)    // X-axis data 1
#define ADXL345_REG_DATAY0              (0x34)    // Y-axis data 0
#define ADXL345_REG_DATAY1              (0x35)    // Y-axis data 1
#define ADXL345_REG_DATAZ0              (0x36)    // Z-axis data 0
#define ADXL345_REG_DATAZ1              (0x37)    // Z-axis data 1
#define ADXL345_REG_FIFO_CTL            (0x38)    // FIFO control
#define ADXL345_REG_FIFO_STATUS         (0x39)    // FIFO status
/*=========================================================================*/

/*=========================================================================
REGISTERS
-----------------------------------------------------------------------*/
#define ADXL345_MG2G_MULTIPLIER (0.004)  // 4mg per lsb
/*=========================================================================*/

/* Used with register 0x2C (ADXL345_REG_BW_RATE) to set bandwidth */
typedef enum
{
	ADXL345_DATARATE_3200_HZ = 0b1111, // 1600Hz Bandwidth   140µA IDD
	ADXL345_DATARATE_1600_HZ = 0b1110, //  800Hz Bandwidth    90µA IDD
	ADXL345_DATARATE_800_HZ = 0b1101, //  400Hz Bandwidth   140µA IDD
	ADXL345_DATARATE_400_HZ = 0b1100, //  200Hz Bandwidth   140µA IDD
	ADXL345_DATARATE_200_HZ = 0b1011, //  100Hz Bandwidth   140µA IDD
	ADXL345_DATARATE_100_HZ = 0b1010, //   50Hz Bandwidth   140µA IDD
	ADXL345_DATARATE_50_HZ = 0b1001, //   25Hz Bandwidth    90µA IDD
	ADXL345_DATARATE_25_HZ = 0b1000, // 12.5Hz Bandwidth    60µA IDD
	ADXL345_DATARATE_12_5_HZ = 0b0111, // 6.25Hz Bandwidth    50µA IDD
	ADXL345_DATARATE_6_25HZ = 0b0110, // 3.13Hz Bandwidth    45µA IDD
	ADXL345_DATARATE_3_13_HZ = 0b0101, // 1.56Hz Bandwidth    40µA IDD
	ADXL345_DATARATE_1_56_HZ = 0b0100, // 0.78Hz Bandwidth    34µA IDD
	ADXL345_DATARATE_0_78_HZ = 0b0011, // 0.39Hz Bandwidth    23µA IDD
	ADXL345_DATARATE_0_39_HZ = 0b0010, // 0.20Hz Bandwidth    23µA IDD
	ADXL345_DATARATE_0_20_HZ = 0b0001, // 0.10Hz Bandwidth    23µA IDD
	ADXL345_DATARATE_0_10_HZ = 0b0000  // 0.05Hz Bandwidth    23µA IDD (default value)
} dataRate_t;

/* Used with register 0x31 (ADXL345_REG_DATA_FORMAT) to set g range */
typedef enum
{
	ADXL345_RANGE_16_G = 0b11,   // +/- 16g
	ADXL345_RANGE_8_G = 0b10,   // +/- 8g
	ADXL345_RANGE_4_G = 0b01,   // +/- 4g
	ADXL345_RANGE_2_G = 0b00    // +/- 2g (default value)
} range_t;



/**************************************************************************/
/*!
Writes 8-bits to the specified destination register
*/
/**************************************************************************/
void ADXL345_writeRegister(uint8_t reg, uint8_t value) {
	Wire.beginTransmission(ADXL345_ADDRESS);
	i2cwrite((uint8_t)reg);
	i2cwrite((uint8_t)(value));
	Wire.endTransmission();
}

/**************************************************************************/
/*!
Reads 8-bits from the specified register
*/
/**************************************************************************/
uint8_t ADXL345_readRegister(uint8_t reg) {
		Wire.beginTransmission(ADXL345_ADDRESS);
		i2cwrite(reg);
		Wire.endTransmission();
		Wire.requestFrom(ADXL345_ADDRESS, 1);
		return (i2cread());
}

/**************************************************************************/
/*!
Setups the HW
*/
/**************************************************************************/
bool ADXL345_begin() {

	Wire.begin();

	// Enable measurements
	ADXL345_writeRegister(ADXL345_REG_POWER_CTL, 0x08);

	return true;
}

/**************************************************************************/
/*!
Sets the g range for the accelerometer
*/
/**************************************************************************/
void ADXL345_setRange(range_t range)
{
	/* Red the data format register to preserve bits */
	uint8_t format = ADXL345_readRegister(ADXL345_REG_DATA_FORMAT);

	/* Update the data rate */
	format &= ~0x0F;
	format |= range;

	/* Make sure that the FULL-RES bit is enabled for range scaling */
	format |= 0x08;

	/* Write the register back to the IC */
	ADXL345_writeRegister(ADXL345_REG_DATA_FORMAT, format);

	/* Keep track of the current range (to avoid readbacks) */
	_range = range;
}

/**************************************************************************/
/*!
Gets the g range of the accelerometer
*/
/**************************************************************************/
range_t ADXL345_getRange(void)
{
	/* Red the data format register to preserve bits */
	return (range_t)(ADXL345_readRegister(ADXL345_REG_DATA_FORMAT) & 0x03);
}

/**************************************************************************/
/*!
Sets the data rate for the ADXL345 (controls power consumption)
*/
/**************************************************************************/
void ADXL345_setDataRate(dataRate_t dataRate)
{
	/* Note: The LOW_POWER bits are currently ignored and we always keep
	the device in 'normal' mode */
	ADXL345_writeRegister(ADXL345_REG_BW_RATE, dataRate);
}

/**************************************************************************/
/*!
Gets the data rate of the ADXL345
*/
/**************************************************************************/
dataRate_t ADXL345_getDataRate(void)
{
	return (dataRate_t)(ADXL345_readRegister(ADXL345_REG_BW_RATE) & 0x0F);
}



