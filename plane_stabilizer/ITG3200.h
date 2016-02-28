// ITG3200.h

#ifndef _ITG3200_h
#define _ITG3200_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#endif

// ITG3200 registers
#define ITG3200_ADDRESS					(0x68)

#define ITG3200_WHO_AM_I				(0x00)		// Device-ID (should be 0x68)
#define ITG3200_SMPLRT_DIV				(0x15)		// Gyro sample rate (8kHz or 1kHz) divider; 0-255; fsample = finternal / (divider+1)
#define ITG3200_DLPF_FS					(0x16)		// Digital Low Pass Filter configuration and Full scale (FS) selection
#define ITG3200_INT_CFG					(0x17)		// Interrupt configuration
#define ITG3200_INT_STATUS				(0x1A)		// Interrupt status
#define ITG3200_TEMP_OUT_H				(0x1B)		// Temperature output (high byte)
#define ITG3200_TEMP_OUT_L				(0x1C)		// Temperature output (low byte)
#define ITG3200_GYRO_XOUT_H				(0x1D)		// Gyro X-axis output (high byte)
#define ITG3200_GYRO_XOUT_L				(0x1E)		// Gyro X-axis output (low byte)
#define ITG3200_GYRO_YOUT_H				(0x1F)		// Gyro Y-axis output (high byte)
#define ITG3200_GYRO_YOUT_L				(0x20)		// Gyro Y-axis output (low byte)
#define ITG3200_GYRO_ZOUT_H				(0x21)		// Gyro Z-axis output (high byte)
#define ITG3200_GYRO_ZOUT_L				(0x22)		// Gyro Z-axis output (low byte)
#define ITG3200_PWR_MGM					(0x3E)		// Power management


/* Used with register 0x16 (ITG3200_DLPF_FS) to set Digital Low Pass Filter configuration as well as Internal Sample Rate (NOTE: Bit 3 and 4 "FS_SEL" must be set to 0x03 for proper operation)*/
enum
{
	ITG3200_DLPF_FS_256Hz_8kHz = 0b00011000,			// 256Hz LPF-Bandwidth	8kHz Internal Sample Rate
	ITG3200_DLPF_FS_188Hz_1kHz = 0b00011001,			// 188Hz LPF-Bandwidth	1kHz Internal Sample Rate
	ITG3200_DLPF_FS_98Hz_1kHz = 0b00011010,				// 98Hz LPF-Bandwidth	1kHz Internal Sample Rate
	ITG3200_DLPF_FS_42Hz_1kHz = 0b00011011,				// 42Hz / 1kHz
	ITG3200_DLPF_FS_20Hz_1kHz = 0b00011100,				// 20Hz / 1kHz
	ITG3200_DLPF_FS_10Hz_1kHz = 0b00011101,				// 10Hz / 1kHz
	ITG3200_DLPF_FS_5Hz_1kHz = 0b00011110				// 5Hz / 1kHz

};


void getGres();
void initITG3200();
void readGyroData(int16_t * destination);
void readTempData(int16_t destination);

