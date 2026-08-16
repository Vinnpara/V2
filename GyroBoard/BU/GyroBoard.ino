
//USING PITCH AND ROLL FROM OLDER CODE
//AND YAW FROM NEWER LIBRARY
//FOR STABILITY REASONS
//PIN A5 SCL, A4 SDA

#include <Wire.h>
#include <Arduino.h>

#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

#include "SerialOrder.h"
#include "ArduinoReceiver.h"
#include "SerialParameters.h"

#define INTERRUPT_PIN 2  // use pin 2 on Arduino Uno & most boards
#define LED_PIN 13 // (Arduino is 13, Teensy is 11, Teensy++ is 6)

#define MAX_PAYLOAD_SIZE 64

bool blinkState = false;
int8_t Buffer16Int[2];

int16_t ValueSent;
int32_t val32;
static int PitchCounter, 
           YawCounter, 
           RollCounter, 
           SerialCounter,
           ValidPCHeartBeatCounter = 0;

static bool FirstPassDone = false,
            RollRequest = false, 
            PitchRequest = false, 
            YawRequest = false, 
            AccXRequest = false, 
            AccYRequest = false, 
            AccZRequest = false;

static SerialOrder MeasurementRequested;

SerialOrder OrderReceived;
SerialOrder OrderSent;

char buffin[MAX_PAYLOAD_SIZE];
unsigned int bufferIndex = 0;
bool isReceiving = false;

int16_t GyroTst;

SerialOrder OrderTest;
// ================================================================
// ===               FROM OLDER CODE                ===
// ================================================================
const int MPU = 0x68; // MPU6050 I2C address
float AccX, AccY, AccZ;
float GyroX, GyroY, GyroZ;
float accAngleX, accAngleY,accAngleZ, gyroAngleX, gyroAngleY, gyroAngleZ;
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
int32_t RollTransfer, PitchTransfer, YawTransfer,
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

SerialOrder OrderReceivedRad, OrderSentRad;
static bool PC_not_Ready=1,
            PC_Ready = 0,
            CommsEstablished =0,
            ReStartSerial = 0,
            SerialFlushed = 0,
            ValidHeartBeatPC = 0;

unsigned long previousMillis = 0,
              previousMillis2 = 0; 
const long interval = 50,
           IntervalPCHBMonitoring = 2000; // Interval in milliseconds

const char START_MARKER = '[',
           END_MARKER = ']';

void setup() {
  // put your setup code here, to run once:
  /*FOR GYROSCOPE*/
  // put your setup code here, to run once:
 pinMode(LED_BUILTIN, OUTPUT);
 
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

    Serial.begin(115200);
    while (!Serial); // wait for Leonardo enumeration, others continue immediately

    
    //Serial.print("\nArd setup start ");
    //Serial.print("\nArd Bool flag  ");
    //Serial.print(FirstPassDone);
        
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

  FirstPassDone =1;

  //Serial.print("\nArd setup done");
  //Serial.print("\nArd Bool flag  ");
  //Serial.print(FirstPassDone);
  
  delay(20);

     
}

void loop() {
  // put your main code here, to run repeatedly:

MeasureGyro();


if(Serial.available()>0)
{
  //First, signal arduino readiness
  //SignalArduinoReadiness();
  //SendHeartBeat();

  //Peek functions will monitor for anything incomining,
  //incl. PC readiness, heartbeat, and commands
  //PeekMessage();
  SignalArdReady();
  SendHeartBeat();
  //SendDataPackets(MEASURED_PITCH, pitch);
  //SendDataPackets(MEASURED_ROLL, roll);

  
  //SendDataPacketsPitchRoll();  
  BulbON();
}

//Determine the availability of a HB and handle the Comms
//established flag
//MonitorPCHeartBeat();
//DeterminePCReadiness();

if(CommsEstablished && Serial.available()>0)
{
  //Communications are established and device can start transmitting data
  //FlashBulb(750);
  //MessageGyro(MeasurementRequested);
  //SendDataPackets(MEASURED_PITCH, pitch);
  //BulbON();
}

if(!Serial.available())
{
  FlashBulb(25);
}

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
  
}

