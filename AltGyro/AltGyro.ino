//PIN A5 SCL, A4 SDA
//Using Kalman filter
//Only Pitch/Roll and 
//size of int == 2
//size of int32_t == 4

#include <Arduino.h>
#include <Wire.h>

#include <LiquidCrystal_I2C.h>

#include "SerialOrder.h"
#include "ArduinoReceiver.h"
#include "SerialParameters.h"
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

#define INTERRUPT_PIN 2  // use pin 2 on Arduino Uno & most boards

/*
Specific to yaw angle, will be using MPU6050 class
*/
float YawMeasured,
      YawPitchRoll[3];
      
MPU6050 mpu;

const int MPU = 0x68; 
      
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer 

Quaternion q;           // [w, x, y, z]         quaternion container
VectorFloat gravity;    // [x, y, z]            gravity vector
volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
      

float RateRoll, 
      RatePitch, 
      RateYaw;

float RateCalibrationRoll, 
      RateCalibrationPitch, 
      RateCalibrationYaw,
      CalibrationRoll,
      CalibrationPitch,
      CalibrationAccX,
      CalibrationAccY,
      CalibrationAccZ,
      CalibrationAccAngleX,
      CalibrationAccAngleY,
      CalibrationAccAngleZ,
      CalibratedAccAngleX,
      CalibratedAccAngleY,
      CalibratedAccAngleZ;

int RateCalibrationNumber, 
    ValueCalibration, 
    AccCalibration;

float gyroAngleX, 
      gyroAngleY, 
      CombinedPitch;

float AccX, 
      AccY, 
      AccZ,
      CalibratedAccX,
      CalibratedAccY,
      CalibratedAccZ
      ;
  
float AngleRoll, 
      AnglePitch,
      CaliratedAnglePitch,
      CaliratedAngleRoll;


uint32_t LoopTimer;

int32_t PitchValueTest;

float KalmanAngleRoll=0
      ,KalmanUncertaintyAngleRoll=2*2
      ,KalmanAngleRollCalibrated=0;
      
float KalmanAnglePitch=0 
      ,KalmanUncertaintyAnglePitch=2*2
      ,KalmanAnglePitchCalibrated=0;
      
float Kalman1DOutput[]={0,0};

const int TransferGain=1000;

static unsigned long StartTime = 0,
                     EndTime = 0,
                     ElapsedTime = 0;

SerialOrder OrderReceived;
SerialOrder OrderSent;

LiquidCrystal_I2C lcd(0x27,  16, 2);

/*
Buffer to send data to pc
*/
const int NMAX = 128;
char Buffer[NMAX];

int32_t ValueReadFromBufferPitch,
        ValueReadFromBufferRoll,
        ValueGainTest,
        ValueGainTestRoll;
        
int16_t ValueGain16t;

SerialOrder EnumReadFromBuffer;

int EnumReadFromBufferAsInt,
    EnumReadFromBufferAsIntRoll,
    ValueReadFromBufferAsInt,
    PitchWithGain;

float ValueFromBufferFloatGain,
      ValueToBufferFloatGain;

double ValueDouble;

static bool EnableDisplay = false; //To enable LCD display

const int CalNum = 850;

void setup() {
  Serial.begin(115200);
 if(EnableDisplay)
 {
  lcd.init();
  lcd.backlight(); 

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("CALIBRATING");
 }
  

  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  Wire.setClock(400000);
  Wire.begin();
  delay(250);
  Wire.beginTransmission(0x68); 
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();
  for (RateCalibrationNumber=0; RateCalibrationNumber<CalNum; RateCalibrationNumber ++) 
  {
    ReadGyroValues();
    
    RateCalibrationRoll+=RateRoll;
    RateCalibrationPitch+=RatePitch;
    RateCalibrationYaw+=RateYaw;

    CalibrationAccX+=AccX;
    CalibrationAccY+=AccY;
    CalibrationAccZ+=AccZ;

   if(EnableDisplay)
   {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("CALIBRATING");
    
    lcd.setCursor(0,1);
    lcd.print(RateCalibrationNumber);

    lcd.setCursor(5,1);
    lcd.print("Cal 1");
   }
    
    delay(1);
  }
  RateCalibrationRoll/=CalNum;
  RateCalibrationPitch/=CalNum;
  RateCalibrationYaw/=CalNum;

  CalibrationAccX/=CalNum;
  CalibrationAccY/=CalNum;
  CalibrationAccZ/=CalNum;

  for(int ValCalibration=0; ValCalibration<CalNum; ValCalibration ++)
  {
    ReadGyroValues();
    CalibrationAccAngleX+=AnglePitch;
    CalibrationAccAngleY+=AngleRoll; 

   if(EnableDisplay)
   {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("CALIBRATING");
    
    lcd.setCursor(0,1);
    lcd.print(ValCalibration);

    lcd.setCursor(5,1);
    lcd.print("Cal 2");

    
   }
     
    delay(1);
  }

  CalibrationAccAngleX/=CalNum;
  CalibrationAccAngleY/=CalNum;  

 for (ValueCalibration=0; ValueCalibration<CalNum; ValueCalibration ++) 
  {
    MeasureGyroSetup();
    //UpdateGyroAttitudes();
    CalibrationRoll+=KalmanAngleRoll;
    CalibrationPitch+=KalmanAnglePitch;

   if(EnableDisplay)
   {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("CALIBRATING");
    
    lcd.setCursor(0,1);
    lcd.print(ValueCalibration);

    lcd.setCursor(5,1);
    lcd.print("Cal 3");
   }

    delay(1);
  }
 CalibrationRoll/=CalNum;
 CalibrationPitch/=CalNum;
   
  LoopTimer=micros();

 //SetupYawMeasurement();
 //LCD setup for debug

  
}

