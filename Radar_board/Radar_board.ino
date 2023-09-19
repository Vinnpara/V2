
#include<Servo.h> 
#include <Arduino.h>
#define echoPin 4 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 5
#define MAX_RADAR_ANGLE 170

#include "SerialOrder.h"
#include "ArduinoReceiver.h"
#include "SerialParameters.h"

long duration;
int distance, ServoRightValue, ServoLeftValue;
float MeasuredDistance;
unsigned long Time1, Time2, Time3, IterationTime, TimeoutValue, ProgramIterationTime;
unsigned int DelayTime;
int8_t MeasuredDistance_int8;
int16_t ServoPos;
Servo serv;
float pos_2;
static bool PC_not_Ready=1;
int8_t Buffer16Int[2];
int16_t MeasuredDistance_int16, ServoAngleInt16;
SerialOrder OrderReceived;
SerialOrder OrderSent;

void setup() {
  // put your setup code here, to run once:
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication
  
  TimeoutValue=20000; //20k microsecons so max range of about 3m
  serv.attach(10);
}

void loop() {
  // put your main code here, to run repeatedly:
 if(Serial.available()>0){
  RadarSweep();
 }
}

void RadarSweep(){
  
    for(ServoRightValue=0; ServoRightValue < 170; ServoRightValue++){
    PingUltraSoundSensor();
    serv.write(ServoRightValue);
    ServoAngleInt16=ServoRightValue;
    ReadSerial();
    }
    
    for(ServoLeftValue=170; ServoLeftValue > -1; ServoLeftValue--){
    PingUltraSoundSensor();
    serv.write(ServoLeftValue);
    ServoAngleInt16=ServoLeftValue;
    ReadSerial();
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
  // Calculating the distance
  //delay(20);
  IterationTime=Time2-Time1;
  if(IterationTime >= 25)
  MeasuredDistance = 200;
  else
  MeasuredDistance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  
  DelayTime=25-IterationTime;
  delay(DelayTime);

  MeasuredDistance_int16=(int16_t)MeasuredDistance; 
  
  /*Serial.print("Distance: ");
  Serial.println(MeasuredDistance_int16);
  Serial.print("Position: ");
  Serial.println(ServoAngleInt16);*/
  
  Time3=millis();
  
  //Serial.print("Position ");
  ProgramIterationTime= Time3-Time1;
  //Serial.println(ServoPos);
  


  //ReadSerial();
  
  }

///SERIAL SEND RECEIVE/////

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

enum SerialOrder read_order()
{
  return (SerialOrder) Serial.read();
}  

void ReadSerial(){

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
        //int16_t ValuetoSend=MeasuredRadarValues[i];
        //WriteCommandInt16(RADAR_DISTANCE, ValuetoSend);
        
        WriteOrder(RADAR_DISTANCE);
        WriteInt16(MeasuredDistance_int16);
        
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
 