void MeasureGyro(){
  
  if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) {
  //GetMessages();    
      
  // display Euler angles in degrees
  mpu.dmpGetQuaternion(&q, fifoBuffer);
  mpu.dmpGetGravity(&gravity, &q);
  mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

  yaw =ypr[0] * 180/M_PI;
  //pitch=ypr[1] * 180/M_PI;
  //roll=ypr[2] * 180/M_PI;
  }
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
  //accAngleY = (atan(abs(AccY)/abs(AccX)));
  

  
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
  
  gyroAngleX = gyroAngleX + GyroX * elapsedTime; // deg/s * s = deg
  gyroAngleY = gyroAngleY + GyroY * elapsedTime;
  //yaw =  yaw + GyroZ * elapsedTime;

  gyroAngleX = 0.96 * gyroAngleX + 0.04 * accAngleX;
  gyroAngleY = 0.96 * gyroAngleY + 0.04 * accAngleY;
 
   
  // Complementary filter - combine acceleromter and gyro angle values
  roll = 0.96 * gyroAngleX + 0.04 * accAngleX;
  pitch = 0.96 * gyroAngleY + 0.04 * accAngleY;

  //roll=gyroAngleX;
  //pitch=gyroAngleY;

  RollTransfer=roll*TransferGain;
  PitchTransfer=pitch*TransferGain;
  YawTransfer=yaw*TransferGain;
  
  PitchCounter ++;

  //Serial.print("\nypr  ");
  //Serial.print(gyroAngleX);
  //Serial.print("      ");
  //Serial.print(roll);
  //Serial.print("     ");
  //Serial.println(pitch);
 }

 
///SERIAL SEND RECEIVE/////


void MonitorPCHeartBeat()
{

    
   unsigned long currentMillis = millis();
  // Check if the interval has passed
  if (currentMillis - previousMillis2 >= IntervalPCHBMonitoring) 
  {
    previousMillis2 = currentMillis; // Save the last time you blinked
    //IntervalPCHBMonitoring
    if(ValidPCHeartBeatCounter > 0)
    {
        ValidHeartBeatPC = 1;
    }
   else
        ValidHeartBeatPC = 0;
    
  ValidPCHeartBeatCounter = 0;
  }

  
}

void DeterminePCReadiness()
{
//A valid PC heartbeat and PC ready flag will trigger the CommsEstablished flag
//If there is a vaild CommsEstablished and we loose PC heart beat, commsestablished is false

if(ValidHeartBeatPC && PC_Ready)
    CommsEstablished =1;

if(CommsEstablished && !ValidHeartBeatPC)
  {
    CommsEstablished =0;
    PC_Ready = 0;  
  }
}



void SignalArduinoReadiness()
{
  //uint8_t* Order = (uint8_t*) ARDUINO_READY;
  //Serial.write(Order, sizeof(uint8_t));
  WriteOrder(ARDUINO_READY);
}

enum SerialOrder read_order()
{
  return (SerialOrder) Serial.read();
} 
 
void WriteOrder(enum SerialOrder CommandOrder)
{
  uint8_t* Order = (uint8_t*) &CommandOrder;
  Serial.write(Order, sizeof(uint8_t));
}

void WriteInt32( int32_t num)
{
  int8_t buffer[4] = { (int8_t)(num & 0xff), (int8_t)(num >> 8 & 0xff), (int8_t)(num >> 16 & 0xff), (int8_t)(num >> 24 & 0xff) };
  Serial.write((char*)buffer, 4 * sizeof(int8_t));
}

void WriteInt32_2val(int32_t num, int32_t num2)
{
  int32_t buffer[8] = { (int32_t)(num & 0xff), (int32_t)(num >> 8 & 0xff), (int32_t)(num >> 16 & 0xff), (int32_t)(num >> 24 & 0xff), 
                       (int32_t)(num2 >> 32 & 0xff), (int32_t)(num2 >> 40 & 0xff), (int32_t)(num2 >> 48 & 0xff), (int32_t)(num2 >> 56 & 0xff)};
  Serial.write((char*)buffer, 8 * sizeof(int32_t));
}

void MessagePitchRoll (){

WriteInt32_2val(PitchTransfer, RollTransfer);

}

void MessageGyro(){
  
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


      case REQUEST_ACCEL_X:
      {
        float TransferValue=AccX*TransferGain;
        int32_t ValueToSend=TransferValue;
        OrderSent=MEASURED_ACCEL_X;
        WriteOrder(OrderSent);
        WriteInt32(ValueToSend);
        break;
      }
      
      case REQUEST_ACCEL_Y:
      {
        float TransferValue=AccY*TransferGain;
        int32_t ValueToSend=TransferValue;
        OrderSent=MEASURED_ACCEL_Y;
        WriteOrder(OrderSent);
        WriteInt32(ValueToSend);
        break;
      }

      case REQUEST_ACCEL_Z:
      {
        float TransferValue=AccZ*TransferGain;
        int32_t ValueToSend=TransferValue;
        OrderSent=MEASURED_ACCEL_Z;
        WriteOrder(OrderSent);
        WriteInt32(ValueToSend);
        break;
      }
      default :
      {
        break;
      }
      
      }
  }
  
  }

