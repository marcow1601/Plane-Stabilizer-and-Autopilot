/* ##################################################
 * ###### Include external libraries ################
 * ###################################################
 */
#include <SFE_BMP180.h>
#include <Wire.h>
#include <Servo.h>


/* ################################################## 
 * ###### Include external headers ##################
 * ##################################################
 */
#include "sensors.h"	// General sensor functionality
#include "IMU.h"		// IMU data fusion
#include "ADXL345.h"	// Accelerometer functionality
#include "ITG3200.h"	// Gyroscope functionality
#include "HMC5883L.h"	// Magnetometer functionality

#include "elevon.h" // Elevon actuations


/* ################################################## 
 * ###### Declare and initialize variables ##########
 * ##################################################
 */
// Time keepers
unsigned long fullcycle = 0;

// IMU
// 3-axis accelerometer values (absolute angle)
int ax = 0;
int ay = 0;
int az = 0;
// 3-axis gyroscope values (angular velocity) + gyroscope temperature
float gx = 0.0f;
float gy = 0.0f;
float gz = 0.0f;
float gt = 0.0f;
//3-axis compass values (magnetic field)
int cx = 0;
int cy = 0;
int cz = 0;

// Barometer
double baseline; // baseline pressure


/* ################################################## 
 * ###### Single execution setup routine ############
 * ##################################################
 */
void setup() {
  // Setup I²C communications
  Wire.begin();
  delay(10);
  
  // Setup serial connection
  Serial.begin(115200);
  delay(10);

}

/* ################################################## 
 * ###### Infinite execution loop routine ###########
 * ##################################################
 */
void loop() {
  // Start timer of full cycle
  fullcycle = millis();

  
  // Calculate elapsed time during full cycle
  fullcycle = millis() - fullcycle;

  // Send cycle time over Serial
  Serial.println(fullcycle);
}