void loop() {


  if(Serial.available())
  {
  
  StartTime = millis();
  MeasureGyro();
  MessageGyro();
  EndTime = millis();
  ElapsedTime = EndTime - StartTime;


  if(EnableDisplay)
  {
    DisplayPR_3Acc(); 
  }

  
  }
}


/*
**********Lcd display**************


*/

void DisplayPR_3Acc()
{
  lcd.clear();
  
  /*lcd.setCursor(0,0);
  lcd.print((CalibratedAccX*100));
  lcd.setCursor(0,1);
  lcd.print((CalibratedAccY*100));
  lcd.setCursor(8,0);
  lcd.print((CalibratedAccZ*100));*/

  /*lcd.setCursor(0,0);  
  lcd.print("PC");*/
  
  lcd.setCursor(0,0);
  lcd.print(KalmanAnglePitchCalibrated);

  lcd.setCursor(0,1);
  lcd.print(KalmanAngleRollCalibrated);
  
}

/*
 
********KALMAN FILTER*********

*/

void UpdateKalmanSetup(){

  Kalman1DFilter(KalmanAngleRoll, KalmanUncertaintyAngleRoll, RateRoll, AngleRoll);
  KalmanAngleRoll=Kalman1DOutput[0]; 
  KalmanUncertaintyAngleRoll=Kalman1DOutput[1];
  Kalman1DFilter(KalmanAnglePitch, KalmanUncertaintyAnglePitch, RatePitch, AnglePitch);
  KalmanAnglePitch=Kalman1DOutput[0];                                                                                                                                                                                                                                                                                                                                                        
  KalmanUncertaintyAnglePitch=Kalman1DOutput[1];
  
  }

void UpdateKalman(){

  Kalman1DFilter(KalmanAngleRoll, KalmanUncertaintyAngleRoll, RateRoll, AngleRoll);
  KalmanAngleRoll=Kalman1DOutput[0]; 
  KalmanUncertaintyAngleRoll=Kalman1DOutput[1];
  Kalman1DFilter(KalmanAnglePitch, KalmanUncertaintyAnglePitch, RatePitch, AnglePitch);
  KalmanAnglePitch=Kalman1DOutput[0] ; 
  KalmanUncertaintyAnglePitch=Kalman1DOutput[1];

  KalmanAngleRollCalibrated = KalmanAngleRoll - CalibrationRoll;
  KalmanAnglePitchCalibrated = KalmanAnglePitch - CalibrationPitch;
  KalmanAnglePitchCalibrated = KalmanAnglePitchCalibrated * -1.0;
  
  /*
  if(KalmanAnglePitch<0)
  {
    KalmanAnglePitchCalibrated * -1.0;
  }
  */
   
    
}
  

void Kalman1DFilter(float KalmanState, float KalmanUncertainty, float KalmanInput, float KalmanMeasurement) {
  
  KalmanState=KalmanState+0.004*KalmanInput;
  KalmanUncertainty=KalmanUncertainty + 0.004 * 0.004 * 4 * 4;
  float KalmanGain=KalmanUncertainty * 1/(1*KalmanUncertainty + 3 * 3);
  KalmanState=KalmanState+KalmanGain * (KalmanMeasurement-KalmanState);
  KalmanUncertainty=(1-KalmanGain) * KalmanUncertainty;
  Kalman1DOutput[0]=KalmanState; 
  Kalman1DOutput[1]=KalmanUncertainty;
  
}

