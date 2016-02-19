// Include library for BMP180 Barometric Sensor
#include <SFE_BMP180.h>

// Include libraries for GY-85 IMU Sensor (Accelerometer, Gyro, Compass)
#include <GY_85.h>
#include <Wire.h>

// Include library for driving elevon servos
#include <Servo.h>


// Creating objects
Servo myservo;  // Controlling servos
GY_85 GY85; // Reading IMU data

// Declare and initialize variables
// Time keepers
unsigned long fullcycle = 0;

// Servo
int val = 90;
int dir = 0;

// IMU
// 3-axis accelerometer values (absolute angle)
int ax = 0;
int ay = 0;
int az = 0;
// 3-axis gyroscope values (angular speed) + gyroscope temperature
float gx = 0.0f;
float gy = 0.0f;
float gz = 0.0f;
float gt = 0.0f;
//3-axis compass values (magnetic field)
int cx = 0;
int cy = 0;
int cz = 0;

void setup() {
  // Setup I²C communications
  Wire.begin();
  delay(10);
  
  // Setup serial connection
  Serial.begin(115200);
  delay(10);

  // Initialize GY85 IMU
  GY85.init();
  delay(10);

  // Attach driving pins to servo object(s)
  myservo.attach(9);
}

void loop() {
  // Start timer of full cycle
  fullcycle = millis();
  
  // Reading GY-85 IMU raw sensor values
  // 3-axis accelerometer data
  ax = GY85.accelerometer_x( GY85.readFromAccelerometer() );
  ay = GY85.accelerometer_y( GY85.readFromAccelerometer() );
  az = GY85.accelerometer_z( GY85.readFromAccelerometer() );

  // 3-axis gyroscope data
  gx = GY85.gyro_x( GY85.readGyro() );
  gy = GY85.gyro_y( GY85.readGyro() );
  gz = GY85.gyro_z( GY85.readGyro() );
  gt = GY85.temp  ( GY85.readGyro() );

  // 3-axis compass data ##################### IMPORTANT: Calibrate compass in plane and store biases before use! ############################
  cx = GY85.compass_x( GY85.readFromCompass() );
  cy = GY85.compass_y( GY85.readFromCompass() );
  cz = GY85.compass_z( GY85.readFromCompass() );

  // Calculate elapsed time during full cycle
  fullcycle = millis() - fullcycle;

  // Send cycle time over Serial
  Serial.println(fullcycle);
}

