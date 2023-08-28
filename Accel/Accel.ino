#include <Wire.h>
#include <Arduino.h>
#include<Servo.h> 

#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

#include "SerialOrder.h"
#include "ArduinoReceiver.h"
#include "SerialParameters.h"

const int MPU = 0x68; // MPU6050 I2C address
float AccX, AccY, AccZ;
float GyroX, GyroY, GyroZ;
float accAngleX, accAngleY, gyroAngleX, gyroAngleY, gyroAngleZ;
float roll, pitch, yaw;
float AccErrorX, AccErrorY, GyroErrorX, GyroErrorY, GyroErrorZ;
float elapsedTime, currentTime, previousTime;
float FloatXtst, FloatYtst, FloatZtst;

bool FirstPass=false;

MPU6050 mpu;

int c = 0;
const int TransferGain=1000;
const int TransferGain2=10000;
int16_t RollTransfer, PitchTransfer, YawTransfer,
        AccelXTranfer, AccelYTranfer, AccelZTranfer;
int8_t Buffer16Int[2];

/*
===From MPU650_DMP6 code===
*/
#define INTERRUPT_PIN 2  // use pin 2 on Arduino Uno & most boards
#define LED_PIN 13 // (Arduino is 13, Teensy is 11, Teensy++ is 6)
bool blinkState = false;

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

/*
===From MPU650_DMP6 code===
*/

void setup() {
 
  // put your setup code here, to run once:
 Serial.begin(19200);
  Wire.begin();                      // Initialize comunication
  Wire.beginTransmission(MPU);       // Start communication with MPU6050 // MPU=0x68
  Wire.write(0x6B);                  // Talk to the register 6B
  Wire.write(0x00);                  // Make reset - place a 0 into the 6B register
  Wire.endTransmission(true);        //end the transmission
  /*
  // Configure Accelerometer Sensitivity - Full Scale Range (default +/- 2g)
  Wire.beginTransmission(MPU);
  Wire.write(0x1C);                  //Talk to the ACCEL_CONFIG register (1C hex)
  Wire.write(0x10);                  //Set the register bits as 00010000 (+/- 8g full scale range)
  Wire.endTransmission(true);
  // Configure Gyro Sensitivity - Full Scale Range (default +/- 250deg/s)
  Wire.beginTransmission(MPU);
  Wire.write(0x1B);                   // Talk to the GYRO_CONFIG register (1B hex)
  Wire.write(0x10);                   // Set the register bits as 00010000 (1000deg/s full scale)
  Wire.endTransmission(true);
  delay(20);
  */
  // Call this function if you need to get the IMU error values for your module
  calculate_IMU_error();
  delay(20);
  
}

void loop() {



  
  // put your main code here, to run repeatedly:
 // === Read acceleromter data === //
 float GyroXLocal;
  Wire.beginTransmission(MPU);
  Wire.write(0x3B); // Start with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
  
  //For a range of +-2g, we need to divide the raw values by 16384, according to the datasheet
  AccX = (Wire.read() << 8 | Wire.read()) / 16384.0; // X-axis value
  AccY = (Wire.read() << 8 | Wire.read()) / 16384.0; // Y-axis value
  AccZ = (Wire.read() << 8 | Wire.read()) / 16384.0; // Z-axis value
  // Calculating Roll and Pitch from the accelerometer data
  accAngleX = (atan(AccY / sqrt(pow(AccX, 2) + pow(AccZ, 2))) * 180 / PI) - 0.58; // AccErrorX ~(0.58) See the calculate_IMU_error()custom function for more details
  accAngleY = (atan(-1 * AccX / sqrt(pow(AccY, 2) + pow(AccZ, 2))) * 180 / PI) + 1.58; // AccErrorY ~(-1.58)
  // === Read gyroscope data === //
  
  previousTime = currentTime;        // Previous time is stored before the actual time read
  currentTime = millis();            // Current time actual time read
  elapsedTime = (currentTime - previousTime) / 1000; // Divide by 1000 to get seconds
  Wire.beginTransmission(MPU);
  Wire.write(0x43); // Gyro data first register address 0x43
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); // Read 4 registers total, each axis value is stored in 2 registers
  
  GyroX = (Wire.read() << 8 | Wire.read()) / 131.0; // For a 250deg/s range we have to divide first the raw value by 131.0, according to the datasheet
  GyroY = (Wire.read() << 8 | Wire.read()) / 131.0;
  GyroZ = (Wire.read() << 8 | Wire.read()) / 131.0;
  // Correct the outputs with the calculated error values
  
  GyroX = GyroX + 0.56; // GyroErrorX ~(-0.56)
  GyroY = GyroY - 2; // GyroErrorY ~(2)
  GyroZ = GyroZ + 0.79; // GyroErrorZ ~ (-0.8)
  
  // Currently the raw values are in degrees per seconds, deg/s, so we need to multiply by sendonds (s) to get the angle in degrees
  float gyX, gyY, gyZ, rollX;
  
  //gyroAngleX = gyroAngleX + GyroX * elapsedTime; // deg/s * s = deg
  //gyroAngleY = gyroAngleY + GyroY * elapsedTime;
  yaw =  yaw + GyroZ * elapsedTime;

  gyroAngleX = 0.96 * gyroAngleX + 0.04 * accAngleX;
  gyroAngleY = 0.96 * gyroAngleY + 0.04 * accAngleY;
 
   
  // Complementary filter - combine acceleromter and gyro angle values
  //roll = 0.96 * gyroAngleX + 0.04 * accAngleX;
  //pitch = 0.96 * gyroAngleY + 0.04 * accAngleY;

  roll=gyroAngleX;
  pitch=gyroAngleY;
  // Print the values on the serial monitor

