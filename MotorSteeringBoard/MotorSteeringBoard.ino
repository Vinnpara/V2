/*

     VEHICHLE FRONT
     
           /\
          /  \
         /    \motor_speed
           ||
           
          USER


[Motor Left]   [Motor right]
  
*/

#include<Servo.h> // include server library
#include <Arduino.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define enA 5
#define in1 6
#define in2 7

#define enB 11
#define in3 2
#define in4 3

#define MAX_PAYLOAD_SIZE 20
#define MAX_PACKET_SIZE 64

#include <LiquidCrystal.h>

#include <Wire.h>

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#include <EEPROM.h>

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C Scrn(0x27,16,2);

#include "SerialOrder.h"
#include "ArduinoReceiver.h"
#include "SerialParameters.h"

SerialOrder OrderReceived;
SerialOrder OrderReceived2;
SerialOrder OrderReceived3;

int Contrast = 70;
Servo ser, ser1; // create servo object to control a servo
int poser, pos_2; // initial position of server pos_2 for the second servo
int val; // initial value of input
int pos_lim;

long duration;
int distance, inches;

int8_t SteerRawCommand, ThrottleLever, ThrottleLever2;
int SteerCommandInt;

int ServoCommand;

int MAX_ANGLE = 180;
int MIN_ANGLE = 0;

int motor_speed=0;
int motor_speed_rev=0;

//---------------------ARDUINO_NAME-------------------------
//-----------------------MSB--------------------------------
// Give this specific board a unique name (Max 20 characters)
//Adjust the define accordingly.

# define NAME_BUFF_SIZE 5
const char uniqueName[NAME_BUFF_SIZE] = "MSB"; 

static char uniqueNameRead[NAME_BUFF_SIZE] = "MSB"; 

int i_r, i_l, Command=0, SteerVal=0;
int8_t ZeroSteer=58;

SerialOrder OrderReceivedRad, OrderSentRad;
static bool PC_not_Ready=1,
            PC_Ready = 0,
            CommsEstablished =0,
            ReStartSerial = 0,
            SerialFlushed = 0,
            ValidHeartBeatPC = 0,
            StartFound = 0;

volatile bool newDataReceived = false;            

unsigned long previousMillis = 0,
              previousMillis2 = 0,
              previousMillis3 = 0,
              previousMillis4 = 0,
              lastValidMsgTime = 0,
              lastRequestTime = 0,
              SerialOffTime = 0,
              BoardNameReq = 0,
              requestSentTime = 0,
              roundTrip = 0;              
               
const long interval = 50,
           IntervalPCHBMonitoring = 1500,
           IntervalSerialMonitoring = 60,
           IntervalDisplayMonitoring = 250,
           IntervalSerialOff = 1500; // Interval in milliseconds

const char START_MARKER = '[',
           END_MARKER = ']';

char DataPacketReceived[MAX_PACKET_SIZE],
     buffin[MAX_PAYLOAD_SIZE];

static int ValidPCHeartBeatCounter = 0,
           ValidPCReadyCounter =0,
           j = 0, //data added
           k = 0; //Steer buff counter

static int  IndexAtStart = 0,
            IndexAtEnd = 0,
            ValidPckts = 0,
            PcktSz = 0,
            PacektsRecieved = 0,
            ValidSteerCommand = 0,
            ValidMotorLeft = 0,
            ValidMotorRight = 0;

static  bool StartValid = 0,
             EndValid =0,
             SendingBoardName =0,
             MotorSteerSetupComplete =0,
             InitDelayTimer = 0,
             DelayStart =0,
             BoardFoundByPC = 0;

static char BuffInComing[20]; //11 works for the STR only command
static uint8_t idx = 0;
static bool inMessage = false;


static unsigned long loopCount = 0;
static unsigned long lastRateCheck = 0;

unsigned long rttSum = 0, rttCount = 0, rttMax = 0, rttAvg = 0;
unsigned long lastRttReport = 0;

