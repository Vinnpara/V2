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

float RateRoll, RatePitch, RateYaw;
float RateCalibrationRoll, RateCalibrationPitch, RateCalibrationYaw;
int RateCalibrationNumber;
float elapsedTime, currentTime, previousTime;
float gyroAngleX, gyroAngleY, CombinedPitch;

float AccX, AccY, AccZ;
float AngleRoll, AnglePitch;
uint32_t LoopTimer;
int32_t PitchValueTest;

float KalmanAngleRoll=0, KalmanUncertaintyAngleRoll=2*2;
float KalmanAnglePitch=0, KalmanUncertaintyAnglePitch=2*2;
float Kalman1DOutput[]={0,0};

const int TransferGain=1000;

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
  for (RateCalibrationNumber=0; RateCalibrationNumber<2000; RateCalibrationNumber ++) {
    ReadGyroValues();
    RateCalibrationRoll+=RateRoll;
    RateCalibrationPitch+=RatePitch;
    RateCalibrationYaw+=RateYaw;
    delay(1);
  }
  RateCalibrationRoll/=2000;
  RateCalibrationPitch/=2000;
  RateCalibrationYaw/=2000;
  LoopTimer=micros();

 /*
 LCD setup for debug
 */
  //initialize lcd screen
  lcd.init();
  // turn on the backlight
  lcd.backlight();
  
}

void loop() {
  if(Serial.available()){
  //ReadGyroValues();
  //UpdateGyroAttitudes();
  //UpdateKalman();

  MeasureGyro();
  MessageGyro();
  
  //float TransferValue=KalmanAnglePitch*TransferGain;
  //int16_t ValueToSend=(int16_t)TransferValue;
  //PitchValueTest = ValueToSend;

  //MessageBuffer();
  //ReadFromBuffer();
  
  /*Serial.print(" Kalman Roll [°] ");
  Serial.print(KalmanAngleRoll);
  Serial.print(" Kalman Pitch Angle [°] ");
  Serial.println(KalmanAnglePitch);*/
  
  /*lcd.clear();
 
  lcd.setCursor(0,0);
  lcd.print("P");
  lcd.setCursor(0,1);
  lcd.print(KalmanAnglePitch);

  lcd.setCursor(5,0);
  lcd.print(EnumReadFromBufferAsInt);
  lcd.setCursor(5,1);
  lcd.print(ValueReadFromBufferPitch);

  lcd.setCursor(10,0);
  lcd.print(EnumReadFromBufferAsIntRoll);
  lcd.setCursor(10,1);
  lcd.print(ValueReadFromBufferPitch);

  delay(75);*/
  
  //while (micros() - LoopTimer < 4000);
  //LoopTimer=micros();
  }
}

///KALMAN FILTER/////

void UpdateKalman(){

  Kalman1DFilter(KalmanAngleRoll, KalmanUncertaintyAngleRoll, RateRoll, AngleRoll);
  KalmanAngleRoll=Kalman1DOutput[0]; 
  KalmanUncertaintyAngleRoll=Kalman1DOutput[1];
  Kalman1DFilter(KalmanAnglePitch, KalmanUncertaintyAnglePitch, RatePitch, AnglePitch);
  KalmanAnglePitch=Kalman1DOutput[0]; 
  KalmanUncertaintyAnglePitch=Kalman1DOutput[1];
  
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

void UpdateGyroAttitudes(){
  
  RateRoll-=RateCalibrationRoll;
  RatePitch-=RateCalibrationPitch;
  RateYaw-=RateCalibrationYaw;
  
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
        float TransferValue=KalmanAngleRoll*TransferGain;
        int32_t ValueToSend=TransferValue;
        OrderSent=MEASURED_ROLL;
        WriteOrder(OrderSent);
        WriteInt32(ValueToSend);
        break;
      }
      case REQUEST_PITCH:
      {
        
        float TransferValue=KalmanAnglePitch*TransferGain;
        int32_t ValueToSend=TransferValue;
        //ValueTest = ValueToSend;
        OrderSent=MEASURED_PITCH;
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