RollTransfer=roll*TransferGain;
PitchTransfer=pitch*TransferGain;
YawTransfer=yaw*TransferGain;

AccelXTranfer=AccX*TransferGain2;
AccelYTranfer=AccY*TransferGain2;
AccelZTranfer=AccZ*TransferGain2;

  /*WriteCommandInt16(MEASURED_ROLL,RollTransfer);
  WriteCommandInt16(MEASURED_PITCH,PitchTransfer);
  WriteCommandInt16(MEASURED_YAW,YawTransfer);
  WriteCommandInt16(MEASURED_ACCEL_X,AccelXTranfer);
  WriteCommandInt16(MEASURED_ACCEL_Y,AccelYTranfer);
  WriteCommandInt16(MEASURED_ACCEL_Z,AccelZTranfer);*/
  Serial.println(" ");
  Serial.print("/ X ");
  Serial.print(roll);
  Serial.print("/ Y ");
  Serial.print(pitch);
  Serial.print("/ Z ");
  Serial.print(yaw);

  /*Serial.println("/ X ");
  Serial.print(AccX);
  Serial.print("/ Y ");
  Serial.print(AccX);
  Serial.print("/ Z ");
  Serial.print(AccX);*/

  
}

void calculate_IMU_error() {
  // We can call this funtion in the setup section to calculate the accelerometer and gyro data error. From here we will get the error values used in the above equations printed on the Serial Monitor.
  // Note that we should place the IMU flat in order to get the proper values, so that we then can the correct values
  // Read accelerometer values 200 times
  while (c < 200) {
    Wire.beginTransmission(MPU);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 6, true);
    AccX = (Wire.read() << 8 | Wire.read()) / 16384.0 ;
    AccY = (Wire.read() << 8 | Wire.read()) / 16384.0 ;
    AccZ = (Wire.read() << 8 | Wire.read()) / 16384.0 ;
    // Sum all readings
    AccErrorX = AccErrorX + ((atan((AccY) / sqrt(pow((AccX), 2) + pow((AccZ), 2))) * 180 / PI));
    AccErrorY = AccErrorY + ((atan(-1 * (AccX) / sqrt(pow((AccY), 2) + pow((AccZ), 2))) * 180 / PI));
    c++;
  }
  //Divide the sum by 200 to get the error value
  AccErrorX = AccErrorX / 200;
  AccErrorY = AccErrorY / 200;
  c = 0;
  // Read gyro values 200 times
  while (c < 200) {
    Wire.beginTransmission(MPU);
    Wire.write(0x43);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 6, true);
    GyroX = Wire.read() << 8 | Wire.read();
    GyroY = Wire.read() << 8 | Wire.read();
    GyroZ = Wire.read() << 8 | Wire.read();
    // Sum all readings
    GyroErrorX = GyroErrorX + (GyroX / 131.0);
    GyroErrorY = GyroErrorY + (GyroY / 131.0);
    GyroErrorZ = GyroErrorZ + (GyroZ / 131.0);
    c++;
  }
  //Divide the sum by 200 to get the error value
  GyroErrorX = GyroErrorX / 200;
  GyroErrorY = GyroErrorY / 200;
  GyroErrorZ = GyroErrorZ / 200;
  // Print the error values on the Serial Monitor
  Serial.print("AccErrorX: ");
  Serial.println(AccErrorX);
  Serial.print("AccErrorY: ");
  Serial.println(AccErrorY);
  Serial.print("GyroErrorX: ");
  Serial.println(GyroErrorX);
  Serial.print("GyroErrorY: ");
  Serial.println(GyroErrorY);
  Serial.print("GyroErrorZ: ");
  Serial.println(GyroErrorZ);
  
}




void WriteOrder(enum SerialOrder CommandOrder)
{
  uint8_t* Order = (uint8_t*) &CommandOrder;
  Serial.write(Order, sizeof(uint8_t));
}

void WriteInt16(int16_t Num)
{
  int8_t buffer[2] = {(int8_t) (Num & 0xff), (int8_t) (Num >> 8)};
  *Buffer16Int = *buffer;
  Serial.write((uint8_t*)&Buffer16Int, 2*sizeof(int8_t));
}

void WriteInt16_2Command(enum SerialOrder CommandOrder1, int16_t Num1, enum SerialOrder CommandOrder2,int16_t Num2 )
{
  int8_t CompoundBuffer[6];
  *CompoundBuffer=(uint8_t*) &CommandOrder1;
  int8_t TemporaryNum1[2] ={(int8_t) (Num1 & 0xff), (int8_t) (Num1 >> 8)};
  
  CompoundBuffer[1]=TemporaryNum1[1];
  CompoundBuffer[2]=TemporaryNum1[2];

  CompoundBuffer[3]=(uint8_t*) &CommandOrder2;

  int8_t TemporaryNum2[2] ={(int8_t) (Num2 & 0xff), (int8_t) (Num2 >> 8)};

  CompoundBuffer[4]=TemporaryNum1[1];
  CompoundBuffer[5]=TemporaryNum1[2];

  //*CompoundBuffer[1]={(int8_t) (Num1 & 0xff), (int8_t) (Num1 >> 8)};
  //int8_t buffer[2] = {(int8_t) (Num1 & 0xff), (int8_t) (Num1 >> 8)};
  //*Buffer16Int = *buffer;
  
  Serial.write((uint8_t*)&CompoundBuffer, 6*sizeof(int8_t));
}

void WriteCommandInt16(enum SerialOrder CommandOrder, int16_t Command){
  WriteOrder(CommandOrder);
  WriteInt16(Command);
  }
  
