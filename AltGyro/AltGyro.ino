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

void setup() {

 
  
  Serial.begin(115200);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  Wire.setClock(400000);
  Wire.begin();
  delay(250);
  Wire.beginTransmission(0x68); 
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();
  for (RateCalibrationNumber=0; RateCalibrationNumber<2500; RateCalibrationNumber ++) 
  {
    ReadGyroValues();
    RateCalibrationRoll+=RateRoll;
    RateCalibrationPitch+=RatePitch;
    RateCalibrationYaw+=RateYaw;
    delay(1);
  }
  RateCalibrationRoll/=2500;
  RateCalibrationPitch/=2500;
  RateCalibrationYaw/=2500;

  for (AccCalibration=0; AccCalibration<2500; AccCalibration ++) 
  {
    ReadGyroValues();
    CalibrationAccX+=AccX;
    CalibrationAccY+=AccY;
    CalibrationAccZ+=AccZ;

    CalibrationAccAngleX+=AnglePitch;
    CalibrationAccAngleY+=AngleRoll; 
     
    delay(1);
  }
  CalibrationAccX/=2500;
  CalibrationAccY/=2500;
  CalibrationAccZ/=2500;

  for(int ValCalibration=0; ValCalibration<2500; ValCalibration ++)
  {
    ReadGyroValues();
    CalibrationAccAngleX+=AnglePitch;
    CalibrationAccAngleY+=AngleRoll; 
     
    delay(1);
  }

  CalibrationAccAngleX/=2500;
  CalibrationAccAngleY/=2500;  

 for (ValueCalibration=0; ValueCalibration<2500; ValueCalibration ++) 
  {
    MeasureGyroSetup();
    //UpdateGyroAttitudes();
    CalibrationRoll+=KalmanAngleRoll;
    CalibrationPitch+=KalmanAnglePitch;

    delay(1);
  }
 CalibrationRoll/=2500;
 CalibrationPitch/=2500;
   
  LoopTimer=micros();

 
 //LCD setup for debug
 
  //initialize lcd screen
  lcd.init();
  // turn on the backlight
  lcd.backlight(); 
  
}

void loop() {
 
  /*lcd.setCursor(0,0);
  lcd.print((AccX*10000));
  lcd.setCursor(0,1);
  lcd.print((AccY*10000));
  
  MeasureGyro();
  lcd.clear();
  
  lcd.setCursor(12,0);
  lcd.print((AccZ*10000));*/
  /**lcd.setCursor(0,0);
  lcd.print("PC");
  lcd.setCursor(0,1);
  lcd.print(KalmanAnglePitchCalibrated);

  lcd.setCursor(10,0);
  lcd.print("RC");
  lcd.setCursor(10,1);
  lcd.print(KalmanAngleRollCalibrated);*/
  //lcd.setCursor(7,1);
  //lcd.print(AccY);
  
  //delay(5);
  
  if(Serial.available()){
  //ReadGyroValues();
  //UpdateGyroAttitudes();
  //UpdateKalman();
  /*lcd.clear();

  lcd.setCursor(0,0);
  lcd.print((AccX*1000));
  lcd.setCursor(0,1);
  lcd.print((AccY*1000));
  lcd.setCursor(9,1);
  lcd.print((AccY*1000));

  delay(5);*/

  StartTime = millis();

  MeasureGyro();
  MessageGyro();

  EndTime = millis();
  
  ElapsedTime = EndTime - StartTime;
  
  //float TransferValue=KalmanAnglePitch*TransferGain;
  //int16_t ValueToSend=(int16_t)TransferValue;
  //PitchValueTest = ValueToSend;

  //MessageBuffer();
  //ReadFromBuffer();
  /*lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(ElapsedTime);*/
  //lcd.setCursor(0,1);
  //lcd.print(KalmanAnglePitch

  /*lcd.setCursor(3,0);
  lcd.print("R");
  lcd.setCursor(3,1);
  lcd.print(KalmanAngleRoll);*/
  
  /*lcd.clear();
  lcd.setCursor(0,0);
  lcd.print((AccX*1000));
  lcd.setCursor(0,1);
  lcd.print((AccY*1000));
  lcd.setCursor(9,1);
  lcd.print((AccY*1000));

  //lcd.setCursor(6,0);
  //lcd.print((AccZ*1000));


  delay(5);*/
  
  //while (micros() - LoopTimer < 4000);
  //LoopTimer=micros();
  }
}

///KALMAN FILTER/////

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

///READ GYRO VALUES/////

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

///SERIAL SEND RECEIVE/////
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

///MAIN FUNCTIONS/////

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

void MessageBuffer(){
  
  int OrderAsInt = (int)MEASURED_PITCH;
  int OrderAsIntRoll = (int)MEASURED_ROLL;
  
  int ValueAsInt = (int)(PitchValueTest * 10000);

  ValueToBufferFloatGain = KalmanAnglePitch * 1000;
  PitchWithGain = (int)(PitchValueTest * 10000);

  ValueGainTest = (int32_t)(KalmanAnglePitch * 1000);
  ValueGainTestRoll = (int32_t)(KalmanAngleRoll * 1000);

  int16_t TestGain = (int16_t)(PitchValueTest);


  /*char *BuffPointer, *BuffPointer2;
  SerialOrder *OrderPointerPitch;
  int32_t *PitchValuePointer;
  
  BuffPointer2 = Buffer;
  BuffPointer = BuffPointer2;
  
  OrderPointerPitch = (int *)BuffPointer;
  *BuffPointer = MEASURED_PITCH;
  BuffPointer += sizeof(int);
  
  PitchValuePointer = (int32_t *)BuffPointer;
  *BuffPointer = PitchValueTest;*/

  char *BuffPointer, *BuffPointer2;
  int *OrderPointerPitch,
      *OrderPointerRoll;
      
  int32_t *PitchValuePointer,
          *RollValuePointer;
  
  BuffPointer2 = Buffer;
  BuffPointer = BuffPointer2;
  
  OrderPointerPitch = (int *)BuffPointer;
  *OrderPointerPitch = OrderAsInt;
  BuffPointer += sizeof(int);
  
  PitchValuePointer = (int32_t *)BuffPointer;
  *PitchValuePointer = ValueGainTest;
  BuffPointer += sizeof(int32_t);

  OrderPointerRoll = (int *)BuffPointer;
  *OrderPointerRoll = OrderAsIntRoll;
  BuffPointer += sizeof(int);
  
  RollValuePointer = (int32_t *)BuffPointer;
  *RollValuePointer = ValueGainTestRoll;

  Serial.write(Buffer, 12);
}

void ReadFromBuffer(){
  
 char *BuffPointer, *BuffPointer2;
 int *OrderPointerPitch,
     *OrderPointerRoll;
 int32_t *PitchValuePointer,
         *RollValuePointer; 

 BuffPointer2 = Buffer;
 BuffPointer = BuffPointer2; 

 OrderPointerPitch = (int *)BuffPointer;
 EnumReadFromBufferAsInt = *OrderPointerPitch;
 BuffPointer += sizeof(int);

 PitchValuePointer = (int32_t *)BuffPointer;
 ValueReadFromBufferPitch = *PitchValuePointer;
 BuffPointer += sizeof(int32_t);

 OrderPointerRoll = (int *)BuffPointer;
 EnumReadFromBufferAsIntRoll = *OrderPointerRoll;
 BuffPointer += sizeof(int);

 RollValuePointer = (int32_t *)BuffPointer;
 ValueReadFromBufferRoll = *RollValuePointer; 
 
  
}
