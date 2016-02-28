#include <Wire.h>

#include "elevon.h"
#include "IMU.h"
#include "sensors.h"
#include "BMP180.h"
#include "HMC5883L.h"
#include "ITG3200.h"
#include "ADXL345.h"

/* ##################################################
* ###### Declare and initialize variables ##########
* ##################################################
*/
/* ###################################################
* ############### Raw Sensor Output #################
* ###################################################
*/
int16_t accelCount[3];  // 16-bit signed accelerometer sensor output
int16_t gyroCount[3];   // 16-bit signed gyro sensor output
int16_t magCount[3];    // 16-bit signed magnetometer sensor output
float   magbias[3];     // User-specified magnetometer corrections values

float ax, ay, az, gx, gy, gz, mx, my, mz; // variables to hold latest sensor data values 
float q[4] = { 1.0f, 0.0f, 0.0f, 0.0f };    // vector to hold quaternion
float eInt[3] = { 0.0f, 0.0f, 0.0f };       // vector to hold integral error for Mahony method



											// IMU configuration parameters
											//uint8_t Gscale = 0x03; // ITG3200 in fullscale 2000°/s **fixed value for ITG3200
uint8_t Grate = ITG3200_DLPF_FS_42Hz_1kHz;  // 200 Hz ODR,  50 Hz bandwidth
uint8_t Ascale = AFS_2G; // ADXL345 in +/- 2g mode
uint8_t Arate = ARTBW_200_100; // 200 Hz ODR, 100 Hz bandwidth
uint8_t Mrate = MRT_75;        //  75 Hz ODR 
							   //uint8_t OSS = ;           // maximum pressure resolution
float aRes, gRes, mRes; // scale resolutions per LSB for the sensors


						// Latest orientational angles calculated
float pitch, yaw, roll;

// Time keepers
uint32_t lastUpdate = 0; // used to calculate integration interval
uint32_t Now = 0;        // used to calculate integration interval

float deltat = 0.0f;        // integration interval for both filter schemes


							// BMP180 object
SFE_BMP180 BMP180;

// Barometer
double baseline; // baseline pressure (at launch site)
float temperature, pressure;



/* ##################################################
*  ###### Pin definitions ##########
*  ##################################################
*/
int intPin = 12;  // These can be changed, 2 and 3 are the Arduinos ext int pins


/* ################################################## 
 * ###### Single execution setup routine ############
 * ##################################################
 */
void setup() {
	// Setup I²C communications
	Wire.begin();
	delay(10);
  
	// Setup serial connection
	Serial.begin(38400);
	delay(10);

	// Set up the interrupt pin, it's set as active high, push-pull
	pinMode(intPin, INPUT);
	digitalWrite(intPin, LOW);

	// Read the WHO_AM_I register of the ADXL345, this is a good test of communication
	uint8_t c = readByte(ADXL345_ADDRESS, ADXL345_DEVID);  // Read WHO_AM_I register for ADXL345
	Serial.println();
	Serial.println("ADXL345");
	Serial.println("I AM");
	Serial.println(c, HEX);
	Serial.println("I Should Be");
	Serial.println(0xE5, HEX);
	delay(1000);

	// Read the WHO_AM_I register of the ITG3200, this is a good test of communication
	uint8_t d = readByte(ITG3200_ADDRESS, ITG3200_WHO_AM_I);  // Read WHO_AM_I register for ITG3200
	Serial.println();
	Serial.println("ITG3200");
	Serial.println("I AM");
	Serial.println(d, HEX);
	Serial.println("I Should Be");
	Serial.println(0x68, HEX);
	delay(1000);

	// Read the WHO_AM_I register of the HMC5883L, this is a good test of communication
	uint8_t e = readByte(HMC5883L_ADDRESS, HMC5883L_IDA);  // Read WHO_AM_I register A for HMC5883L
	uint8_t f = readByte(HMC5883L_ADDRESS, HMC5883L_IDB);  // Read WHO_AM_I register B for HMC5883L
	uint8_t g = readByte(HMC5883L_ADDRESS, HMC5883L_IDC);  // Read WHO_AM_I register C for HMC5883L
	Serial.println();
	Serial.println("HMC5883L");
	Serial.println("I AM");
	Serial.println(e, HEX);
	Serial.println(f, HEX);
	Serial.println(g, HEX);
	Serial.println("I Should Be");
	Serial.println(0x48, HEX);
	Serial.println(0x34, HEX);
	Serial.println(0x33, HEX);
	delay(1000);

	if (c == 0xE5 && d == 0x68 && e == 0x48 && f == 0x34 && g == 0x33) // WHO_AM_I must match to proceed
	{
		Serial.println("ADXL345  is online...");
		Serial.println("ITG3200 is online...");
		Serial.println("HMC5883L is online...");
	}

}

/* ################################################## 
 * ###### Infinite execution loop routine ###########
 * ##################################################
 */
void loop() {
	
}

