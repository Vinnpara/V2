//USING PITCH AND ROLL FROM OLDER CODE
//AND YAW FROM NEWER LIBRARY
//FOR STABILITY REASONS

#include <Wire.h>
#include <Arduino.h>
#include<Servo.h> 
#include <LiquidCrystal.h>
#define MAX_RADAR_ANGLE 170

#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

#include "SerialOrder.h"
#include "ArduinoReceiver.h"
#include "SerialParameters.h"

#define INTERRUPT_PIN 2  // use pin 2 on Arduino Uno & most boards
#define LED_PIN 13 // (Arduino is 13, Teensy is 11, Teensy++ is 6)

#define echoPin 4 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 5
bool blinkState = false;
int8_t Buffer16Int[2];

int16_t ValueSent;
int32_t val32;
static int PitchCounter, YawCounter, RollCounter;

static bool FirstPassDone;

SerialOrder OrderReceived;
SerialOrder OrderSent;

char buffin[64];
int16_t GyroTst;

int Contrast=75;

LiquidCrystal lcd(12, 11, 5, 4, 3, 8);  

SerialOrder OrderTest;
// ================================================================
// ===               FROM OLDER CODE                ===
// ================================================================
const int MPU = 0x68; // MPU6050 I2C address
float AccX, AccY, AccZ;
float GyroX, GyroY, GyroZ;
float accAngleX, accAngleY, gyroAngleX, gyroAngleY, gyroAngleZ;
float roll, pitch, yaw;
float AccErrorX, AccErrorY, GyroErrorX, GyroErrorY, GyroErrorZ;
float elapsedTime, currentTime, previousTime;
int c = 0;

MPU6050 mpu;

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

// packet structure for InvenSense teapot demo
uint8_t teapotPacket[14] = { '$', 0x02, 0,0, 0,0, 0,0, 0,0, 0x00, 0x00, '\r', '\n' };


const int TransferGain=1000;
const int TransferGain2=10000;
int16_t RollTransfer, PitchTransfer, YawTransfer,
        AccelXTranfer, AccelYTranfer, AccelZTranfer;
//float pitch, roll, yaw;


// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}

// ================================================================
// ===               RADAR FUNCTION VARIABLES               ===
// ================================================================
long duration;
int distance, ServoRightValue, ServoLeftValue;
float MeasuredDistance;
int16_t MeasuredDistance_int16, ServoAngleInt16, ProgramIterationTime16, ServRead;
unsigned long Time1, Time2, Time3, IterationTime, TimeoutValue, ProgramIterationTime;
unsigned int DelayTime;

int16_t MeasuredRadarValues[MAX_RADAR_ANGLE];

Servo serv, serv1;
float pos_2;

SerialOrder OrderReceivedRad, OrderSentRad;
static bool PC_not_Ready=1;

void setup() {
    /*FOR GYROSCOPE*/
  // put your setup code here, to run once:
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties

        Wire.beginTransmission(MPU);       // Start communication with MPU6050 // MPU=0x68
        Wire.write(0x6B);                  // Talk to the register 6B
        Wire.write(0x00);                  // Make reset - place a 0 into the 6B register
        Wire.endTransmission(true);        //end the transmission

        
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    // initialize serial communication
    // (115200 chosen because it is required for Teapot Demo output, but it's
    // really up to you depending on your project)
    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin, INPUT); // Sets the echoPin as an Input
    Serial.begin(9600);
    while (!Serial); // wait for Leonardo enumeration, others continue immediately

    // NOTE: 8MHz or slower host processors, like the Teensy @ 3.3V or Arduino
    // Pro Mini running at 3.3V, cannot handle this baud rate reliably due to
    // the baud timing being too misaligned with processor ticks. You must use
    // 38400 or slower in these cases, or use some kind of external separate
    // crystal solution for the UART timer.

    // initialize device
    //Serial.println(F("Initializing I2C devices..."));
    mpu.initialize();
    pinMode(INTERRUPT_PIN, INPUT);

    // verify connection
    //Serial.println(F("Testing device connections..."));
    //Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

    // wait for ready
    
    /*Serial.println(F("\nSend any character to begin DMP programming and demo: "));
    while (Serial.available() && Serial.read()); // empty buffer
    while (!Serial.available());                 // wait for data
    while (Serial.available() && Serial.read()); // empty buffer again*/

    // load and configure the DMP
    //Serial.println(F("Initializing DMP..."));
    devStatus = mpu.dmpInitialize();

    // supply your own gyro offsets here, scaled for min sensitivity
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788); // 1688 factory default for my test chip

    // make sure it worked (returns 0 if so)
    if (devStatus == 0) {
        // Calibration Time: generate offsets and calibrate our MPU6050
        mpu.CalibrateAccel(6);
        mpu.CalibrateGyro(6);
        mpu.PrintActiveOffsets();
        // turn on the DMP, now that it's ready
        //Serial.println(F("Enabling DMP..."));
        mpu.setDMPEnabled(true);

        // enable Arduino interrupt detection
        
        //Serial.print(F("Enabling interrupt detection (Arduino external interrupt "));
        //Serial.print(digitalPinToInterrupt(INTERRUPT_PIN));
        //Serial.println(F(")..."));
        
        attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        
        //Serial.println(F("DMP ready! Waiting for first interrupt..."));
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        
        //Serial.print(F("DMP Initialization failed (code "));
        //Serial.print(devStatus);
        //Serial.println(F(")"));
    }
  calculate_IMU_error();
  delay(20);
    // configure LED for output
    /*pinMode(LED_PIN, OUTPUT);
    analogWrite(6,Contrast);
     lcd.begin(16, 2);*/


  /*FOR RADAR*/
  TimeoutValue=20000; //20k microsecons so max range of about 3m
  serv.attach(10);
  serv1.attach(9);

  FirstPassDone=0;
}

