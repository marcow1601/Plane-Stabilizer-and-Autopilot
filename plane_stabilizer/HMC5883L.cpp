// 
// 
// 

#include "HMC5883L.h"

/* ###################################################
*  ####### Useful functions for magnetometer #########
*  ###################################################
*/

void initHMC5883L()
{
	// Set magnetomer ODR; default is 15 Hz 
	writeByte(HMC5883L_ADDRESS, HMC5883L_CONFIG_A, Mrate << 2);
	writeByte(HMC5883L_ADDRESS, HMC5883L_CONFIG_B, 0x00);  // set gain (bits[7:5]) to maximum resolution of 0.73 mG/LSB
	writeByte(HMC5883L_ADDRESS, HMC5883L_MODE, 0x80);     // enable continuous data mode
}

byte selfTestHMC5883L()
{
	int16_t selfTest[3] = { 0, 0, 0 };
	//  Perform self-test and calculate temperature compensation bias
	writeByte(HMC5883L_ADDRESS, HMC5883L_CONFIG_A, 0x71);   // set 8-average, 15 Hz default, positive self-test measurement
	writeByte(HMC5883L_ADDRESS, HMC5883L_CONFIG_B, 0xA0);   // set gain (bits[7:5]) to 5
	writeByte(HMC5883L_ADDRESS, HMC5883L_MODE, 0x80);      // enable continuous data mode
	delay(150); // wait 150 ms

	uint8_t rawData[6] = { 0, 0, 0, 0, 0, 0 };                        // x/y/z gyro register data stored here
	readBytes(HMC5883L_ADDRESS, HMC5883L_OUT_X_H, 6, &rawData[0]);  // Read the six raw data registers sequentially into data array
	selfTest[0] = ((int16_t)rawData[0] << 8) | rawData[1];          // Turn the MSB and LSB into a signed 16-bit value
	selfTest[1] = ((int16_t)rawData[4] << 8) | rawData[5];
	selfTest[2] = ((int16_t)rawData[2] << 8) | rawData[3];
	writeByte(HMC5883L_ADDRESS, HMC5883L_CONFIG_A, 0x00);   // exit self test

	if (selfTest[0] < 575 && selfTest[0] > 243 && selfTest[1] < 575 && selfTest[1] > 243 && selfTest[2] < 575 && selfTest[2] > 243)
	{
		return true;
	}
	else { return false; }
}

void readMagData(int16_t * destination)
{
	uint8_t rawData[6];  // x/y/z gyro register data stored here
	readBytes(HMC5883L_ADDRESS, HMC5883L_OUT_X_H, 6, &rawData[0]);  // Read the six raw data registers sequentially into data array
	destination[0] = ((int16_t)rawData[0] << 8) | rawData[1];       // Turn the MSB and LSB into a signed 16-bit value
	destination[1] = ((int16_t)rawData[4] << 8) | rawData[5];
	destination[2] = ((int16_t)rawData[2] << 8) | rawData[3];
}