void MessageGyro(SerialOrder order_received){
        
    switch(order_received)
  {
      
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
    
        MeasurementRequested =0;
        RollRequest = 0;
        break;
      }
      
      case REQUEST_PITCH:
      {
        
        float TransferValue=pitch*TransferGain;
        int32_t ValueToSend=TransferValue;
        OrderSent=MEASURED_PITCH;
        WriteOrder(OrderSent);
        WriteInt32(ValueToSend);
    
        MeasurementRequested =0;
        PitchRequest = 0;   
        break;
      }

      case REQUEST_YAW:
      {
        float TransferValue=yaw*TransferGain;
        int32_t ValueToSend=TransferValue;
        OrderSent=MEASURED_YAW; 
        WriteOrder(OrderSent);
        WriteInt32(ValueToSend);
    
        MeasurementRequested =0;
        YawRequest = 0;   
        break;
      }


      case REQUEST_ACCEL_X:
      {
        float TransferValue=AccX*TransferGain;
        int32_t ValueToSend=TransferValue;
        OrderSent=MEASURED_ACCEL_X;
        WriteOrder(OrderSent);
        WriteInt32(ValueToSend);
    
        MeasurementRequested =0;
        AccXRequest = 0;
        break;
      }
      
      case REQUEST_ACCEL_Y:
      {
        float TransferValue=AccY*TransferGain;
        int32_t ValueToSend=TransferValue;
        OrderSent=MEASURED_ACCEL_Y;
        WriteOrder(OrderSent);
        WriteInt32(ValueToSend);
    
        MeasurementRequested =0;
        AccYRequest = 0;
        break;
      }

      case REQUEST_ACCEL_Z:
      {
        float TransferValue=AccZ*TransferGain;
        int32_t ValueToSend=TransferValue;
        OrderSent=MEASURED_ACCEL_Z;
        WriteOrder(OrderSent);
        WriteInt32(ValueToSend);
    
        MeasurementRequested =0;
        AccZRequest = 0;
        break;
      }
    
    default :
    {
        break;
    }
      
    }
  
  
  }

void PeekMessage()
{
 if(Serial.available() > 0)
  {
      char incomingChar = Serial.peek();
      SerialOrder CommandReceived = (SerialOrder)incomingChar;
    
  switch(CommandReceived)
  {

      case PC_NOT_READY:
      {
          Serial.read();
          break;
      }
      case PC_READY:
      {
          Serial.read();
          PC_Ready = 1;
          //CommsEstablished = 1;
          break;
      }
     case PC_HEARTBEAT:
      {
          Serial.read();
          ValidPCHeartBeatCounter++;
          break;
      }
     case REQUEST_ROLL:
      {
          RollRequest = 1;
          MeasurementRequested = REQUEST_ROLL;
          break;
      }   
     case REQUEST_PITCH:
      {
          PitchRequest = 1;
          MeasurementRequested = REQUEST_PITCH;
          break;
      }
     case REQUEST_YAW:
      {
          YawRequest = 1;
          MeasurementRequested = REQUEST_YAW;
          break;
      }
     case REQUEST_ACCEL_X:
      {
          AccXRequest = 1;
          MeasurementRequested = REQUEST_ACCEL_X;
          break;
      }
     case REQUEST_ACCEL_Y:
      {
          AccYRequest = 1;
          MeasurementRequested = REQUEST_ACCEL_Y;
          break;
      }
     case REQUEST_ACCEL_Z:
      {
          AccZRequest = 1;
          MeasurementRequested = REQUEST_ACCEL_Z;
          break;
      }
     default :
      {
      break; 
    }   
    
  }
  
  }
  
}