void loop() {
  // put your main code here, to run repeatedly:
 if(Serial.available()>0){  
  //SerialBasic();
  //ReadSerial();
  //GetMessages();
  while(Serial.available()>0){
  RadarSweep(171,-1);
  if (!dmpReady) return;
  //MeasureGyro();
  FirstPassDone=1;
  }
  
}

    // read a packet from FIFO
    
}

///ACCEL/GYRO FUNC///
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
  
  /*Serial.print("AccErrorX: ");
  Serial.println(AccErrorX);
  Serial.print("AccErrorY: ");
  Serial.println(AccErrorY);
  Serial.print("GyroErrorX: ");
  Serial.println(GyroErrorX);
  Serial.print("GyroErrorY: ");
  Serial.println(GyroErrorY);
  Serial.print("GyroErrorZ: ");
  Serial.println(GyroErrorZ);*/
  
}


void MeasureGyro(){
  
  if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) {
  GetMessages();    
      
  // display Euler angles in degrees
  mpu.dmpGetQuaternion(&q, fifoBuffer);
  mpu.dmpGetGravity(&gravity, &q);
  mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

  yaw =ypr[0] * 180/M_PI;
  //pitch=ypr[1] * 180/M_PI;
  //roll=ypr[2] * 180/M_PI;

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
  //yaw =  yaw + GyroZ * elapsedTime;

  gyroAngleX = 0.96 * gyroAngleX + 0.04 * accAngleX;
  gyroAngleY = 0.96 * gyroAngleY + 0.04 * accAngleY;
 
   
  // Complementary filter - combine acceleromter and gyro angle values
  //roll = 0.96 * gyroAngleX + 0.04 * accAngleX;
  //pitch = 0.96 * gyroAngleY + 0.04 * accAngleY;

  roll=gyroAngleX;
  pitch=gyroAngleY;

  RollTransfer=roll*TransferGain;
  PitchTransfer=pitch*TransferGain;
  YawTransfer=yaw*TransferGain;
  
  PitchCounter ++;

  
  /*Serial.print("ypr\t");
  Serial.print(yaw);
  Serial.print("\t");
  Serial.print(roll);
  Serial.print("\t");
  Serial.println(pitch);*/

      }
  
 }

///RADAR FUNCTION////

void RadarSweep(int MaxRightValue, int MaxLeftValue){
  
    for(ServoRightValue=0; ServoRightValue < MaxRightValue; ServoRightValue++){
    //MeasureGyro();  
    PingUltraSoundSensor(ServoRightValue);
    MeasuredRadarValues[ServoRightValue]=MeasuredDistance_int16;
    serv.write(ServoRightValue);
    //MeasuredRadarValues[ServoRightValue]=MeasuredDistance_int16;
    //Serial.print("Servo angle Written: ");
    //Serial.println(ServoRightValue);
    }
    
    for(ServoLeftValue=MaxRightValue; ServoLeftValue > MaxLeftValue; ServoLeftValue--){
    //MeasureGyro();
    PingUltraSoundSensor(ServoLeftValue);
    MeasuredRadarValues[ServoLeftValue]=MeasuredDistance_int16;
    serv.write(ServoLeftValue);
    //MeasuredRadarValues[ServoLeftValue]=MeasuredDistance_int16;
    //Serial.print("Servo angle Written: ");
    //Serial.println(ServoLeftValue);
    }    
  
}