void setup() {
  // put your setup code here, to run once:
pinMode(LED_BUILTIN, OUTPUT);

Serial.begin(115200); // Serial comm begin at 9600bps
/*
 while (!Serial) {
    ; // Wait for the serial port to connect (needed for native USB boards, good practice)
  }
*/

//WRITING INTO EPROM,ONLY USE THIS **************
//ONE TIME TO CHANGE THE NAME********************

///////////TO BE USED ONE TIME\\\\\\\\\\\\\\\\\
********DEVICE NAME HAS BEEN UPDATED*************
//^^^^CHANGE STATUS ONCE NAME UPDATED^^^^^\\\\\\
/*
 for (int i = 0; i < NAME_BUFF_SIZE; i++) 
 {
 
    EEPROM.write(i, uniqueName[i]);
 }
 
 for (int i = 0; i < NAME_BUFF_SIZE; i++) 
 {
 
    uniqueNameRead[i] = EEPROM.read(i);
 }
//To check name change

 
///////////TO BE USED ONE TIME\\\\\\\\\\\\\\\\\

///////////TO BE USED ONE TIME\\\\\\\\\\\\\\\\\

//WRITING INTO EPROM,ONLY USE THIS **************
//ONE TIME TO CHANGE THE NAME********************
/*
 * 
 */

 
/*
OLED screen
*/ 
// 0x3C is default i2c adress in some cases MAY be different
display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
display.clearDisplay();
display.setTextSize(2);
display.setTextColor(WHITE);

}



void loop() {
/* to determin execution speed
 * ensure to manually set any 
 * serial condition cases to true to simulate
 * sending to PC   (currently 457-458 Hz)*/
/* 
loopCount++;
if (millis() - lastRateCheck >= 1000)
{
  Serial.print("Loop Hz: ");
  Serial.println(loopCount);
  loopCount = 0;
  lastRateCheck = millis();
}*/

ReadAndParseDataSTD();


if(!MotorSteerSetupComplete)
{
  MotorServosSetup();
  MotorSteerSetupComplete = 1;
}
 
MonitorPCHeartBeat();
MonitorPCReadiness();
DeterminePCReadiness();

if(!BoardFoundByPC)
{
  SendBoardName();
}

if(SendingBoardName) //SendingBoardName
{
  
  SendBoardName();

  if (millis() - BoardNameReq > 1000) //750 was also acceptable  
  {
     DelayStart =1;
  }

  if(DelayStart && BoardFoundByPC) //DelayStart && BoardFoundByPC
  {
    SignalArdReady();
    SendDataPacketHB();
  }
}


if (CommsEstablished && SendingBoardName) //CommsEstablished && SendingBoardName
{
  
  //ServiceSerialLink();
  SignalArdCommsEst();

  //BulbON();
}
else
{
 //BulbOFF();

  if(PacektsRecieved > 0)
    PacektsRecieved =0;
 
}

if(BoardFoundByPC)
 BulbON();
else
 BulbOFF();

if (millis() - lastRttReport > 2000 && rttCount > 0)
{
  //Serial.print("avgRTT:");
  rttAvg = rttSum / rttCount;
  //Serial.print(" maxRTT:");
  //Serial.println(rttMax);
  rttSum = 0; rttCount = 0; rttMax = 0;
  lastRttReport = millis();
}


UpdateDisp();
}

////******END OF MAIN LOOP********///////


  ////_______________\\\\
 ////*****************\\\\
////  ESTABLISH COMMS  \\\\
\\\\*******************////

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

