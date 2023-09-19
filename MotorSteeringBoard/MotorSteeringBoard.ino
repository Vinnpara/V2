#include<Servo.h> // include server library
#include <Arduino.h>
#define enA 8
#define in1 6
#define in2 7

#include <LiquidCrystal.h>
#define echoPin 4 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 5

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include "SerialOrder.h"
#include "ArduinoReceiver.h"
#include "SerialParameters.h"

SerialOrder OrderReceived;
SerialOrder OrderReceived2;

int Contrast = 70;
Servo ser, ser1; // create servo object to control a servo
int poser, pos_2; // initial position of server pos_2 for the second servo
int val; // initial value of input
int pos_lim;

long duration;
int distance, inches;

int8_t SteerRawCommand, ThrottleLever;

int MAX_ANGLE = 106;
int MIN_ANGLE = 10;

int motor_speed=0;
int i_r, i_l, Command=0, SteerVal=0;
int8_t ZeroSteer=58;

const int BUFFER_SIZE = 50;
char SteerBuff[BUFFER_SIZE];

//LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
LiquidCrystal_I2C Scrn(0x27,16,2);

void setup() {
  // put your setup code here, to run once:

Serial.begin(9600); // Serial comm begin at 9600bps
ser.attach(9);// servo is connected at pin 9 
//ser1.attach(10); //Servo 2 at pin 10 

pinMode(enA, OUTPUT);
pinMode(in1, OUTPUT);
pinMode(in2, OUTPUT);

// Set initial rotation direction
digitalWrite(in1, LOW);
digitalWrite(in2, HIGH);

pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);

 
  
Scrn.init();
Scrn.backlight();
  // Print a message to the LCD.

poser=44;
ZeroSteering();
}



void loop() {
  // put your main code here, to run repeatedly:
  /*lcd.setCursor(0, 1);
  lcd.print("OR");
  lcd.setCursor(1, 1);*/
  //lcd.print(OrderReceived);

  /*lcd.setCursor(0, 4);
  lcd.print("SPD");
  lcd.setCursor(1, 4);*/
  //lcd.print(SteerRawCommand);
  //TestSweepSteering();

if (Serial.available()){

  RequestCommand(REQUEST_STEER);
  //ReadSerial();
  ReadSerial2();
  //ReadDirectSerial();
  //SerialTest();
  //SerialSteeringControl();
  //TestSweepSteering();

  /*Scrn.clear();
  
  Scrn.setCursor(0, 0);
  Scrn.print("OR");

  Scrn.setCursor(5, 0);
  Scrn.print(OrderReceived);

  Scrn.setCursor(8, 0);
  Scrn.print(OrderReceived2);
  
  Scrn.setCursor(0, 1);
  Scrn.print("COM");

  Scrn.setCursor(5, 1);
  Scrn.print(SteerRawCommand);

  Scrn.setCursor(8, 1);
  Scrn.print(motor_speed);*/

}

}
////******END OF MAIN LOOP********///////
/*

list of funtions



*/
///SERIAL SEND RECEIVE/////

void SerialTest(){
   int8_t Num=10;
   int8_t Buff[1]={(int8_t) (Num & 0xff)};
   int8_t SteerRawCommand =(int8_t) static_cast<signed char>(Buff[0]);
   Serial.println(" Serial Test  ");
   Serial.print(SteerRawCommand);
  }

void WriteOrder(enum SerialOrder CommandOrder)
{
  uint8_t* Order = (uint8_t*) &CommandOrder;
  Serial.write(Order, sizeof(uint8_t));
}

void Write8InCommand(uint8_t Command)
{
  uint8_t* Order = (uint8_t*) &Command;
  Serial.write(Order, sizeof(uint8_t));
}

void RequestCommand (enum SerialOrder CommandOrder){

  uint8_t* Order = (uint8_t*) &CommandOrder;
  Serial.write(Order, sizeof(uint8_t));
  
  }

enum SerialOrder read_order()
{
  return (SerialOrder) Serial.read();
}

void ReadForSerial(enum SerialOrder CommandOrder){
  
  SerialOrder Command = (SerialOrder) Serial.read();
  
  }

void ReadDirectSerial(){
  
  
SteerRawCommand =(int8_t) static_cast<signed char>(Serial.read());
  
  
  }

void ReadSerial(enum SerialOrder RequestedOrder){
//Requested command - expected command =10
//ie REQUEST_STEER(23) - STEER_COMMAND(13) =10

  OrderReceived = read_order();
  Command=(int)OrderReceived;
    
    switch(OrderReceived){
      
     case PC_NOT_READY:
      {    
        
        break;
      }
      case STEER_COMMAND:
      {
        //SteerRawCommand =(int8_t)Serial.read();
        //int sent=Serial.readBytes(SteerBuff, sizeof(int8_t));
        //SteerRawCommand =(int8_t) static_cast<signed char>(Serial.read());
        //SteerRawCommand =(int8_t)(SteerBuff[0]);
        //SteerRawCommand =55;
        //SteerVal=(int)SteerRawCommand;
        //SteerRawCommand =(int)Serial.read();        
        //SerialSteeringControl();
        //SendCommandReceived(STEER_COMMAND,SteerRawCommand);
        break;
      }
    
    }
  
  
   
}

