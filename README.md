# Plane-Stabilizer-and-Autopilot

## General desciption

This is an automatic stabilizing and auto-piloting system designed to be run in my [FPV49](http://fpv.no/vbulletin/showthread.php/2390-FPV49-Scratch-built-FPV-KFm-Airfoil#2) RC flying wing.

It is written for the Arduino Uno/Nano R3 platform (Atmel 328p) and uses quite a few different sensoric units detailled in the next paragraph.


## Sensors currently in use


* InvenSense **ITG3200** 3-axis gyroscope to measure _angular speed_  
* Analog Devices **ADXL345** 3-axis accelerometer to measure _absolute rotation_  
* Honeywell **HMC5883L** 3-axis magnetometer to measure the _orientation of the earth's magnetic field_  
* Bosch Sensortec **BMP180** barometric pressure sensor to measure _relative height_

Soon to be followed by:  
u-blox GPS module


## Software functions currently implemented

TBA
