 // MPU-9150 Accelerometer + Gyro + Compass + Temperature
// -----------------------------
//
// By arduino.cc user "frtrobotik" (Tobias HÃ¼bner)
//
//
// July 2013
//      first version
//
#include "imu.h"

//Variables where our values can be stored
int cmps[3];
int accl[3];
int gyro[3];
int temp;

void setup()
{      
  // Initialize the Serial Bus for printing data.
  Serial.begin(9600);

  // Initialize the 'Wire' class for the I2C-bus.
  MPU9150_setup();

  // Clear the 'sleep' bit to start the sensor.
  MPU9150_writeSensor(MPU9150_PWR_MGMT_1, 0);

  MPU9150_setupCompass();
}


void loop()
{
  // Print all sensor values which the sensor provides
  // Formated all values as x, y, and z in order for
  // Compass, Gyro, Acceleration. The First value is
  // the temperature.

  double dT = ( (double) MPU9150_readSensor(MPU9150_TEMP_OUT_L,MPU9150_TEMP_OUT_H) + 12412.0) / 340.0;
  Serial.print(dT);
  Serial.print("  ");
  Serial.print(MPU9150_readSensor(MPU9150_CMPS_XOUT_L,MPU9150_CMPS_XOUT_H));
  Serial.print("  ");
  Serial.print(MPU9150_readSensor(MPU9150_CMPS_YOUT_L,MPU9150_CMPS_YOUT_H));
  Serial.print("  ");
  Serial.print(MPU9150_readSensor(MPU9150_CMPS_ZOUT_L,MPU9150_CMPS_ZOUT_H));
  Serial.print("  ");
  Serial.print(MPU9150_readSensor(MPU9150_GYRO_XOUT_L,MPU9150_GYRO_XOUT_H));
  Serial.print("  ");
  Serial.print(MPU9150_readSensor(MPU9150_GYRO_YOUT_L,MPU9150_GYRO_YOUT_H));
  Serial.print("  ");
  Serial.print(MPU9150_readSensor(MPU9150_GYRO_ZOUT_L,MPU9150_GYRO_ZOUT_H));
  Serial.print("  ");
  Serial.print(MPU9150_readSensor(MPU9150_ACCEL_XOUT_L,MPU9150_ACCEL_XOUT_H));
  Serial.print("  ");
  Serial.print(MPU9150_readSensor(MPU9150_ACCEL_YOUT_L,MPU9150_ACCEL_YOUT_H));
  Serial.print("  ");
  Serial.print(MPU9150_readSensor(MPU9150_ACCEL_ZOUT_L,MPU9150_ACCEL_ZOUT_H));
  Serial.println();
  delay(100);
}

//http://pansenti.wordpress.com/2013/03/26/pansentis-invensense-mpu-9150-software-for-arduino-is-now-on-github/
//Thank you to pansenti for setup code.
//I will documented this one later.
