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
int SteerCommand;

int ServoCommand;

int MAX_ANGLE = 180;
int MIN_ANGLE = 0;

int motor_speed=0;
int motor_speed_rev=0;



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
              previousMillis3 = 0; 
const long interval = 50,
           IntervalPCHBMonitoring = 1000; // Interval in milliseconds

const char START_MARKER = '[',
           END_MARKER = ']';

char DataPacketReceived[MAX_PACKET_SIZE],
     buffin[MAX_PAYLOAD_SIZE];

static int ValidPCHeartBeatCounter = 0,
           ValidPCReadyCounter =0,
           j = 0, //data added
           k = 0; //Steer buff counter

static int IndexAtStart = 0,
            IndexAtEnd = 0,
            ValidPckts = 0,
            NumberOfBytes = 0;

static  bool StartValid = 0,
       EndValid =0;

static char BuffInComing[20]; //11 works for the STR only command
static uint8_t idx = 0;
static bool inMessage = false;

void setup() {
  // put your setup code here, to run once:
pinMode(LED_BUILTIN, OUTPUT);

Serial.begin(9600); // Serial comm begin at 9600bps
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


 /*
LCD setup for debug
*/ 
  
Scrn.init();
Scrn.backlight();
  // Print a message to the LCD.
ser.write(50);
//poser=50;
//ZeroSteering();


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

ClearDisp();

if(Serial.available()>0)
{

   //BulbON();
}

ReadAndParseDataLim();

ProcessInformationV3Motor();
MonitorPCHeartBeat();
MonitorPCReadiness();
DeterminePCReadiness();

SignalArdReady();
SendDataPacketHB();



if (CommsEstablished && ValidHeartBeatPC)
{
  SignalArdCommsEst();
  //SerialSteeringControl();
  BulbON();
}
else
 BulbOFF();

//DisplayMessage("Tst", 55, 15);

DisplayStatus();

ShowDisp();

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

void ProcessInformationV3()
{

  int BuffLenght = strlen(BuffInComing);
  bool SteerCommRec=0;
  
  for(int i=0; i< BuffLenght; i++)
  {
      
      
      if((int)BuffInComing[i] == STEER_COMMAND)
      {
         //Scrn.setCursor(2, 0);
         //Scrn.print("S");

         SteerCommRec = 1;
      }
      if(BuffInComing[i] == ':')
      {
         //Scrn.setCursor(11, 0);
         //Scrn.print(":");
         
         if(i<(BuffLenght - 1))
         {
          if(SteerCommRec)
          {
            int command = (int)BuffInComing[i+1];
            
            int CommandConverted = ConvertValue(command, 1.042, -10.42);
            ServoCommand = position_err(CommandConverted, MAX_ANGLE,  MIN_ANGLE);
            SerialSteeringControl();
            
            //Scrn.setCursor(3, 0);
            //Scrn.print(ServoCommand);
          }
         }
         
      }
      if((int)BuffInComing[i] == PC_READY)
      {
         //Scrn.setCursor(0, 1);
         //Scrn.print("P");
         ValidPCReadyCounter++;
      }

      if((int)BuffInComing[i] == PC_HEARTBEAT)
      {
         //Scrn.setCursor(1, 1);
         //Scrn.print("H");
         ValidPCHeartBeatCounter++;
      }
      
  }

  //Scrn.setCursor(7, 0);
  //Scrn.print(BuffInComing);
/*
  for(int i =0; i < strlen(BuffInComing); i++)
  {
      if(BuffInComing[i] != START_MARKER || BuffInComing[i] != END_MARKER )
      Scrn.setCursor(1 + i, 0);
      Scrn.print(BuffInComing[i]);
  }
  */
}

void ProcessInformationV3Motor()
{

  int BuffLenght = strlen(BuffInComing);
  bool SteerCommRec=0,
       LeftFwd = 0,
       RightFwd =0,
       LeftRev = 0,
       RightRev =0;

  
  
  for(int i=0; i< BuffLenght; i++)
  {
      
      
     if((int)BuffInComing[i] == STEER_COMMAND)
     {
         //Scrn.setCursor(2, 0);
         //Scrn.print("S");
         SteerCommRec = 1;
     if(i<(BuffLenght - 2))
     {
      if(BuffInComing[i+1] == ':')
      {
          if(SteerCommRec)
          {
            int command = (int)BuffInComing[i+2];
            
            int CommandConverted = ConvertValue(command, 1.042, -10.42);
            ServoCommand = position_err(CommandConverted, MAX_ANGLE,  MIN_ANGLE);
            SerialSteeringControl();
            //Scrn.setCursor(4, 0);
            //Scrn.print(ServoCommand);
            //DisplayMessage("S", 60,5);
            //DisplayMessageInt(ServoCommand, 75,5);            
          }
        }
      }
      }

     if((int)BuffInComing[i] == MOTOR_FWD_LEFT)
     {
         //Scrn.setCursor(7, 0);
         //Scrn.print("FL");
         LeftFwd = 1;
     if(i<(BuffLenght - 2))
     {
      if(BuffInComing[i+1] == ':')
      {
          if(LeftFwd)
          {
            int MotorCommandLeft = (int)BuffInComing[i+2];
            //Scrn.setCursor(9, 0);
            //Scrn.print(MotorCommandLeft);
            int MotorCommandLeftAct = ConvertValue(MotorCommandLeft, 3.55,-3.55);
            DriveMotorsCommandLeft(MotorCommandLeftAct);
            SetMotorsFWD();
          }
        }
      }
      }

     if((int)BuffInComing[i] == MOTOR_FWD_RIGHT)
     {
         //Scrn.setCursor(7, 1);
         //Scrn.print("FR");
         RightFwd = 1;
     if(i<(BuffLenght - 2))
     {
      if(BuffInComing[i+1] == ':')
      {
          if(RightFwd)
          {
            int MotorCommandRight = (int)BuffInComing[i+2];
            //Scrn.setCursor(9, 1);
            //Scrn.print(MotorCommandRight);
            int MotorCommandRightAct = ConvertValue(MotorCommandRight, 3.55,-3.55);
            DriveMotorsCommandRight(MotorCommandRightAct);
            SetMotorsFWD();            
          }
        }
      }
      }

     if((int)BuffInComing[i] == MOTOR_REV_LEFT)
     {
         //Scrn.setCursor(7, 0);
         //Scrn.print("LR");
         LeftRev = 1;
     if(i<(BuffLenght - 2))
     {
      if(BuffInComing[i+1] == ':')
      {
          if(LeftRev)
          {
            int MotorCommandLeft = (int)BuffInComing[i+2];
            //Scrn.setCursor(9, 0);
            //Scrn.print(MotorCommandLeft);
            int MotorCommandLeftAct = ConvertValue(MotorCommandLeft, 3.55,-3.55);
            DriveMotorsCommandLeft(MotorCommandLeftAct);
            SetMotorsREV();             
          }
        }
      }
      }

     if((int)BuffInComing[i] == MOTOR_REV_RIGHT)
     {
         //Scrn.setCursor(7, 1);
         //Scrn.print("RR");
         RightRev = 1;
     if(i<(BuffLenght - 2))
     {
      if(BuffInComing[i+1] == ':')
      {
          if(RightRev)
          {
            int MotorCommandRight = (int)BuffInComing[i+2];
            //Scrn.setCursor(9, 1);
            //Scrn.print(MotorCommandRight);
            int MotorCommandRightAct = ConvertValue(MotorCommandRight, 3.55,-3.55);
            DriveMotorsCommandRight(MotorCommandRightAct);
            SetMotorsREV();             
          }
        }
      }
      }      
      
      if((int)BuffInComing[i] == PC_READY)
      {
         //Scrn.setCursor(2, 0);
         //Scrn.print("P");
         ValidPCReadyCounter++;
      }

      if((int)BuffInComing[i] == PC_HEARTBEAT)
      {
         //Scrn.setCursor(2, 1);
         //Scrn.print("H");
         ValidPCHeartBeatCounter++;
      }
  }
  

  //Scrn.setCursor(7, 0);
  //Scrn.print(BuffInComing);
/*
  for(int i =0; i < strlen(BuffInComing); i++)
  {
      if(BuffInComing[i] != START_MARKER || BuffInComing[i] != END_MARKER )
      Scrn.setCursor(1 + i, 0);
      Scrn.print(BuffInComing[i]);
  }
  */
}

void ProcessInformation()
{
  int DataPacketSize = strlen(buffin);
  int CommandsReceived[DataPacketSize];

   if(DataPacketSize>0)
   {
    for(int i=0; i < DataPacketSize; i++)
    {
      CommandsReceived[i] = (int)buffin[i];
    }
   }
     
     for(int i=0; i < DataPacketSize; i++)
    {
      if(CommandsReceived[i] == PC_HEARTBEAT)
      {
             ValidPCHeartBeatCounter++;
      }

      if(CommandsReceived[i] == PC_READY)
      {
            ValidPCReadyCounter++;
      }
    }
   
}


void ProcessInformationV2()
{
  int DataPacketSize = strlen(buffin);
  int CommandsReceived[DataPacketSize];
  int SteerIndex = 0;
  char SteerCommnd[6];

  bool StrCommandValid; //data must be of the order Command_type : Command_value
                        //The second memeber of SteerCommnd must be ':' 
  
   if(DataPacketSize>0)
   {

    for(int i=0; i < DataPacketSize; i++)
    {
      CommandsReceived[i] = (int)buffin[i];
    }
   }
     
   for(int i=0; i < DataPacketSize; i++)
   {
     if(CommandsReceived[i] == PC_HEARTBEAT)
     {
         ValidPCHeartBeatCounter++;
     }

     if(CommandsReceived[i] == PC_READY)
     {
        ValidPCReadyCounter++;
     }
    }

   for(int i=0; i < DataPacketSize; i++)
   {
      if((int)buffin[i] == STEER_COMMAND)
      { 
        //Scrn.setCursor(3, 1);
        //Scrn.print("str det");
        SteerIndex =i;
      }
   }

   /*for(int i=SteerIndex; i <= SteerIndex + 3; i++)
   {
    SteerCommnd[k] = buffin[i];
    k++;

    if(k == 2)
    {
      SteerCommnd[k] = '\0';
      k =0;
      
    }
    }
    */
   
    SteerCommnd[0] = buffin[SteerIndex];
    SteerCommnd[1] = buffin[SteerIndex + 1];
    SteerCommnd[2] = buffin[SteerIndex + 2];
    SteerCommnd[3] = '\0';

   if(SteerCommnd[1] == ':')
   {
    //Scrn.setCursor(5, 1);
    //Scrn.print("val");

    //Scrn.setCursor(9, 1);
    //Scrn.print((int)SteerCommnd[2]);
    SteerCommand = (int)SteerCommnd[2];
   }
  



}

void ReadAndParseDataLim()
{
  if (Serial.available())
  { 
   //char buffer[15],Serial.readBytes(Buffer,11) works when sending char buff[] = { START_MARKER, HeartBeat, ReadyInd, CommandType, ':', Command, END_MARKER };
   //
    char Buffer[16];
    int bytesread = Serial.readBytes(Buffer,15);
    
    //char c = Serial.read();
   for(int i =0; i < strlen(Buffer); i++)
   {
    
    char c = Buffer[i];

    if (c == START_MARKER)
    {
      idx = 0;
      inMessage = true;
      BuffInComing[idx++] = c;
    }
    else if (inMessage)
    {
      if (idx < sizeof(BuffInComing) - 1)
        BuffInComing[idx++] = c;

      if (c == END_MARKER)
      {
        BuffInComing[idx] = '\0';
        inMessage = false;
        SignalSingleMarker(REQUEST_COMMAND_MSB);
        //Scrn.setCursor(0, 0);
        //Scrn.print("Rd");
        DisplayMessage("Rq", 30, 5);
        
      }
     }
   }
  }
  if (Serial.available() == 0)
  {
    //Scrn.setCursor(6, 1);
    //Scrn.print("No serial");
    SignalSingleMarker(REQUEST_COMMAND_MSB);
    DisplayMessage("Rq", 30, 5);
    for(int j =0; j < 11; j++)
    {
       BuffInComing[j] = '\0';
    }
  }

  int BuffSize = Serial.available();
  DisplayMessage("Bz", 30, 30);
  DisplayMessageInt(BuffSize, 60, 30);
}
void ReadAndParseData()
{

  if (Serial.available())
  {
    
    char peek1 =Serial.peek();
    
    if((int)peek1 == PC_HEARTBEAT)
    {
      ValidPCHeartBeatCounter++;
    }

    if((int)peek1 == PC_READY)
    {
        ValidPCReadyCounter++;
    }

    //Scrn.setCursor(9, 0);
    //Scrn.print((int)peek1);
    
    char c = Serial.read();

    if (c == START_MARKER)
    {
      idx = 0;
      inMessage = true;
      BuffInComing[idx++] = c;
    }
    else if (inMessage)
    {
      if (idx < sizeof(BuffInComing) - 1)
        BuffInComing[idx++] = c;

      if (c == END_MARKER)
      {
        BuffInComing[idx] = '\0';
        inMessage = false;
        //DispTestData();
      }
    }
    // byte outside a START...END span is discarded
  }
}

void DispTestData()
{
    //Scrn.setCursor(6, 1);
    //Scrn.print(BuffInComing);
}

void ReadAndParseDataStd()
{
bool Strt;

  
  if(Serial.available())
  {
    char peek1 = Serial.peek(); //[

    if(peek1 == START_MARKER)
    {
      Serial.read();
    }

    char FirstChar = Serial.read();//HB

    if(FirstChar == PC_HEARTBEAT)
    {
      ValidPCHeartBeatCounter++;
    }

    char peek2 = Serial.peek();//]

    if(peek2 == END_MARKER)
    {
      Serial.read();
    }

    char peek3 = Serial.peek();//[
        
    if(peek3 == START_MARKER)
    {
      Serial.read();
    }

    char SecondChar = Serial.read();//PC READY

    if(SecondChar == PC_READY)
    {
      ValidPCReadyCounter++;
    }

   char peek4 = Serial.peek();//]
        
    if(peek4 == END_MARKER)
    {
      Serial.read();
    }

   char peek5 = Serial.peek();//[
        
    if(peek5 == START_MARKER)
    {
      Serial.read();
    }

                       //     0       1   2      3
   char CommandBuff[4];//SERIAL_ORDER : COMMAND  ]    


   char peek6 = Serial.peek();

   if((SerialOrder)peek6 == START_MARKER)
  {
     Serial.read();
  
    
   for (int i=0; i<4; i++)
   {
    CommandBuff[i] = Serial.read();
   }

    if(strlen(CommandBuff) >2 ) {
   //SERIAL ORDER
   SerialOrder Command = (SerialOrder)CommandBuff[0];

    if(Command == STEER_COMMAND)
    {
      Scrn.setCursor(6, 1);
      Scrn.print(Command);
    }


    if(CommandBuff[1] == ':')
    {
     Scrn.setCursor(8, 1);
     Scrn.print(":");
    }

    if(CommandBuff[3] == ']')
    {
     //Scrn.setCursor(8, 1);
     //Scrn.print(":");
      ServoCommand = position_err((int)CommandBuff[2], MAX_ANGLE,  MIN_ANGLE);
      Scrn.setCursor(9, 1);
      Scrn.print(ServoCommand);
      //delay(10);
    }
    }
  }

  }
  if(!Serial.available())
  {
   for(int i =0; i< 6; i++)
   {
    //Once the Program exits, there is no serial
    //so fill the buffer with null pointer, this
    //is for the PC ready, HB flags.
      buffin[i] = '\0';
   }
  }
}

void ReadAndParseDataV2()
{
  //Scrn.clear();
  if(Serial.available() > 0)
  {

  char DataRec = Serial.read();

  if (DataRec == START_MARKER) 
  {
    StartFound = true;
    j = 0;
    buffin[j] = '\0';

    //Scrn.setCursor(5, 0);
    //Scrn.print("Strt");

  }
  else if(StartFound)
  {
    if(DataRec != END_MARKER)
    {
      if(j < (sizeof(buffin) - 1))
      {
        buffin[j]= DataRec;
        j++;
        buffin[j] = '\0';
      }
    }
  else
  {
    StartFound = 0;
    newDataReceived =1;

    //Scrn.setCursor(5, 0);
    //Scrn.print("End");
  }
  }

  //Scrn.setCursor(6, 1);
  //Scrn.print(buffin);

  //Scrn.setCursor(1, 1);
  //Scrn.print(strlen(buffin));

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
  //Scrn.clear();

  //display.clearDisplay();
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

    //display.display();
    
/*
  if(CommsEstablished)
  {
     Scrn.setCursor(0, 0);
     Scrn.print("COMMSEST");
  }
  else
  {     
    Scrn.setCursor(0, 0);
    Scrn.print("NOCOMMS");
  }


  if(ValidHeartBeatPC)
  {
     Scrn.setCursor(0, 1);
     Scrn.print("HBEST");
  }
  else
  {
    Scrn.setCursor(0, 1);
    Scrn.print("NOHB");
  }

    //Scrn.setCursor(7, 1);
    //Scrn.print(SteerCommand);
*/

//Scrn.setCursor(0, 0);
//Scrn.print("diag");
/*
  Scrn.setCursor(2, 0);
   Scrn.print(ValidPckts);

  if(newDataReceived)
  {
    ValidPckts++;
    Scrn.setCursor(7, 0);
    Scrn.print("Vld");

    newDataReceived = 0;
  }
  else
  {
    Scrn.setCursor(7, 0);
    Scrn.print("No buff");
  }
 */ 
    //Scrn.setCursor(6, 1);
    //Scrn.print(SteerCommand);

    //Scrn.setCursor(6, 1);
    //Scrn.print(strlen(DataPacketReceived));
    //Scrn.setCursor(7, 1);
    //Scrn.print(ServoPosition);
    //Scrn.setCursor(11, 1);
    //Scrn.print(MeasuredDistance);
    //Scrn.setCursor(14, 1);
    //Scrn.print(ValidPCReadyCounter);   
    
   /*for(int i = 0; i< strlen(DataPacketReceived); i++)
    {
       Scrn.setCursor(10 + i, 0);
       Scrn.print(DataPacketReceived[i]);
    } 
    
   Scrn.setCursor(12, 1);
   Scrn.print(strlen(DataPacketReceived)); */
    
    //Scrn.setCursor(11, 0);
    //Scrn.print((int)DataPacketReceived[1]);
    //Scrn.setCursor(12, 0);
    //Scrn.print(DataPacketReceived[2]);
    //Scrn.setCursor(13, 0);
    //Scrn.print(DataPacketReceived[3]);    
    //Scrn.setCursor(14, 0);
   //Scrn.print(strlen(DataPacketReceived)); 
    
    //Scrn.setCursor(11, 0);
    //Scrn.print(ElapsedTIme); 
    //Scrn.setCursor(12, 0);
    //Scrn.print(ValidPCHeartBeatCounter);
    //Scrn.setCursor(13, 0);
    //Scrn.print(ValidPCReadyCounter);
    //Scrn.setCursor(14, 0);
    //Scrn.print(ValidPCReadyCounter);   

    //delay(5);
}  
