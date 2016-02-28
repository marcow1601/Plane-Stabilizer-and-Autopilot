// 
// 
// 

#include "ITG3200.h"

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


