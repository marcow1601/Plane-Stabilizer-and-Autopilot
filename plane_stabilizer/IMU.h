// IMU.h

#ifndef _IMU_h
#define _IMU_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#endif

// Fusion and AHRS constants
#define GyroMeasError PI * (40.0f / 180.0f)       // gyroscope measurement error in rads/s (shown as 40 deg/s)
#define GyroMeasDrift PI * (0.0f / 180.0f)      // gyroscope measurement drift in rad/s/s (shown as 0.0 deg/s/s)
// There is a tradeoff in the beta parameter between accuracy and response speed.
// In the original Madgwick study, beta of 0.041 (corresponding to GyroMeasError of 2.7 degrees/s) was found to give optimal accuracy.
// However, with this value, the LSM9SD0 response time is about 10 seconds to a stable initial quaternion.
// Subsequent changes also require a longish lag time to a stable output, not fast enough for a quadcopter or robot car!
// By increasing beta (GyroMeasError) by about a factor of fifteen, the response time constant is reduced to ~2 sec
// I haven't noticed any reduction in solution accuracy. This is essentially the I coefficient in a PID control sense; 
// the bigger the feedback coefficient, the faster the solution converges, usually at the expense of accuracy. 
// In any case, this is the free parameter in the Madgwick filtering and fusion scheme.
#define beta sqrt(3.0f / 4.0f) * GyroMeasError   // compute beta
#define zeta sqrt(3.0f / 4.0f) * GyroMeasDrift   // compute zeta, the other free parameter in the Madgwick scheme usually set to a small or zero value
#define Kp 2.0f * 5.0f // these are the free parameters in the Mahony filter and fusion scheme, Kp for proportional feedback, Ki for integral
#define Ki 0.0f

float ax, ay, az, gx, gy, gz, mx, my, mz; // variables to hold latest sensor data values 
float q[4] = { 1.0f, 0.0f, 0.0f, 0.0f };    // vector to hold quaternion
float eInt[3] = { 0.0f, 0.0f, 0.0f };       // vector to hold integral error for Mahony method

// Latest orientational angles calculated
float pitch, yaw, roll;

uint32_t lastUpdate = 0; // used to calculate integration interval
uint32_t Now = 0;        // used to calculate integration interval

float deltat = 0.0f;        // integration interval for both filter schemes

/* ###################################################
* ############### Raw Sensor Output #################
* ###################################################
*/
int16_t accelCount[3];  // 16-bit signed accelerometer sensor output
int16_t gyroCount[3];   // 16-bit signed gyro sensor output
int16_t magCount[3];    // 16-bit signed magnetometer sensor output
float   magbias[3];     // User-specified magnetometer corrections values


void MadgwickQuaternionUpdate(float ax, float ay, float az, float gx, float gy, float gz, float mx, float my, float mz);
void MahonyQuaternionUpdate(float ax, float ay, float az, float gx, float gy, float gz, float mx, float my, float mz);

