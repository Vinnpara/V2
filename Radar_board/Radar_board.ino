#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C Scrn(0x27,16,2);

#include<Servo.h> 
#include <Arduino.h>
#define echoPin 6 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 7
#define MAX_RADAR_ANGLE 180
#define MIN_RADAR_ANGLE 0

#define MAX_PAYLOAD_SIZE 64
#define MAX_PACKET_SIZE 7

#include "SerialOrder.h"
#include "ArduinoReceiver.h"
#include "SerialParameters.h"

float duration;
int distance, ServoRightValue, ServoLeftValue;
float MeasuredDistance;
unsigned long Time1, Time2, Time3, IterationTime, TimeoutValue, ProgramIterationTime;
unsigned int DelayTime;
int8_t MeasuredDistance_int8;
int16_t ServoPos;
Servo serv;
float pos_2;

int8_t Buffer16Int[2];
int16_t MeasuredDistance_int16, ServoAngleInt16;
SerialOrder OrderReceived;
SerialOrder OrderSent;

char buffin[MAX_PAYLOAD_SIZE];
char DataPacketReceived[MAX_PACKET_SIZE];


static int ServoPosition = 0,
           ValidPCHeartBeatCounter = 0,
           ValidPCReadyCounter =0;

static bool PC_not_Ready=1,
            PC_Ready = 0,
            CommsEstablished =0,
            ReStartSerial = 0,
            SerialFlushed = 0,
            ValidHeartBeatPC = 0,
            PositiveRadarSweep =1,
            NegativeRadarSweep =0;

unsigned long previousMillis = 0,
              previousMillis2 = 0,
              previousMillis3 = 0; 
const long interval = 50,
           IntervalPCHBMonitoring = 2000; // Interval in milliseconds

const char START_MARKER = '[',
           END_MARKER = ']';

unsigned long ElapsedTIme =0;

///////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
// ================================================================
// ===                    SETUP FUNCTION                       ===
// ================================================================
//***************************************************************\\


void setup() {
  // put your setup code here, to run once:
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication
  
  Scrn.init();
  Scrn.backlight();
  pinMode(LED_BUILTIN, OUTPUT);
  
  TimeoutValue=20000; //20k microsecons so max range of about 3m
  serv.attach(3);
}

///////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
// ================================================================
// ===                    LOOP FUNCTION                         ===
// ================================================================
//***************************************************************\\

void loop() {
  // put your main code here, to run repeatedly:
 
 /*if(Serial.available()>0)
 {
  //PingUltraSoundSensor();
  RadarSweep();
 }*/

RadarMeasure();

if(Serial.available()>0)
{

   //BulbON();
}

ReadAndParseData();
ProcessInformation();
MonitorPCHeartBeat();
MonitorPCReadiness();
DeterminePCReadiness();

SignalArdReady();
SendDataPacketHB();

DisplayStatus();

if(CommsEstablished)
{
   //RadarSweep();
   SendDataPackets(RADAR_DISTANCE,MeasuredDistance);
   SendDataPackets(RADAR_POSITION,ServoPosition);
   SignalArdCommsEst();
   BulbON();
}

if(!Serial.available())
{
  FlashBulb(20);
}

RadarMove();
 
}

  ////_______________\\\\
 ////*****************\\\\
////     RADAR FUNC    \\\\
\\\\*******************////

void RadarSweep(){
  
    for(ServoRightValue=0; ServoRightValue < 181; ServoRightValue++){
    ServoPosition = position_err(ServoRightValue, MAX_RADAR_ANGLE, MIN_RADAR_ANGLE );
    PingUltraSoundSensor();
    serv.write(ServoPosition);
    }
    
    for(ServoLeftValue=180; ServoLeftValue > -1; ServoLeftValue--){
    ServoPosition = position_err(ServoLeftValue, MAX_RADAR_ANGLE, MIN_RADAR_ANGLE );
    PingUltraSoundSensor();
    serv.write(ServoPosition);
    }    
  
  }

void RadarMeasure()
{
    PingUltraSoundSensor();
    serv.write(ServoPosition);
    delay(5);
}

