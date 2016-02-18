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
  Serial.begin(9600);
  delay(10)

  // Initialize GY85 IMU
  GY85.init();
  delay(10);

  // Attach driving pins to servo object(s)
  myservo.attach(9);
}

void loop() {
  
  /*Serial.println(val);
  myservo.write(val);                  // sets the servo position according to the scaled value

  if(dir==0) {
    if(val >= 170){
      dir = 1;
    }
    
    else{
      val += 3;
    }
  }
  else {
    if(val <= 20){
      dir = 0;
    }
    else{
      val -= 3;
    }
  }
  
  delay(15);*/

  

  
}