/*
 
********READ GYRO VALUES*********

*/

void ReadGyroValues(void) {
  
  Wire.beginTransmission(0x68);
  Wire.write(0x1A);
  Wire.write(0x05);
  Wire.endTransmission();
  Wire.beginTransmission(0x68);
  Wire.write(0x1C);
  Wire.write(0x10);
  Wire.endTransmission();
  Wire.beginTransmission(0x68);
  Wire.write(0x3B);
  Wire.endTransmission(); 
  Wire.requestFrom(0x68,6);
  
  int16_t AccXLSB = Wire.read() << 8 | Wire.read();
  int16_t AccYLSB = Wire.read() << 8 | Wire.read();
  int16_t AccZLSB = Wire.read() << 8 | Wire.read();
  
  Wire.beginTransmission(0x68);
  Wire.write(0x1B); 
  Wire.write(0x8);
  Wire.endTransmission();     
  Wire.beginTransmission(0x68);
  Wire.write(0x43);
  Wire.endTransmission();
  Wire.requestFrom(0x68,6);
  
  int16_t GyroX=Wire.read()<<8 | Wire.read();
  int16_t GyroY=Wire.read()<<8 | Wire.read();
  int16_t GyroZ=Wire.read()<<8 | Wire.read();
  
  RateRoll=(float)GyroX/65.5;
  RatePitch=(float)GyroY/65.5;
  RateYaw=(float)GyroZ/65.5;
  AccX=(float)AccXLSB/4096;
  AccY=(float)AccYLSB/4096;
  AccZ=(float)AccZLSB/4096;
  AngleRoll=atan(AccY/sqrt(AccX*AccX+AccZ*AccZ))*1/(3.142/180);
  AnglePitch=-atan(AccX/sqrt(AccY*AccY+AccZ*AccZ))*1/(3.142/180);

}

void ReadGyroValuesSetup(void) {
  
  Wire.beginTransmission(0x68);
  Wire.write(0x1A);
  Wire.write(0x05);
  Wire.endTransmission();
  Wire.beginTransmission(0x68);
  Wire.write(0x1C);
  Wire.write(0x10);
  Wire.endTransmission();
  Wire.beginTransmission(0x68);
  Wire.write(0x3B);
  Wire.endTransmission(); 
  Wire.requestFrom(0x68,6);
  
  int16_t AccXLSB = Wire.read() << 8 | Wire.read();
  int16_t AccYLSB = Wire.read() << 8 | Wire.read();
  int16_t AccZLSB = Wire.read() << 8 | Wire.read();
  
  Wire.beginTransmission(0x68);
  Wire.write(0x1B); 
  Wire.write(0x8);
  Wire.endTransmission();     
  Wire.beginTransmission(0x68);
  Wire.write(0x43);
  Wire.endTransmission();
  Wire.requestFrom(0x68,6);
  
  int16_t GyroX=Wire.read()<<8 | Wire.read();
  int16_t GyroY=Wire.read()<<8 | Wire.read();
  int16_t GyroZ=Wire.read()<<8 | Wire.read();
  
  RateRoll=(float)GyroX/65.5;
  RatePitch=(float)GyroY/65.5;
  RateYaw=(float)GyroZ/65.5;
  AccX=(float)AccXLSB/4096;
  AccY=(float)AccYLSB/4096;
  AccZ=(float)AccZLSB/4096;
  AngleRoll=atan(AccY/sqrt(AccX*AccX+AccZ*AccZ))*1/(3.142/180);
  AnglePitch=-atan(AccX/sqrt(AccY*AccY+AccZ*AccZ))*1/(3.142/180);

}

void MeasureAccelAngles()
{
  AngleRoll=atan(CalibratedAccY/sqrt(CalibratedAccX*CalibratedAccX+CalibratedAccZ*CalibratedAccZ))*1/(3.142/180);
  AnglePitch=-atan(CalibratedAccX/sqrt(CalibratedAccY*CalibratedAccY+CalibratedAccZ*CalibratedAccZ))*1/(3.142/180);
  
}