void MonitorPCReadiness()
{
//A valid PC heartbeat and PC ready flag will trigger the CommsEstablished flag
//If there is a vaild CommsEstablished and we loose PC heart beat, commsestablished is false

  unsigned long currentMillis = millis();
  // Check if the interval has passed
  if (currentMillis - previousMillis3 >= IntervalPCHBMonitoring) 
  {
    previousMillis3 = currentMillis; // Save the last time you blinked
    //IntervalPCHBMonitoring
    if(ValidPCReadyCounter > 0)
    {
        PC_Ready = 1;
    }
   else
        PC_Ready = 0;
    
    ValidPCReadyCounter = 0;
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

  ////_______________\\\\
 ////*****************\\\\
////SERIAL SEND RECEIVE\\\\
\\\\*******************////


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

void SignalArdCommsEst()
{
  Serial.print(START_MARKER);
  Serial.print(ARD_COMMS_EST);
  Serial.print(END_MARKER);
}

void SignalSingleMarker(SerialOrder Marker)
{
  Serial.print(START_MARKER);
  Serial.print(Marker);
  Serial.print(END_MARKER);
}

void SendDataPacketHB()
{
  Serial.print(START_MARKER);
  Serial.print('?');
  Serial.print(END_MARKER);
}

void SendBoardName()
{
  Serial.print(START_MARKER);
  Serial.print(uniqueNameRead);
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

void ProcessInformationDualMotor()
{
  
  for(int i =0; i <21; i++)
  {
    bool SteerCommand = CheckForReqComm((int)BuffInComing[i], STEER_COMMAND);
    bool MotorLeft = CheckForReqComm((int)BuffInComing[i], MOTOR_FWD_LEFT) || CheckForReqComm((int)BuffInComing[i], MOTOR_REV_LEFT);
    bool MotorRight = CheckForReqComm((int)BuffInComing[i], MOTOR_FWD_RIGHT) || CheckForReqComm((int)BuffInComing[i], MOTOR_REV_RIGHT);    
    
    bool MotorLeftFwd = CheckForReqComm((int)BuffInComing[i], MOTOR_FWD_LEFT);
    bool MotorLeftRev = CheckForReqComm((int)BuffInComing[i], MOTOR_REV_LEFT);

    bool MotorRightFwd = CheckForReqComm((int)BuffInComing[i], MOTOR_FWD_RIGHT);
    bool MotorRightRev = CheckForReqComm((int)BuffInComing[i], MOTOR_REV_RIGHT);
    
    if(SteerCommand)
    {
     if(i<19)
     {
      if(BuffInComing[i+1] == ':')
      {
          int command = (int)BuffInComing[i+2]; 
          int CommandConverted = ConvertValue(command, 1.042, -10.42);
          ServoCommand = position_err(CommandConverted, MAX_ANGLE,  MIN_ANGLE);
          SerialSteeringControl();
          ValidSteerCommand++;           
      }
     }
   }

    if(MotorLeft)
    {
     if(i<19)
     {
      if(BuffInComing[i+1] == ':')
      {
        if(MotorLeftFwd)
        {
            int MotorCommandLeft = (int)BuffInComing[i+2];
            int MotorCommandLeftAct = ConvertValue(MotorCommandLeft, 3.55,-3.55);
            DriveMotorsCommandLeft(MotorCommandLeftAct);
            SetMotorsFWD();
        }
        if(MotorLeftRev)
        {
            int MotorCommandLeft = (int)BuffInComing[i+2];
            int MotorCommandLeftAct = ConvertValue(MotorCommandLeft, 3.55,-3.55);
            DriveMotorsCommandLeft(MotorCommandLeftAct);
            SetMotorsREV();
        }
       ValidMotorLeft++;
      }
     }
    }

    if(MotorRight)
    {
     if(i<19)
     {
      if(BuffInComing[i+1] == ':')
      {
        if(MotorRightFwd)
        {
            int MotorCommandRight = (int)BuffInComing[i+2];
            int MotorCommandRightAct = ConvertValue(MotorCommandRight, 3.55,-3.55);
            DriveMotorsCommandRight(MotorCommandRightAct);
            SetMotorsFWD();
        }
        if(MotorRightRev)
        {
            int MotorCommandRight = (int)BuffInComing[i+2];
            int MotorCommandRightAct = ConvertValue(MotorCommandRight, 3.55,-3.55);
            DriveMotorsCommandRight(MotorCommandRightAct);
            SetMotorsREV();
        }
       ValidMotorRight++;
      }
     }
    }
            
      if((int)BuffInComing[i] == PC_READY)
      {
         ValidPCReadyCounter++;
      }

      if((int)BuffInComing[i] == PC_HEARTBEAT)
      {
         ValidPCHeartBeatCounter++;
      }
  }
}

void ProcessInformationSingleMotor()
{
  
  for(int i =0; i <21; i++)
  {
    bool SteerCommand = CheckForReqComm((int)BuffInComing[i], STEER_COMMAND);
    bool MotorLeft = CheckForReqComm((int)BuffInComing[i], MOTOR_FWD_LEFT) || CheckForReqComm((int)BuffInComing[i], MOTOR_REV_LEFT);    
    bool MotorLeftFwd = CheckForReqComm((int)BuffInComing[i], MOTOR_FWD_LEFT);
    bool MotorLeftRev = CheckForReqComm((int)BuffInComing[i], MOTOR_REV_LEFT);

    if(BuffInComing[i] =='M') //Ensure this matches the unique char
    {
      SendBoardName();
      SendingBoardName = 1;
      
      if(BoardNameReq == 0)
        BoardNameReq = millis();
    }

    
    if(SteerCommand)
    {
     if(i<19)
     {
      if(BuffInComing[i+1] == ':')
      {
          int command = (int)BuffInComing[i+2]; 
          int CommandConverted = ConvertValue(command, 1.042, -10.42);
          ServoCommand = position_err(CommandConverted, MAX_ANGLE,  MIN_ANGLE);
          SerialSteeringControl();
          ValidSteerCommand++; 

      }
     }
   }

    if(MotorLeft)
    {
     if(i<19)
     {
      if(BuffInComing[i+1] == ':')
      {
        if(MotorLeftFwd)
        {
            int MotorCommandLeft = (int)BuffInComing[i+2];
            int MotorCommandLeftAct = ConvertValue(MotorCommandLeft, 3.55,-3.55);
            DriveMotorsCommand(MotorCommandLeftAct);
            
            motor_speed = MotorCommandLeftAct;
            
            SetMotorsFWD();
        }
        if(MotorLeftRev)
        {
            int MotorCommandLeft = (int)BuffInComing[i+2];
            int MotorCommandLeftAct = ConvertValue(MotorCommandLeft, 3.55,-3.55);
            DriveMotorsCommand(MotorCommandLeftAct);
                        
            motor_speed = MotorCommandLeftAct;
            
            SetMotorsREV();
        }
       ValidMotorLeft++;
      }
     }
    }

            
      if((int)BuffInComing[i] == PC_READY)
      {
         ValidPCReadyCounter++;
      }

      if((int)BuffInComing[i] == PC_HEARTBEAT)
      {
         ValidPCHeartBeatCounter++;
      }

      if((int)BuffInComing[i] == BOARD_FOUND)
      {
         BoardFoundByPC = 1;
      }

      
  }


}

void ServiceSerialLink()
{
  if (millis() - lastRequestTime > IntervalSerialMonitoring)   // e.g. 200 ms
  {
    requestSentTime = millis();
    SignalSingleMarker(REQUEST_COMMAND_MSB);   // nudge the PC, unconditionally
  }
}

void ListenForPCBoardRequest()
{
  uint8_t budget = 16;
  
  while (Serial.available() && budget--)
  {
   
    char Peeked = Serial.peek();

      if(Peeked == '?')
      {
        //SendingBoardName =1;
        SendBoardName();
      }
    
  }

   
}

void ReadAndParseDataSTD()
{
  uint8_t budget = 16;

  if(Serial.available())
    SerialOffTime = millis();
  
  while (Serial.available() && budget--)
  {
    char Peeked = Serial.peek();

      if((int)Peeked == PC_READY)
      {
         ValidPCReadyCounter++;
      }

      if((int)Peeked == PC_HEARTBEAT)
      {
         ValidPCHeartBeatCounter++;
      }

      if(Peeked == 'M')
      {
        SendingBoardName =1;
        SendBoardName();

        if(BoardNameReq == 0)
          BoardNameReq = millis();
      }
      
      if((int)Peeked == BOARD_FOUND)
      {
         BoardFoundByPC = 1;
      }
    
    char c = Serial.read();

    if (c == START_MARKER)
    {
      idx = 0;
      inMessage = true;
      BuffInComing[idx++] = c;
      StartValid =1;
    }
    else if (inMessage)
    {
      if (idx < sizeof(BuffInComing) - 1)
        BuffInComing[idx++] = c;

      if (c == END_MARKER)
      {
        BuffInComing[idx] = '\0';
        inMessage = false;
        EndValid =1;
        PcktSz = sizeof(BuffInComing);
        PacektsRecieved++;

        //if(SendingBoardName)
        // SignalSingleMarker(REQUEST_COMMAND_MSB);
        
        //ProcessInformationDualMotor();
        ProcessInformationSingleMotor();    
        
        roundTrip = millis() - requestSentTime;

        //if(BoardFoundByPC)
        //{
         //Serial.print("RT");
        //Serial.print(roundTrip);
        //}
        
        lastValidMsgTime = millis();
        lastRequestTime = millis();

        rttSum += roundTrip;
        rttCount++;
        if (roundTrip > rttMax) rttMax = roundTrip;
        
      }  
    }
  }
  if (Serial.available() == 0)
  {
    for(int j =0; j < 21; j++)
    {
       BuffInComing[j] = '\0';
    }
    if (millis() - SerialOffTime > IntervalSerialOff)   
    {
      SerialOffTime = millis(); // Serial off for specified interval (1.5 seconds)
      SendingBoardName = 0;
      MotorSteerSetupComplete = 0;
      BoardFoundByPC =0;
    }
    //PacektsRecieved =0;
  }

}


  ////________________\\\\
 ////******************\\\\
////  HELPER FUNCTIONS  \\\\
\\\\*******************////
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


int ConvertValue(int ValX, float m, float C){
  
 int ConvertedVal = (m*ValX) +  C;

  return ConvertedVal;
}

bool IsValidCommand(int command)
{
  SerialOrder CommandRec = (SerialOrder)command;

    
    switch (CommandRec)
    {
    case RADAR_DISTANCE:
    {   
        return true;
    }
    case RADAR_POSITION:
    {  
        return true;
    }
    case REQUEST_RADAR:
    {   
        return true;
    }
    case REQUEST_RADAR_POS:
    {   
        return true;
    }
    case ARDUINO_READY:
    {   
        return true;
    }
    case PC_READY:
    {   
        return true;
    }
    case PC_HEARTBEAT:
    {   
        return true;
    }
    case STEER_COMMAND:
    {   
        return true;
    }          
    default:
    {
        return false; //No valid order has been found.
    }
    }
}



bool CheckForReqComm(int command, SerialOrder ExpCommand)
{

  if((SerialOrder)command == ExpCommand)
    return true;
  else
    return false;
  
}

  ////______________________\\\\
 ////************************\\\\
////STEERING & MOTOR FUNCTIONS\\\\
\\\\*************************////
void MotorServosSetup()
{
  ser.attach(12);// servo is connected at pin 12

  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  // Set initial rotation direction
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  analogWrite(enA,0);

  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enB,0);

  //pinMode(trigPin, OUTPUT);
  //pinMode(echoPin, INPUT);

  // Print a message to the LCD.
  ser.write(50);
}
void SetMotorsFWD()
{
     digitalWrite(in1, HIGH);
     digitalWrite(in2, LOW);
     digitalWrite(in3, LOW);
     digitalWrite(in4, HIGH);
}

void SetMotorsREV()
{
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
}
void SetMotorSTOP()
{
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
      digitalWrite(in4, LOW);
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
      ser.write(ServoCommand);
  }

void DriveMotorForward()
{
analogWrite(enA, 255);
analogWrite(enB, 255);
} 

void DriveMotorReverse()
{
analogWrite(enA, 255);
analogWrite(enB, 255);
}

void DriveMotorsCommand(int Speed)
{  
analogWrite(enA, Speed);
analogWrite(enB, Speed);
}  

void DriveMotorsCommandLeft(int Speed)
{  
analogWrite(enA, Speed);
}

void DriveMotorsCommandRight(int Speed)
{  
analogWrite(enB, Speed);
} 

void DriveMotorStop(){
      
      analogWrite(enA, 0);
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);

      analogWrite(enB, 0);
      digitalWrite(in3, LOW);
      digitalWrite(in4, LOW);
      
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

     if(val=='g'){//motor increment
        
      motor_speed-=5;
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
  ////________________\\\\
 ////******************\\\\
////DIAGNOSIS,INDICATION\\\\
\\\\*******************////

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

void ClearDisp()
{
  display.clearDisplay();
}

void ShowDisp()
{
  display.display();
}

void DisplayMessage(char *mess, int x, int y)
{
  //display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);

  display.setCursor(x, y);
  display.println(mess); 
  
  //display.display();
}

void DisplayMessageInt(int mess, int x, int y)
{
  //display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);

  display.setCursor(x, y);
  display.println(mess); 
  
  //display.display();
}

void DisplayStatus()
{
  display.setTextSize(2);
  display.setTextColor(WHITE);

  if(CommsEstablished)
  {
    display.setCursor(1, 30);
    display.println("C"); 
  }
  else
  {     
    display.setCursor(1, 30);
    display.println("NC"); 
  }
  if(ValidHeartBeatPC)
  {
    display.setCursor(1, 5);
    display.println("H");
  }
  else
  {
    display.setCursor(1, 5);
    display.println("NH");
  }


  DisplayMessage("Pz", 30, 5);
  DisplayMessageInt(PacektsRecieved, 60, 5);

  //int BuffSize = Serial.available();
  DisplayMessage("St", 30, 30);
  DisplayMessageInt(ServoCommand, 70, 30);

  DisplayMessage("L", 1, 50);
  DisplayMessageInt(motor_speed, 20, 50);

  if(SendingBoardName)
  {
    DisplayMessage("NR", 50, 50);
  }

  //DisplayMessage("R", 65, 50);
  //DisplayMessageInt(ValidMotorRight, 80, 50);  

  //ValidSteerCommand;
  
}
void UpdateDisp()
{
  if (millis() - previousMillis4 < 250) return;   
  
  previousMillis4 = millis();
  
  ClearDisp();
  DisplayStatus();
  ShowDisp();
}  