void PingUltraSoundSensor(int16_t i){
  
  Time1 = millis();
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH, TimeoutValue);
  Time2 = millis();    

  IterationTime=Time2-Time1;
  
  if(IterationTime >= 25)
  MeasuredDistance_int16 = 200;
  else
  MeasuredDistance_int16 = duration * 0.034 / 2;

  DelayTime=25-IterationTime;
  
  delay(DelayTime);
  
  /*Serial.print("Distance: ");
  Serial.println(MeasuredDistance_int16);
  Serial.print("Duration: ");
  Serial.println(ServoAngleInt16);*/

  ServoAngleInt16=serv.read();
  
  //Serial.print("Servo angle: ");
  //Serial.println(ServoAngleInt16);
  
  //Serial.print("Iteration Time ");
  //Serial.println(ProgramIterationTime);

  //WriteCommandInt16(RADAR_DISTANCE,MeasuredDistance_int16);
  //WriteCommandInt16(RADAR_POSITION,ServoAngleInt16);

  //ReadSerial();
  MeasureGyro();  
  Time3=millis();
  ProgramIterationTime16= Time3-Time1;
  if(FirstPassDone){
    ReadSerial(i);
  }
  //WriteCommandInt16(ARD_ITERATION_TIME,ProgramIterationTime16);
  //WriteInt16_2Command(RADAR_DISTANCE_2, MeasuredDistance_int16, RADAR_POSITION_2, ServoAngleInt16);
  }

///SERIAL SEND RECEIVE/////
enum SerialOrder read_order()
{
  return (SerialOrder) Serial.read();
}  

void ReadSerial(int16_t i){

 //if(Serial.available()>0){   
    SerialOrder order_received = read_order();
    OrderReceived=order_received;
    
    switch(order_received){
      
     case PC_NOT_READY:
      {    
        PC_not_Ready=0;
        break;
      }
      case REQUEST_RADAR:
      {
        PC_not_Ready=1;
        int16_t ValuetoSend=MeasuredRadarValues[i];
        //WriteCommandInt16(RADAR_DISTANCE, ValuetoSend);
        
        WriteOrder(RADAR_DISTANCE);
        WriteInt16(ValuetoSend);
        
        break;
      }
      case REQUEST_RADAR_POS:
      {
        PC_not_Ready=0;
        //WriteCommandInt16(RADAR_POSITION, ServoAngleInt16);
        WriteOrder(RADAR_POSITION);
        WriteInt16(ServoAngleInt16);
        break;
      }
      
      }
 //}
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
  GyroTst=Command;
  WriteOrder(CommandOrder);
  WriteInt16(Command);
  }

void WriteInt32( int32_t num)
{
  int8_t buffer[4] = { (int8_t)(num & 0xff), (int8_t)(num >> 8 & 0xff), (int8_t)(num >> 16 & 0xff), (int8_t)(num >> 24 & 0xff) };
  Serial.write((char*)buffer, 4 * sizeof(int8_t));
}
  


void GetMessages(){
  
  if(Serial.available() > 0)
  {
    
    SerialOrder order_received = read_order();
    OrderReceived=order_received;
    
    switch(order_received){
      
      case PC_NOT_READY:
      {
          break;
        
       }
      
      case REQUEST_ROLL:
      {
        float TransferValue=roll*TransferGain;
        int32_t ValueToSend=TransferValue;
        OrderSent=MEASURED_ROLL;
        WriteOrder(OrderSent);
        WriteInt32(ValueToSend);
        break;
      }
      
      case REQUEST_PITCH:
      {
        
        float TransferValue=pitch*TransferGain;
        int32_t ValueToSend=TransferValue;
        OrderSent=MEASURED_PITCH;
        WriteOrder(OrderSent);
        WriteInt32(ValueToSend);
        break;
      }

      case REQUEST_YAW:
      {
        float TransferValue=yaw*TransferGain;
        int32_t ValueToSend=TransferValue;
        OrderSent=MEASURED_YAW; 
        WriteOrder(OrderSent);
        WriteInt32(ValueToSend);
        break;
      }

     /*case REQUEST_RADAR:
      {
        
        //WriteCommandInt16(RADAR_DISTANCE, MeasuredDistance_int16);
        WriteOrder(RADAR_DISTANCE);
        WriteInt16(MeasuredDistance_int16);
        break;
      }
      case REQUEST_RADAR_POS:
      {
        
        //WriteCommandInt16(RADAR_POSITION, ServoAngleInt16);
        WriteOrder(RADAR_POSITION);
        WriteInt16(ServoAngleInt16);
        break;
      }*/
      
      }
  }
 
}