void UpdateGyroAttitudes(){
  
  RateRoll-=RateCalibrationRoll;
  RatePitch-=RateCalibrationPitch;
  RateYaw-=RateCalibrationYaw;

  CalibratedAccX = abs(AccX) - abs(CalibrationAccX);
  if(AccX<0)
  CalibratedAccX *= -1.0; 
  
  CalibratedAccY = abs(AccY) - abs(CalibrationAccY);
  if(AccY<0)
  CalibratedAccY *= -1.0; 

  CalibratedAccZ = abs(AccZ) - abs(CalibrationAccZ);
  if(AccZ<0)
  CalibratedAccZ *= -1.0; 

  CalibratedAccAngleX = abs(AnglePitch) - abs(CalibrationAccAngleX);
  if(AnglePitch<0)
  CalibratedAccAngleX *= -1.0; 

  CalibratedAccAngleY = abs(AngleRoll) - abs(CalibrationAccAngleY);
  if(AngleRoll<0)
  CalibratedAccAngleY *= -1.0; 
  
  
}

/*Yaw specific funcs*/

void dmpDataReady() {
    mpuInterrupt = true;
}

void SetupYawMeasurement()
{
    mpu.initialize();
    pinMode(INTERRUPT_PIN, INPUT);

    devStatus = mpu.dmpInitialize();

   
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788); 

    if (devStatus == 0) {
        mpu.CalibrateAccel(60);
        mpu.CalibrateGyro(60);
        mpu.PrintActiveOffsets();
        mpu.setDMPEnabled(true);
        attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();
        dmpReady = true;
        packetSize = mpu.dmpGetFIFOPacketSize();
    } 
}

void MeasureYaw()
{
  if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) {
    
  mpu.dmpGetQuaternion(&q, fifoBuffer);
  mpu.dmpGetGravity(&gravity, &q);
  mpu.dmpGetYawPitchRoll(YawPitchRoll, &q, &gravity);

  YawMeasured =YawPitchRoll[0] * 180/M_PI;

  }
  else
  {
   YawMeasured = -10000; 
  }
}

/*
 
********SERIAL FULCTIONS*********

*/

/*  Serial helpers*/
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

/*  Main serial*/
void MessageGyro(){
  
  if(Serial.available() > 0){
    SerialOrder order_received = read_order();
    OrderReceived=order_received;
     switch(order_received){
      case PC_NOT_READY:
      {
          break;
      }
      case REQUEST_ROLL:
      {
        float TransferValue=KalmanAngleRollCalibrated*TransferGain;
        int32_t ValueToSend=TransferValue;
        OrderSent=MEASURED_ROLL;
        WriteOrder(OrderSent);
        WriteInt32(ValueToSend);
        break;
      }
      case REQUEST_PITCH:
      {
        
        float TransferValue=KalmanAnglePitchCalibrated*TransferGain;
        int32_t ValueToSend=TransferValue;
        //ValueTest = ValueToSend;
        OrderSent=MEASURED_PITCH;
        WriteOrder(OrderSent);
        WriteInt32(ValueToSend);
        break;
      }
      case REQUEST_ACCEL_X:
      {
        float TransferValue=CalibratedAccX*TransferGain;
        int32_t ValueToSend=TransferValue;
        OrderSent=MEASURED_ACCEL_X;
        WriteOrder(OrderSent);
        WriteInt32(ValueToSend);
        break;
      }
      case REQUEST_ACCEL_Y:
      {
        float TransferValue=CalibratedAccY*TransferGain;
        int32_t ValueToSend=TransferValue;
        OrderSent=MEASURED_ACCEL_Y;
        WriteOrder(OrderSent);
        WriteInt32(ValueToSend);
        break;
      }
      case REQUEST_ACCEL_Z:
      {
        float TransferValue=CalibratedAccZ*TransferGain;
        int32_t ValueToSend=TransferValue;
        OrderSent=MEASURED_ACCEL_Z;
        WriteOrder(OrderSent);
        WriteInt32(ValueToSend);
        break;
      }
      case REQUEST_TIME:
      {
        //float TransferValue=CalibratedAccZ*TransferGain;
        int32_t ValueToSend=ElapsedTime;
        OrderSent=MEASURED_TIME;
        WriteOrder(OrderSent);
        WriteInt32(ValueToSend);
        break;
      }
      }
    }
}

/*

MAIN FUNCTIONS

*/

void MeasureGyro(){

  ReadGyroValues();
  UpdateGyroAttitudes();
  UpdateKalman();
  
}

void MeasureGyroSetup(){
  
  ReadGyroValuesSetup();
  UpdateGyroAttitudes();
  //MeasureAccelAngles();
  UpdateKalmanSetup();
  
}