void PeakAndMessagePC()
{

    if(Serial.available() > 0)
  {
  char incomingChar = Serial.peek();
  SerialOrder CommandReceived = (SerialOrder)incomingChar;

switch(CommandReceived){
      
      case PC_NOT_READY:
      {
          Serial.read();
          break;
        
       }
      
      case REQUEST_ROLL:
      {
        Serial.read();
        float TransferValue=roll*TransferGain;
        int32_t ValueToSend=TransferValue;
        OrderSent=MEASURED_ROLL;
        WriteOrder(OrderSent);
        WriteInt32(ValueToSend);
        break;
      }
      
      case REQUEST_PITCH:
      {
        
        Serial.read();        
        float TransferValue=pitch*TransferGain;
        int32_t ValueToSend=TransferValue;
        OrderSent=MEASURED_PITCH;
        WriteOrder(OrderSent);
        WriteInt32(ValueToSend);
        break;
      }

      case REQUEST_YAW:
      {
        Serial.read();
        float TransferValue=yaw*TransferGain;
        int32_t ValueToSend=TransferValue;
        OrderSent=MEASURED_YAW; 
        WriteOrder(OrderSent);
        WriteInt32(ValueToSend);
        break;
      }


      case REQUEST_ACCEL_X:
      {
        Serial.read();        
        float TransferValue=AccX*TransferGain;
        int32_t ValueToSend=TransferValue;
        OrderSent=MEASURED_ACCEL_X;
        WriteOrder(OrderSent);
        WriteInt32(ValueToSend);
        break;
      }
      
      case REQUEST_ACCEL_Y:
      {
        Serial.read();        
        float TransferValue=AccY*TransferGain;
        int32_t ValueToSend=TransferValue;
        OrderSent=MEASURED_ACCEL_Y;
        WriteOrder(OrderSent);
        WriteInt32(ValueToSend);
        break;
      }

      case REQUEST_ACCEL_Z:
      {
        Serial.read();        
        float TransferValue=AccZ*TransferGain;
        int32_t ValueToSend=TransferValue;
        OrderSent=MEASURED_ACCEL_Z;
        WriteOrder(OrderSent);
        WriteInt32(ValueToSend);
        break;
      }
      
      }
  }
}

void SignalArdReady()
{
  Serial.print(START_MARKER);
  Serial.print(ARDUINO_READY);
  Serial.print(END_MARKER);
}

void SendDataPackets(SerialOrder Type, float value)
{
  Serial.print(START_MARKER);
  Serial.print(Type);
  Serial.print(':');
  Serial.print(value);
  Serial.print(END_MARKER);
}

void SendDataPacketsPitchRoll()
{
  Serial.print(START_MARKER);
  Serial.print(MEASURED_PITCH);
  Serial.print(':');
  Serial.print(pitch);
  Serial.print(',');
  Serial.print(MEASURED_ROLL);
  Serial.print(':');
  Serial.print(roll);  
  Serial.print(END_MARKER);
}

void SendDataPacketHB()
{
  Serial.print(START_MARKER);
  Serial.print('?');
  Serial.print(END_MARKER);
}

void SendHeartBeat()
{
  unsigned long currentMillis = millis();
  // Check if the interval has passed
  if (currentMillis - previousMillis >= interval) 
  {
    previousMillis = currentMillis; // Save the last time you blinked
    // Send heartbeat
    SendDataPacketHB();
  }

}

void ReadAndParseSerial()
{
  if(Serial.available())
  {
  char currentByte = (char)Serial.read(); // Pulls one byte out of the ring buffer

        // Case A: Detect start marker to sync or reset the frame
        if (currentByte == START_MARKER) 
        {
            bufferIndex = 0;
            isReceiving = true;
           
        }

        // Case B: If we are in the middle of capturing a packet
        if (isReceiving) {
            
            // If we hit the end marker, the packet is whole and complete
            if (currentByte == END_MARKER) {
                buffin[bufferIndex] = '\0'; // Manually append null-terminator for string safety
                
                // Route the clean text string to your execution logic
                //executeReceivedCommand(incomingPacketBuffer);
                
                // Reset state for the next incoming packet
                isReceiving = false;
                bufferIndex = 0;
            } 
            // Otherwise, keep appending normal data characters
            else {
                // Prevent memory corruption / buffer overflow on the Arduino array
                if (bufferIndex < (MAX_PAYLOAD_SIZE - 1)) {
                    buffin[bufferIndex] = currentByte;
                    bufferIndex++;
                } 
                else {
                    // Error: PC sent a packet larger than MAX_PAYLOAD_SIZE. 
                    // Force a reset of the parser to clear the corrupted state.
                    isReceiving = false;
                    bufferIndex = 0;
                }
            }

        
  }
}
}

void FlashBulb(int duration)
{

  digitalWrite(LED_BUILTIN, HIGH);   // Turn the LED on
  delay(duration);                       // Wait for a second (1000 milliseconds)
  digitalWrite(LED_BUILTIN, LOW);    // Turn the LED off
  delay(duration);  
}

void BulbON()
{

  digitalWrite(LED_BUILTIN, HIGH);   // Turn the LED on
}

void BulbOFF()
{
  digitalWrite(LED_BUILTIN, LOW);    // Turn the LED off
}
