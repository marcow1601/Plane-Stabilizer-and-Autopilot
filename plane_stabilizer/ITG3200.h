#include <Wire.h>
#include "sensors.h"

/*=========================================================================
I2C ADDRESS/BITS
-----------------------------------------------------------------------*/
#define ITG3200_ADDRESS                 (0x68)
/*=========================================================================*/

/*=========================================================================
REGISTERS
-----------------------------------------------------------------------*/
#define ITG3200_WHO_AM_I               (0x00)		// Device-ID (should be 0x68)
#define ITG3200_SMPLRT_DIV			   (0x15)		// Gyro sample rate (8kHz or 1kHz) divider; 0-255; fsample = finternal / (divider+1)
#define ITG3200_DLPF_FS				   (0x16)		// Digital Low Pass Filter configuration and Full scale (FS) selection
#define ITG3200_INT_CFG				   (0x17)		// Interrupt configuration
#define ITG3200_INT_STATUS			   (0x1A)		// Interrupt status
#define ITG3200_TEMP_OUT_H			   (0x1B)		// Temperature output (high byte)
#define ITG3200_TEMP_OUT_L			   (0x1C)		// Temperature output (low byte)
#define ITG3200_GYRO_XOUT_H			   (0x1D)		// Gyro X-axis output (high byte)
#define ITG3200_GYRO_XOUT_L			   (0x1E)		// Gyro X-axis output (low byte)
#define ITG3200_GYRO_YOUT_H			   (0x1F)		// Gyro Y-axis output (high byte)
#define ITG3200_GYRO_YOUT_L			   (0x20)		// Gyro Y-axis output (low byte)
#define ITG3200_GYRO_ZOUT_H			   (0x21)		// Gyro Z-axis output (high byte)
#define ITG3200_GYRO_ZOUT_L			   (0x22)		// Gyro Z-axis output (low byte)
#define ITG3200_PWR_MGM				   (0x3E)		// Power management
/*=========================================================================*/

/* Used with register 0x16 (ITG3200_DLPF_FS) to set Digital Low Pass Filter configuration as well as Internal Sample Rate (NOTE: Bit 3 and 4 "FS_SEL" must be set to 0x03 for proper operation)*/
typedef enum
{
	ITG3200_DLPF_FS_256Hz_8kHz	=	0b00011000,			// 256Hz LPF-Bandwidth	8kHz Internal Sample Rate
	ITG3200_DLPF_FS_188Hz_1kHz	=	0b00011001,			// 188Hz LPF-Bandwidth	1kHz Internal Sample Rate
	ITG3200_DLPF_FS_98Hz_1kHz	=	0b00011010,			// 98Hz LPF-Bandwidth	1kHz Internal Sample Rate
	ITG3200_DLPF_FS_42Hz_1kHz	=	0b00011011,			// 42Hz / 1kHz
	ITG3200_DLPF_FS_20Hz_1kHz	=	0b00011100,			// 20Hz / 1kHz
	ITG3200_DLPF_FS_10Hz_1kHz	=	0b00011101,			// 10Hz / 1kHz
	ITG3200_DLPF_FS_5Hz_1kHz	=	0b00011110			// 5Hz / 1kHz

} DLPF_CFG_t;

/* ###################################################
*  ######### Useful functions for gyroscope ##########
*  ###################################################
*/

void getGres() {
	// For ITG3200: only 2000 DPS full scale mode possible
	gRes = 2000.0 / 32768.0;
}

void initITG3200()
{
	writeByte(ITG3200_ADDRESS, ITG3200_DLPF_FS, Grate);		// Set Digital Low Pass Filter Bandwidth and Internal Sample Rate, Gyro-Range: Full Scale +/- 2000 deg/sec
	writeByte(ITG3200_ADDRESS, ITG3200_SMPLRT_DIV, 0x04);						// Set Sample Rate Divider to 4 to get 200Hz ODR
	/*Interrupt Configuration:
		Bit 7 "ACTL"-INT logic level: 1 = active low; 0 = active high
		Bit 6 "OPEN"-INT drive type: 1 = open drain; 0 = push-pull
		Bit 5 "LATCH_INT_EN"-latch mode: 1 = latch until interrupt is cleared; 0 = 50us pulse
		Bit 4 "INT_ANYRD_2CLEAR"-latch clear mode: 1 = any register read; 0 = status register read only
		Bit 3 0
		Bit 2 "ITG_RDY_EN"-INT when device ready
		Bit 1 0
		Bit 0 "RAW_RDY_EN"-Int when data available
	*/
	writeByte(ITG3200_ADDRESS, ITG3200_INT_CFG, 0b00100001); // Active High; Push-pull; Latch until INT cleared; cleared by status register read; enable data ready interrupt
	writeByte(ITG3200_ADDRESS, ITG3200_PWR_MGM, 0x01); // No reset, no sleep, all axes active, Clock source: PLL with X Gyro reference

}

void readGyroData(int16_t * destination)
{
	uint8_t rawData[6];  // x/y/z gyro register data stored here
	readBytes(ITG3200_ADDRESS, ITG3200_GYRO_XOUT_H, 6, &rawData[0]);  // Read the six raw data registers sequentially into data array
	destination[0] = ((int16_t)rawData[0] << 8) | rawData[1];  // Turn the MSB and LSB into a signed 16-bit value
	destination[1] = ((int16_t)rawData[2] << 8) | rawData[3];
	destination[2] = ((int16_t)rawData[4] << 8) | rawData[5];
}

void readTempData(int16_t destination)
{
	uint8_t rawData[2];
	readBytes(ITG3200_ADDRESS, ITG3200_TEMP_OUT_H, 2, &rawData[0]);  // Read the two raw data register 
	destination = ((int16_t)rawData[0] << 8) | rawData[1]; 
}
