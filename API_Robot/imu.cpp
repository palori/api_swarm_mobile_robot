#include "imu.h"

int MPU9150_I2C_ADDRESS = 0x68;

void MPU9150_setup(){
  Wire.begin();

  // Clear the 'sleep' bit to start the sensor.
  MPU9150_writeSensor(MPU9150_PWR_MGMT_1, 0);

  setup_custom();
}

void setup_custom(){
  MPU9150_setupCompass();

  //Accessing the register 1B - Gyroscope Configuration (Sec. 4.4)
  //Setting the gyro to full scale +/- 250deg./s
  MPU9150_writeSensor(MPU9150_GYRO_CONFIG, 0);

  //Accessing the register 1C - Acccelerometer Configuration (Sec. 4.5)
  //Setting the accel to +/- 2g
  MPU9150_writeSensor(MPU9150_ACCEL_CONFIG, 0);
}

int MPU9150_writeSensor(int addr,int data){
  Wire.beginTransmission(MPU9150_I2C_ADDRESS);
  Wire.write(addr);
  Wire.write(data);
  Wire.endTransmission(true);

  return 1;
}

void MPU9150_setupCompass(){

  MPU9150_I2C_ADDRESS = 0x0C;      //change Address to Compass

  MPU9150_writeSensor(0x0A, 0x00); //PowerDownMode
  MPU9150_writeSensor(0x0A, 0x0F); //SelfTest
  MPU9150_writeSensor(0x0A, 0x00); //PowerDownMode

  MPU9150_I2C_ADDRESS = 0x68;      //change Address to MPU

  MPU9150_writeSensor(0x24, 0x40); //Wait for Data at Slave0
  MPU9150_writeSensor(0x25, 0x8C); //Set i2c address at slave0 at 0x0C
  MPU9150_writeSensor(0x26, 0x02); //Set where reading at slave 0 starts
  MPU9150_writeSensor(0x27, 0x88); //set offset at start reading and enable
  MPU9150_writeSensor(0x28, 0x0C); //set i2c address at slv1 at 0x0C
  MPU9150_writeSensor(0x29, 0x0A); //Set where reading at slave 1 starts
  MPU9150_writeSensor(0x2A, 0x81); //Enable at set length to 1
  MPU9150_writeSensor(0x64, 0x01); //overvride register
  MPU9150_writeSensor(0x67, 0x03); //set delay rate
  MPU9150_writeSensor(0x01, 0x80);

  MPU9150_writeSensor(0x34, 0x04); //set i2c slv4 delay
  MPU9150_writeSensor(0x64, 0x00); //override register
  MPU9150_writeSensor(0x6A, 0x00); //clear usr setting
  MPU9150_writeSensor(0x64, 0x01); //override register
  MPU9150_writeSensor(0x6A, 0x20); //enable master i2c mode
  MPU9150_writeSensor(0x34, 0x13); //disable slv4
}

int MPU9150_readSensor(int addrL, int addrH){
  Wire.beginTransmission(MPU9150_I2C_ADDRESS);
  Wire.write(addrL);
  Wire.endTransmission(false);

  Wire.requestFrom(MPU9150_I2C_ADDRESS, 1, true);
  byte L = Wire.read();

  Wire.beginTransmission(MPU9150_I2C_ADDRESS);
  Wire.write(addrH);
  Wire.endTransmission(false);

  Wire.requestFrom(MPU9150_I2C_ADDRESS, 1, true);
  byte H = Wire.read();

  return (int16_t)((H<<8)+L);
}

int MPU9150_readSensor(int addr){
  Wire.beginTransmission(MPU9150_I2C_ADDRESS);
  Wire.write(addr);
  Wire.endTransmission(false);

  Wire.requestFrom(MPU9150_I2C_ADDRESS, 1, true);
  return Wire.read();
}


void MPU9150_readGyro(){
  float X = IMU_gyro('X') / 131.0;
  float Y = IMU_gyro('Y') / 131.0;
  float Z = IMU_gyro('Z') / 131.0;

  if (X > 250) X -= 500.0;
  if (Y > 250) Y -= 500.0;
  if (Z > 250) Z -= 500.0;

  // NEED TO RETURN THE VALUES!
}

void MPU9150_readAccel(){
  float X = IMU_accel('X') / 16384.0;
  float Y = IMU_accel('Y') / 16384.0;
  float Z = IMU_accel('Z') / 16384.0;

  if (X > 250) X -= 4.0;
  if (Y > 250) Y -= 4.0;
  if (Z > 250) Z -= 4.0;

  // NEED TO RETURN THE VALUES!
}

void MPU9150_readCmps(){
  float X = IMU_accel('X');
  float Y = IMU_accel('Y');
  float Z = IMU_accel('Z');

  // NEED SOME PROCESSING
  // NEED TO RETURN THE VALUES!
}

int IMU_cmps(char coordinate){
  int sensorValue = 0;
  switch(coordinate){
    case 'X':
        sensorValue = MPU9150_readSensor(MPU9150_CMPS_XOUT_L,MPU9150_CMPS_XOUT_H);
      break;
    case 'Y':
        sensorValue = MPU9150_readSensor(MPU9150_CMPS_YOUT_L,MPU9150_CMPS_YOUT_H);
      break;
    case 'Z':
        sensorValue = MPU9150_readSensor(MPU9150_CMPS_ZOUT_L,MPU9150_CMPS_ZOUT_H);
      break;
    }
  return sensorValue;
}

int IMU_gyro(char coordinate){
  int sensorValue = 0;
  switch(coordinate){
    case 'X':
        sensorValue = MPU9150_readSensor(MPU9150_GYRO_XOUT_L,MPU9150_GYRO_XOUT_H);
      break;
    case 'Y':
        sensorValue = MPU9150_readSensor(MPU9150_GYRO_YOUT_L,MPU9150_GYRO_YOUT_H);
      break;
    case 'Z':
        sensorValue = MPU9150_readSensor(MPU9150_GYRO_ZOUT_L,MPU9150_GYRO_ZOUT_H);
      break;
    }
  return sensorValue;
}
int IMU_accel(char coordinate){
  int sensorValue = 0;
    switch(coordinate){
      case 'X':
          sensorValue = MPU9150_readSensor(MPU9150_ACCEL_XOUT_L,MPU9150_ACCEL_XOUT_H);
        break;
      case 'Y':
          sensorValue = MPU9150_readSensor(MPU9150_ACCEL_YOUT_L,MPU9150_ACCEL_YOUT_H);
        break;
      case 'Z':
          sensorValue = MPU9150_readSensor(MPU9150_ACCEL_ZOUT_L,MPU9150_ACCEL_ZOUT_H);
        break;
      }
    return sensorValue;
}