#include <Wire.h>

#include "elevon.h"
#include "IMU.h"
#include "BMP180.h"
#include "HMC5883L.h"
#include "ITG3200.h"
#include "ADXL345.h"



/* ##################################################
* ###### Declare and initialize variables ##########
* ##################################################
*/


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

		calADXL345(); // Calibrate ADXL345 accelerometers, load biases in bias registers  
		initADXL345(); // Initialize and configure accelerometer 
		Serial.println("ADXL345 initialized for active data mode....");

		initITG3200(); // Initialize and configure gyroscope
		Serial.println("ITG3200 initialized for active data mode....");

		if (selfTestHMC5883L()) {   // perform magnetometer self test
			Serial.print(" HMC5883L passed self test!");
			delay(1000);
		}
		else {
			Serial.print(" HMC5883L failed self test!");
			delay(1000);
		}

		initHMC5883L(); // Initialize and configure magnetometer
		Serial.println("HMC5883L initialized for active data mode....");
	}
	else
	{
		Serial.println("No Connection to GY-85");
		while (1); // Loop forever if communication doesn't happen
	}

}

/* ################################################## 
 * ###### Infinite execution loop routine ###########
 * ##################################################
 */
void loop() {
	// If intPin goes high or data ready status is TRUE, all data registers have new data
	if (readByte(ADXL345_ADDRESS, ADXL345_INT_SOURCE) & 0x80) {  // When data is ready  
		readAccelData(accelCount);  // Read the x/y/z adc values
		getAres();

		// Now we'll calculate the accleration value into actual g's
		ax = (float)accelCount[0] * aRes;  // get actual g value, this depends on scale being set
		ay = (float)accelCount[1] * aRes;
		az = (float)accelCount[2] * aRes;

	}
		
	if (readByte(ITG3200_ADDRESS, ITG3200_INT_STATUS) & 0x01) {
		readGyroData(gyroCount);  // Read the x/y/z adc values
		getGres();
		
		// Calculate the gyro value into actual degrees per second
		gx = (float)gyroCount[0] * gRes;  // get actual gyro value, this depends on scale being set
		gy = (float)gyroCount[1] * gRes;
		gz = (float)gyroCount[2] * gRes;
		
	}

	if (readByte(HMC5883L_ADDRESS, HMC5883L_STATUS) & 0x01) { // If data ready bit set, then read magnetometer data
		readMagData(magCount);  // Read the x/y/z adc values
		mRes = 0.73; // Conversion to milliGauss, 0.73 mG/LSB in hihgest resolution mode
					 // So far, magnetometer bias is calculated and subtracted here manually, should construct an algorithm to do it automatically
					 // like the gyro and accelerometer biases
		magbias[0] = -30.;  // User environmental x-axis correction in milliGauss
		magbias[1] = +85.;  // User environmental y-axis correction in milliGauss
		magbias[2] = -78.;  // User environmental z-axis correction in milliGauss

							// Calculate the magnetometer values in milliGauss
							// Include factory calibration per data sheet and user environmental corrections
		mx = (float)magCount[0] * mRes - magbias[0];  // get actual magnetometer value, this depends on scale being set
		my = (float)magCount[1] * mRes - magbias[1];
		mz = (float)magCount[2] * mRes - magbias[2];
	}

	Now = micros();
	deltat = ((Now - lastUpdate) / 1000000.0f); // set integration time by time elapsed since last filter update
	lastUpdate = Now;
	
	//MadgwickQuaternionUpdate(ax, ay, az, gx*PI / 180.0f, gy*PI / 180.0f, gz*PI / 180.0f, mx, my, mz);
	MahonyQuaternionUpdate(ax, ay, az, gx*PI/180.0f, gy*PI/180.0f, gz*PI/180.0f, mx, my, mz);

	// Define output variables from updated quaternion---these are Tait-Bryan angles, commonly used in aircraft orientation.
	// In this coordinate system, the positive z-axis is down toward Earth. 
	// Yaw is the angle between Sensor x-axis and Earth magnetic North (or true North if corrected for local declination, looking down on the sensor positive yaw is counterclockwise.
	// Pitch is angle between sensor x-axis and Earth ground plane, toward the Earth is positive, up toward the sky is negative.
	// Roll is angle between sensor y-axis and Earth ground plane, y-axis up is positive roll.
	// These arise from the definition of the homogeneous rotation matrix constructed from quaternions.
	// Tait-Bryan angles as well as Euler angles are non-commutative; that is, the get the correct orientation the rotations must be
	// applied in the correct order which for this configuration is yaw, pitch, and then roll.
	// For more see http://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles which has additional links.
	yaw = atan2(2.0f * (q[1] * q[2] + q[0] * q[3]), q[0] * q[0] + q[1] * q[1] - q[2] * q[2] - q[3] * q[3]);
	pitch = -asin(2.0f * (q[1] * q[3] - q[0] * q[2]));
	roll = atan2(2.0f * (q[0] * q[1] + q[2] * q[3]), q[0] * q[0] - q[1] * q[1] - q[2] * q[2] + q[3] * q[3]);
	pitch *= 180.0f / PI;
	yaw *= 180.0f / PI;
	yaw -= 2.308; // Declination at Stuttgart, Germany is 2 degrees and 18.48 minutes on 2016-03-03
	roll *= 180.0f / PI;

	Serial.print("Yaw, Pitch, Roll: ");
	Serial.print(yaw, 2);
	Serial.print(", ");
	Serial.print(pitch, 2);
	Serial.print(", ");
	Serial.println(roll, 2);

	Serial.print("average rate = ");
	Serial.println(1.0f / deltat, 2);
	Serial.println(" Hz");
}