void RadarMove()
{
  if(ServoPosition < 181 && PositiveRadarSweep)
  {
   if(ServoPosition != 180)
    ServoPosition++;
   if(ServoPosition == 180)
   {
    PositiveRadarSweep=0;
    NegativeRadarSweep=1;
   }
  }
  if(NegativeRadarSweep)
  {
   if(ServoPosition == 0)
   {
    NegativeRadarSweep=0;
    PositiveRadarSweep=1;
   }
   if(ServoPosition != 0)
    ServoPosition--;
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
  
  if(IterationTime >= 20)
  {
    MeasuredDistance = 150;
    IterationTime = 20;
  }
  else
  {
    MeasuredDistance = duration * 0.034 / 2;
  }

  //delay(10);

  
  }

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
  ElapsedTIme = currentMillis - previousMillis3;

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

void SignalArdCommsEst()
{
  Serial.print(START_MARKER);
  Serial.print(ARD_COMMS_EST);
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

void SendDataPacketsRadar()
{
  /*Serial.print(START_MARKER);
  Serial.print(MEASURED_PITCH);
  Serial.print(':');
  Serial.print(pitch);
  Serial.print(',');
  Serial.print(MEASURED_ROLL);
  Serial.print(':');
  Serial.print(roll);  
  Serial.print(END_MARKER);*/
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

void ReadAndParseData()
{
  if(Serial.available())
  {
   int bytesread = Serial.readBytes(DataPacketReceived,6);
   DataPacketReceived[7] = '\0';
   int j =0; //data added
   for(int i =0; i< 6; i++)
   {
    
    /*if((IsValidCommand((int)DataPacketReceived[i])) && j < MAX_PACKET_SIZE)
    {
      buffin[j] = DataPacketReceived[i];
      j++;
    }
    if(DataPacketReceived[i] == '[' && DataPacketReceived[i + 2] == ']')
    {
      buffin[j] = DataPacketReceived[i + 1]; //A valid datapacket will be enclosed between
      j++;                                   //two square
    }*/
    if((IsValidCommand((int)DataPacketReceived[i])) && j < MAX_PACKET_SIZE)
    {
      buffin[j] = DataPacketReceived[i]; //A valid datapacket will be enclosed between
      j++;                                   //two square
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

  ////__________________\\\\
 ////********************\\\\
//// DIAGNOSIS,INDICATION \\\\
\\\\**********************////

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

void DisplayStatus()
{
  Scrn.clear();
  
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


    //Scrn.setCursor(6, 1);
    //Scrn.print(strlen(DataPacketReceived));
    Scrn.setCursor(7, 1);
    Scrn.print(ServoPosition);
    Scrn.setCursor(11, 1);
    Scrn.print(MeasuredDistance);
    //Scrn.setCursor(14, 1);
    //Scrn.print(ValidPCReadyCounter);   
    
    /*Scrn.setCursor(9, 0);
    Scrn.print(DataPacketReceived[0]);
    Scrn.setCursor(10, 0);
    Scrn.print(DataPacketReceived[1]);*/
    Scrn.setCursor(11, 0);
    Scrn.print(ElapsedTIme); 
    //Scrn.setCursor(12, 0);
    //Scrn.print(ValidPCHeartBeatCounter);
    //Scrn.setCursor(13, 0);
    //Scrn.print(ValidPCReadyCounter);
    //Scrn.setCursor(14, 0);
    //Scrn.print(ValidPCReadyCounter);   

    //delay(10);
}

  ////________________\\\\
 ////******************\\\\
////  HELPER FUNCTIONS  \\\\
\\\\*******************////

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
    default:
    {
        return false; //No valid order has been found.
    }
    }
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

/*
**************************************************************
**************************************************************
***-----------UNUSED FUNCTIONS FROM HERE ON----------------***
***Functions from here on are unused for now, kept for
***Reference
*************************************************************
*/

/*



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




*/


/*

    Serial.print("Distance: ");
    Serial.println(MeasuredDistance);
    Serial.print("Radar pos: ");
    Serial.println(ServoRightValue);

  //Serial.print("Distance: ");
  //Serial.println(MeasuredDistance);
  //Serial.print("Iteration time : ");
  //Serial.println(IterationTime);
  //Serial.print("Delay time");
  //Serial.println(DelayTime); 
    
*/
 
