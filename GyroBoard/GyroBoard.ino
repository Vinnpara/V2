//OLED SCREEN VARIABLES
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Wire.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


//USING PITCH AND ROLL FROM OLDER CODE
//AND YAW FROM NEWER LIBRARY
//FOR STABILITY REASONS
//PIN A5 SCL, A4 SDA


#include <Arduino.h>

#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

#include "SerialOrder.h"
#include "ArduinoReceiver.h"
#include "SerialParameters.h"

#define INTERRUPT_PIN 2  // use pin 2 on Arduino Uno & most boards
#define LED_PIN 13 // (Arduino is 13, Teensy is 11, Teensy++ is 6)

#define MAX_PAYLOAD_SIZE 64
#define MAX_PACKET_SIZE 7

bool blinkState = false;

int16_t ValueSent;
int32_t val32;
static int PitchCounter, 
           YawCounter, 
           RollCounter, 
           SerialCounter,
           ValidPCHeartBeatCounter = 0,
           ValidPCReadyCounter =0,
           PcktSz = 0,
           PacektsRecieved = 0,
           PitchReq = 0;

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
char DataPacketReceived[MAX_PACKET_SIZE];

unsigned int bufferIndex = 0;
bool isReceiving = false;

int16_t GyroTst;

//---------------------ARDUINO_NAME-------------------------
//-----------------------GYRO--------------------------------
// Give this specific board a unique name (Max 20 characters)
//Adjust the define accordingly.

# define NAME_BUFF_SIZE 10
const char uniqueName[NAME_BUFF_SIZE] = "GYRO"; 

static char uniqueNameRead[NAME_BUFF_SIZE] = "GYRO"; 

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
// ===               VAR. FUNCTION VARIABLES               ===
// ================================================================

SerialOrder OrderReceivedRad, OrderSentRad;
static bool PC_not_Ready=1,
            PC_Ready = 0,
            CommsEstablished =0,
            ReStartSerial = 0,
            SerialFlushed = 0,
            ValidHeartBeatPC = 0;

unsigned long previousMillis = 0,
              previousMillis2 = 0,
              previousMillis3 = 0,
              previousMillis4 = 0,
              SerialOffTime = 0;

const long interval = 50,
           IntervalPCHBMonitoring = 2000,
           IntervalSerialOff = 1500; // Interval in milliseconds

const char START_MARKER = '[',
           END_MARKER = ']';

//New buffer
static char BuffInComing[20]; //11 works for the STR only command
static uint8_t idx = 0;
static bool inMessage = false;

static  bool StartValid = 0,
             EndValid =0,
             SendingBoardName =0,
             Calibrated = false,
             BoardFoundByPC = false;
             
//static unsigned long loopCount = 0;
//static unsigned long lastRateCheck = 0;

const unsigned long SEND_INTERVAL_MS = 10;   // 100 Hz
static unsigned long lastSendTime = 0;

const unsigned long SEND_INTERVAL_MSHB = 10;   // 100 Hz
static unsigned long lastSendTimeHB = 0;

///////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
// ================================================================
// ===                    SETUP FUNCTION                       ===
// ================================================================
//***************************************************************\\

void setup() {

  // put your setup code here, to run once:
  /*FOR GYROSCOPE*/
  // put your setup code here, to run once:
 pinMode(LED_BUILTIN, OUTPUT);

  // initialize serial communication
  // (115200 chosen because it is required for Teapot Demo output, but it's
  // really up to you depending on your project)

  Serial.begin(115200);

  //GyroSetup();
  //delay(20);
  
/*
OLED screen
*/ 
// 0x3C is default i2c adress in some cases MAY be different
/*
display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
display.clearDisplay();
display.setTextSize(2);
display.setTextColor(WHITE);
*/
     
}

///////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
// ================================================================
// ===                    LOOP FUNCTION                         ===
// ================================================================
//***************************************************************\\


