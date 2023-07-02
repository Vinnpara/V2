#include <Arduino.h>
#include<Servo.h> 

#include "SerialOrder.h"
#include "ArduinoReceiver.h"
#include "SerialParameters.h"

#define echoPin 4 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 5

long duration;
int distance, ServoRightValue, ServoLeftValue;
float MeasuredDistance;
int16_t MeasuredDistance_int16, ServoAngleInt16, ProgramIterationTime16;
unsigned long Time1, Time2, Time3, IterationTime, TimeoutValue, ProgramIterationTime;
unsigned int DelayTime;

int8_t Buffer16Int[2];


Servo serv, serv1;
float pos_2;

void setup() {
  // put your setup code here, to run once:
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication
  
  TimeoutValue=20000; //20k microsecons so max range of about 3m
  serv.attach(10);
  serv1.attach(9);
  
}

void loop() {  
  RadarSweep(171,-1);
  //SteeringSweep(105,10);
  //RadarSweepMechanical(170,-1);
}

void SteeringSweep(int MaxRightValue, int MaxLeftValue){
    for(ServoRightValue=0; ServoRightValue <= MaxRightValue; ServoRightValue++){
    //PingUltraSoundSensor();
    serv1.write(ServoRightValue);
    Serial.print("Servo angle Written: ");
    Serial.println(ServoRightValue);
    }
    
    for(ServoLeftValue=MaxRightValue; ServoLeftValue >= MaxLeftValue; ServoLeftValue--){
    //PingUltraSoundSensor();
    serv1.write(ServoLeftValue);
    Serial.print("Servo angle Written: ");
    Serial.println(ServoLeftValue);
    }  
  
  }

void RadarSweep(int MaxRightValue, int MaxLeftValue){
  
    for(ServoRightValue=0; ServoRightValue < MaxRightValue; ServoRightValue++){
    PingUltraSoundSensor();
    serv.write(ServoRightValue);
    //Serial.print("Servo angle Written: ");
    //Serial.println(ServoRightValue);
    }
    
    for(ServoLeftValue=MaxRightValue; ServoLeftValue > MaxLeftValue; ServoLeftValue--){
    PingUltraSoundSensor();
    serv.write(ServoLeftValue);
    //Serial.print("Servo angle Written: ");
    //Serial.println(ServoLeftValue);
    }    
  
}

void RadarSweepMechanical(int MaxRightValue, int MaxLeftValue){
  
    for(ServoRightValue=0; ServoRightValue < 170; ServoRightValue++){
    //PingUltraSoundSensor();
    serv.write(ServoRightValue);
    //delay(10);
    Serial.print("Servo angle Written: ");
    Serial.println(ServoRightValue);
    }
    
    for(ServoLeftValue=170; ServoLeftValue > -1; ServoLeftValue--){
    //PingUltraSoundSensor();
    serv.write(ServoLeftValue);
    //delay(10);
    Serial.print("Servo angle Written: ");
    Serial.println(ServoLeftValue);
    }    
  
}
   
void PingUltraSoundSensor(){
  
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
  
  if(IterationTime >= 13)
  MeasuredDistance_int16 = 200;
  else
  MeasuredDistance_int16 = duration * 0.034 / 2;

  DelayTime=25-IterationTime;
  
  delay(DelayTime);
  
  //Serial.print("Distance: ");
  //Serial.println(MeasuredDistance_int16);
  //Serial.print("Duration: ");
  //Serial.println(duration);

  ServoAngleInt16=serv.read();
  
  //Serial.print("Servo angle: ");
  //Serial.println(ServoAngleInt16);
  
  //Serial.print("Iteration Time ");
  //Serial.println(ProgramIterationTime);

  WriteCommandInt16(RADAR_DISTANCE,MeasuredDistance_int16);
  WriteCommandInt16(RADAR_POSITION,ServoAngleInt16);
  
  Time3=millis();
  ProgramIterationTime16= Time3-Time1;
  
  //WriteCommandInt16(ARD_ITERATION_TIME,ProgramIterationTime16);
  //WriteInt16_2Command(RADAR_DISTANCE_2, MeasuredDistance_int16, RADAR_POSITION_2, ServoAngleInt16);
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
  