void ReadSerial(){

    int sent=Serial.readBytes(SteerBuff, 4*sizeof(int8_t));

    OrderReceived=(SerialOrder)SteerBuff[0];
    OrderReceived2=(SerialOrder)SteerBuff[2];
 
    //SerialOrder order_received = read_order();
    //OrderReceived=order_received;
    //SteerRawCommand =(int8_t) static_cast<signed char>(SteerBuff[0]);
    //SteerRawCommand =(int8_t)Serial.read();
    
    switch(OrderReceived){
      
     case PC_NOT_READY:
      {    
        
        break;
      }
      case STEER_COMMAND:
      {
        //SteerRawCommand = Serial.read();
        //int sent=Serial.readBytes(SteerBuff, sizeof(int8_t));
        //SteerRawCommand =(int8_t) static_cast<signed char>(SteerBuff[0]);
        //SteerRawCommand =10;
        //SerialSteeringControl();
        SteerRawCommand=(int8_t)SteerBuff[1];
        //SerialSteeringControl();
        //SendCommandReceived(STEER_COMMAND,SteerRawCommand);
        break;
      }
      case MOTOR_SPEED:
      {
        //SteerRawCommand = Serial.read();
        //int sent=Serial.readBytes(SteerBuff, sizeof(int8_t));
        //SteerRawCommand =(int8_t) static_cast<signed char>(SteerBuff[0]);
        //SteerRawCommand =10;
        //SerialSteeringControl();
        ThrottleLever=(int8_t)SteerBuff[3];
        motor_speed=ConvertValue(ThrottleLever,3.55f,0);
        //DriveMotorForward();
        //SerialSteeringControl();
        //SendCommandReceived(STEER_COMMAND,SteerRawCommand);
        break;
      }
      
      }

}

void ReadSerial2(){
  int sent=Serial.readBytes(SteerBuff, 4*sizeof(int8_t));

    OrderReceived=(SerialOrder)SteerBuff[0];
    OrderReceived2=(SerialOrder)SteerBuff[2];

   if(STEER_COMMAND==OrderReceived){
    SteerRawCommand=(int8_t)SteerBuff[1];
    SerialSteeringControl();
    }
   if(MOTOR_SPEED==OrderReceived2){
    ThrottleLever=(int8_t)SteerBuff[3];
    motor_speed=ConvertValue(ThrottleLever,3.55f,0);
    DriveMotorForward();
    }
    
  
  }

void SendCommandReceived(enum SerialOrder CommandOrder, int8_t Value){
  
  WriteOrder(CommandOrder);
  Write8InCommand(Value);
  
  }

int position_err(int val, int max, int min)
{
  int val_lim;

  if(val >= max)
  {  
   val_lim=max; 
  }
  else if(val <= min)
  {
   val_lim=min;
  }
  else
  {
   val_lim=val;  
  }
  
  return val_lim;
  
}

//STEERING & MOTOR FUNCTIONS
int ConvertValue(int8_t ValX, float m, int C){
  
 int ConvertedVal = (m*ValX) +  C;

  return ConvertedVal;
}

void TestSweepSteering(){
  
  for(int8_t i=10;i<107;i++){
     ser.write(i);// the servo will move according to position
      delay(100);//delay for the servo to get to the position
  }
  for(int8_t i=106;i>9;i--){
     ser.write(i);// the servo will move according to position
      delay(100);//delay for the servo to get to the position
  }
  
  }

void ZeroSteering(){
  
     ser.write(position_err(ZeroSteer,MAX_ANGLE, MIN_ANGLE));// the servo will move according to position
     //delay(15);//delay for the servo to get to the position
  
  }

void SerialSteeringControl(){
      
      ser.write(position_err((int)SteerRawCommand,MAX_ANGLE, MIN_ANGLE));// the servo will move according to position
      //ser.write(position_err(10,MAX_ANGLE, MIN_ANGLE));// the servo will move according to position
      //delay(15);//delay for the servo to get to the position
  
  }

void DriveMotorForward(){
      
      analogWrite(enA, motor_speed);
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
  
  }  

void ManualControl(){
  
    if (val == 'c') //if value input is equals to c Center steering command

    {
      //center steering
      poser = 44; //0 Steering
      //poser=position_err(poser,MAX_ANGLE, MIN_ANGLE );
      ser.write(poser);// the servo will move according to position
      delay(15);//delay for the servo to get to the position

    }

    if (val == 'd') //if value input is equals to d

    {
    //steer right
    poser += 2; //than position of servo motor increases by 1 ( anti clockwise)
    //poser=position_err(poser,MAX_ANGLE, MIN_ANGLE );
    ser.write(poser);// the servo will move according to position
    delay(15);//delay for the servo to get to the position
      
    }

   if (val == 'a') //if value input is equals to a

    {
    //steer left
    poser -= 2; //than position of servo motor decreases by 1 (clockwise)
    //poser=position_err(poser,MAX_ANGLE, MIN_ANGLE );
    ser.write(poser);// the servo will move according to position
    delay(15);//delay for the servo to get to the position
    
    }

      
     
      if(val=='w'){//mptor fwd
        
      motor_speed=355;
      analogWrite(enA, motor_speed);
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
        
      }

     if(val=='t'){//motor increment
        
      motor_speed+=5;
      analogWrite(enA, motor_speed);
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
        
      }

      if(val=='s'){//motor back
        
      motor_speed=355;
      analogWrite(enA, motor_speed);
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);      
  
      }

      if(val=='v'){//motor stop
        
      motor_speed=0;
      analogWrite(enA, motor_speed);
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
        
      }
   

/*Serial.print("\nSteer Value read ");
Serial.print(poser);

Serial.print("\nmotor Value read ");
Serial.print(motor_speed);*/

//Serial.print("\nservo Value read pos_2 ");
//Serial.print(pos_2);

  
  }