void loop() {
  // put your main code here, to run repeatedly:

/* to determin execution speed
 * ensure to manually set any 
 * serial condition cases to true to simulate
 * sending to PC
loopCount++;
if (millis() - lastRateCheck >= 1000)
{
  Serial.print("Loop Hz: ");
  Serial.println(loopCount);
  loopCount = 0;
  lastRateCheck = millis();
}*/

ReadAndParseDataSTD();

if(!BoardFoundByPC)
{
 SendBoardName();
}

if(!Calibrated && SendingBoardName )
{
   GyroSetup();
   Calibrated = 1;

}

//if(Calibrated)
 MeasureGyro();


if(Serial.available()>0)
{
  //FlashBulb(15);
}

//ReadAndParseDataSTD();
//ProcessInformation();

MonitorPCHeartBeat();
MonitorPCReadiness();
DeterminePCReadiness();

if(BoardFoundByPC && (millis() - lastSendTime >= SEND_INTERVAL_MSHB))
{
  lastSendTimeHB = millis();
  SignalArdReady();
  SendDataPacketHB();
  SignalArdCommsEst();
}

if(CommsEstablished && (millis() - lastSendTime >= SEND_INTERVAL_MS))
{

   lastSendTime = millis();
   SendDataPackets(MEASURED_PITCH,pitch);
   SendDataPackets(MEASURED_ROLL,roll);
   //SignalArdCommsEst();
   //SendingBoardName = 0;
   //BulbON();
}

if(CommsEstablished)
 BulbON();
else
 BulbOFF();

if(!Serial.available())
{
  //FlashBulb(500);
}

if(Serial.available() && !CommsEstablished)
{
  //FlashBulb(25);
}

//UpdateDisp();

}
  ////_______________\\\\
 ////*****************\\\\
////  ACCEL/GYRO FUNC  \\\\
\\\\*******************////
void GyroSetup()
{
  
  
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
    
    while (!Serial); // wait for Leonardo enumeration, others continue immediately
     
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
        mpu.CalibrateGyro(6);     ///The *......>... Likely comes from above here
        //mpu.PrintActiveOffsets();
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

  
}
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
  elapsedTime = (currentTime - previousTime) / 1000.0; // Divide by 1000 to get seconds
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


  RollTransfer=roll*TransferGain;
  PitchTransfer=pitch*TransferGain;
  YawTransfer=yaw*TransferGain;
  
  PitchCounter ++;
/*
Serial.print("\npitch read ");
Serial.print(pitch);

Serial.print("\nroll read ");
Serial.print(roll);
*/
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

void ListenForPCBoardRequest()
{
  uint8_t budget = 16;
  
  while (Serial.available() && budget--)
  {
   
    char Peeked = Serial.peek();

      if(Peeked == 'G')
      {
        SendingBoardName =1;
        SendBoardName();
      }
    
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
    
    if((IsValidCommand((int)DataPacketReceived[i])) && j < MAX_PACKET_SIZE)
    {
      buffin[j] = DataPacketReceived[i];
      j++;
    }
   }
  }
  if(!Serial.available())
  {
   for(int i =0; i< 6; i++)
   {
    //Once the Program exits, there is no serial
    //so fill the buffer with empty, this
    //is for the PC ready, HB flags.
      buffin[i] = '\0';
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

      if(Peeked == 'G')
      {
        SendingBoardName =1;
        //SendBoardName();
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
        ProcessInformation();
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
      Calibrated = 0;
      
      if(BoardFoundByPC)
        BoardFoundByPC =0;
    }
  }

}

void ProcessInformation()
{
  int DataPacketSize = strlen(buffin);
  int CommandsReceived[DataPacketSize];

    for(int i =0; i <21; i++)
  {
     if(BuffInComing[i] == '\0')
      break;

       if((int)buffin[i] == PC_HEARTBEAT) 
       {
          ValidPCHeartBeatCounter++;
       }
       
       if((int)buffin[i] == PC_READY) 
       {
          ValidPCReadyCounter++;
       }

       if((int)buffin[i] == REQUEST_PITCH) 
       {
          PitchReq++;
       }

        if(BuffInComing[i] =='G')
       {
         SendingBoardName = 1;
         //SendBoardName();
       }

        if((int)BuffInComing[i] == BOARD_FOUND)
       {
         BoardFoundByPC = 1;
       }
  
  }


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


  DisplayMessage("Pn", 30, 5);
  DisplayMessageInt(PacektsRecieved, 60, 5);

  //int BuffSize = Serial.available();
  DisplayMessage("Pr", 30, 30);
  DisplayMessageInt(PitchReq, 70, 30);


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
  if (millis() - previousMillis4 < 250) return;   // 4 Hz is plenty
  
  previousMillis4 = millis();
  
  ClearDisp();
  DisplayStatus();
  ShowDisp();
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
    case MEASURED_ROLL:
    {   
        return true;
    }
    case MEASURED_PITCH:
    {  
        return true;
    }
    case MEASURED_YAW:
    {   
        return true;
    }
    case MEASURED_ACCEL_X:
    {   
        return true;
    }
    case MEASURED_ACCEL_Y:
    {   
        return true;
    }
    case MEASURED_ACCEL_Z:
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

/*
**************************************************************
**************************************************************
***-----------UNUSED FUNCTIONS FROM HERE ON----------------***
***Functions from here on are unused for now, kept for
***Reference
*************************************************************
*/
