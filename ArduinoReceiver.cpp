#include <iostream>
#include <windows.h>

#include <fstream>
#include<serial/SerialPort.h>
#include<serial/SerialOrder.h>
#include "SerialComms.h"

#include <mutex>
#include <memory> 
#include <stdexcept>       
#include <cstddef>   

#include<ArduinoReceiver.h>
#include<SerialPortSelection.h>
#include <algorithm>
#include<string.h>

#define MAX_BUFFER 128
#define MAX_BUFFER_GYRO 64

char* ARDPort = "\\\\.\\COM3";
char* ARDPort2 = "\\\\.\\COM1";
char* ARDPort3 = "\\\\.\\COM5";
char* ARDPort4 = "\\\\.\\COM4";
char* ARDPort5 = "\\\\.\\COM7";
char* ARDPort6 = "\\\\.\\COM8";
char* ARDPort7 = "\\\\.\\COM9";
char* ARDPort8 = "\\\\.\\COM6";

static bool ValidCommandRoll, 
            ValidCommandPitch, 
            ValidCommandYaw, 
            ValidRadarVal, 
            ValidRadarPos, 
            ValidYaw,
            ValidCommandAccelX,
            ValidCommandAccelY,
            ValidCommandAccelZ,
            ValidCommandTime;


const char START_MARKER = '[',
           END_MARKER = ']';

ArduinoReceiver::ArduinoReceiver() {
	

	//Ard = Serial;

}

ArduinoReceiver::ArduinoReceiver(SerialName PortName) {


    switch (PortName)
    {
      case COM3:
        {
          SerialPort Serial(ARDPort);
          Ard = Serial;
          break;
        }

      case COM1:
      {
          SerialPort Serial2(ARDPort2);
          Ard = Serial2;
          break;
      }

      case COM5:
      {
          SerialPort Serial3(ARDPort3);
          Ard = Serial3;
          break;
      }
      case COM4:
      {
          SerialPort Serial4(ARDPort4);
          Ard = Serial4;
          break;
      }
      case COM7:
      {
          SerialPort Serial5(ARDPort5);
          Ard = Serial5;
          break;
      }
      case COM8:
      {
          SerialPort Serial6(ARDPort6);
          Ard = Serial6;
          break;
      }
      case COM9:
      {
          SerialPort Serial6(ARDPort7);
          Ard = Serial6;
          break;
      }

    }

}

void ArduinoReceiver::AssignPort(SerialName PortName, SerialSpeed BaudRate) {
    switch (PortName)
    {
    case COM3:
    {
        SerialPort Serial(ARDPort, BaudRate);
        Ard = Serial;
        break;
    }

    case COM1:
    {
        SerialPort Serial2(ARDPort, BaudRate);
        Ard = Serial2;
        break;
    }

    case COM5:
    {
        SerialPort Serial3(ARDPort, BaudRate);
        Ard = Serial3;
        break;
    }
    case COM4:
    {
        SerialPort Serial4(ARDPort, BaudRate);
        Ard = Serial4;
        break;
    }
    case COM7:
    {
        SerialPort Serial5(ARDPort, BaudRate);
        Ard = Serial5;
        break;
    }
    case COM8:
    {
        SerialPort Serial6(ARDPort, BaudRate);
        Ard = Serial6;
        break;
    }
    case COM9:
    {
        SerialPort Serial6(ARDPort, BaudRate);
        Ard = Serial6;
        break;
    }

    }

}

ArduinoReceiver::ArduinoReceiver(SerialName PortName, SerialSpeed BaudRate) {


    switch (PortName)
    {
    case COM3:
    {
        SerialPort Serial(ARDPort , BaudRate);
        Ard = Serial;
        break;
    }

    case COM1:
    {
        SerialPort Serial2(ARDPort, BaudRate);
        Ard = Serial2;
        break;
    }

    case COM5:
    {
        SerialPort Serial3(ARDPort, BaudRate);
        Ard = Serial3;
        break;
    }
    case COM4:
    {
        SerialPort Serial4(ARDPort, BaudRate);
        Ard = Serial4;
        break;
    }
    case COM7:
    {
        SerialPort Serial5(ARDPort, BaudRate);
        Ard = Serial5;
        break;
    }
    case COM8:
    {
        SerialPort Serial6(ARDPort, BaudRate);
        Ard = Serial6;
        break;
    }
    case COM9:
    {
        SerialPort Serial6(ARDPort, BaudRate);
        Ard = Serial6;
        break;
    }

    }

}

void ArduinoReceiver::AssignPort(SerialName PortName) {



    switch (PortName)
    {
    case COM3:
    {
        SerialPort Serial(ARDPort);
        Ard = Serial;
        break;
    }

    case COM1:
    {
        SerialPort Serial2(ARDPort2);
        Ard = Serial2;
        break;
    }

    case COM5:
    {
        SerialPort Serial3(ARDPort3);
        Ard = Serial3;
        break;
    }
    case COM4:
    {
        SerialPort Serial4(ARDPort4);
        Ard = Serial4;
        break;
    }
    case COM7:
    {
        SerialPort Serial5(ARDPort5);
        Ard = Serial5;
        break;
    }
    case COM8:
    {
        SerialPort Serial6(ARDPort6);
        Ard = Serial6;
        break;
    }
    case COM9:
    {
        SerialPort Serial6(ARDPort7);
        Ard = Serial6;
        break;
    }

    }


}

void ArduinoReceiver::SetArdPort(SerialName PortName) {



    switch (PortName)
    {
    case COM3:
    {
        //SerialPort Serial(ARDPort);
        Ard.InitializeSerial(ARDPort);
        break;
    }

    case COM1:
    {
        //SerialPort Serial2(ARDPort2);
        Ard.InitializeSerial(ARDPort2);
        break;
    }

    case COM5:
    {
        //SerialPort Serial3(ARDPort3);
        Ard.InitializeSerial(ARDPort3);
        break;
    }
    case COM4:
    {
        //SerialPort Serial4(ARDPort4);
        Ard.InitializeSerial(ARDPort4);
        break;
    }
    case COM7:
    {
        //SerialPort Serial5(ARDPort5);
        Ard.InitializeSerial(ARDPort5);
        break;
    }
    case COM8:
    {
        //SerialPort Serial6(ARDPort6);
        Ard.InitializeSerial(ARDPort6);
        break;
    }
    case COM9:
    {
        //SerialPort Serial6(ARDPort7);
        Ard.InitializeSerial(ARDPort7);
        break;
    }

    }


}

std::string ArduinoReceiver::GetBoardMessage()
{
    std::string board;

    switch (ArduinoType)
    {
        case RADAR_BOARD:
        {
            
            board = {"Radar board"};
            
            return  board;
            break;
        }
        case GYROSCOPE_BOARD:
        {

            board = { "GyroScope board" };

            return  board;
            break;
        }
        case MOTOR_STEER_BOARD:
        {

            board = { "Motor steer board" };

            return  board;
            break;
        }

    }
}

int ArduinoReceiver::GetBoardMessageCode()
{
    int board;

    switch (ArduinoType)
    {
    case RADAR_BOARD:
    {

        board = 4000;

        return  board;
        break;
    }
    case GYROSCOPE_BOARD:
    {

        board = 3000;

        return  board;
        break;
    }
    case MOTOR_STEER_BOARD:
    {

        board = 5000;

        return  board;
        break;
    }

    }
}

void ArduinoReceiver::SetArdPort(SerialName PortName, DataConcentrator& DC)
{
 /*
* Messages:
* Board initilization, cyan or red
* code 8001, 8002 Radar
* code 8003, 8004 Gyroscope
* code 8005, 8006 Motor steer
 */
    
    
    bool Serialinitialized = 0;
    
    switch (PortName)
    {
    case COM3:
    {
        //SerialPort Serial(ARDPort);
        Serialinitialized = Ard.InitializeSerialM(ARDPort);
        break;
    }

    case COM1:
    {
        //SerialPort Serial2(ARDPort2);
        Serialinitialized = Ard.InitializeSerialM(ARDPort2);
        break;
    }

    case COM5:
    {
        //SerialPort Serial3(ARDPort3);
        Serialinitialized = Ard.InitializeSerialM(ARDPort3);
        break;
    }
    case COM4:
    {
        //SerialPort Serial4(ARDPort4);
        Serialinitialized = Ard.InitializeSerialM(ARDPort4);
        break;
    }
    case COM7:
    {
        //SerialPort Serial5(ARDPort5);
        Serialinitialized = Ard.InitializeSerialM(ARDPort5);
        break;
    }
    case COM8:
    {
        //SerialPort Serial6(ARDPort6);
        Serialinitialized = Ard.InitializeSerialM(ARDPort6);
        break;
    }
    case COM9:
    {
        //SerialPort Serial6(ARDPort7);
        Serialinitialized = Ard.InitializeSerialM(ARDPort7);
        break;
    }
    case COM6:
    {
        //SerialPort Serial6(ARDPort7);
        Serialinitialized = Ard.InitializeSerialM(ARDPort8);
        break;
    }

    }

    std::string SerialStatus;
    std::string Board = GetBoardMessage();

    if (Serialinitialized)
    {
        
        SerialStatus = { "13:Ardunio initialized " };
        std::string CompleteMessage = SerialStatus + Board;

        int code = 8000 + (int)ArduinoType;

        std::cout << "\nBOARD INIT " << Board <<" " << code;

        DC.SetMessageStatus(CompleteMessage, code, MESSAGE_ON);

    }
    else
    {
        
        SerialStatus = { "11: Warning, Ardunio not initialized " };
        std::string CompleteMessage = SerialStatus + Board;

        int code = 8000 + ((int)ArduinoType * 2);
        
        DC.SetMessageStatus(CompleteMessage, code, MESSAGE_ON);
    }

}

void ArduinoReceiver::SetArdPort(char* PortName, DataConcentrator& DC)
{
    /*
* Messages:
* Board initilization, cyan or red
* code 8001, 8002 Radar
* code 8003, 8004 Gyroscope
* code 8005, 8006 Motor steer
 */

    bool Serialinitialized = Ard.InitializeSerialM(PortName);

    std::string SerialStatus;
    std::string Board = GetBoardMessage();

    if (Serialinitialized)
    {

        SerialStatus = { "13:Ardunio initialized " };
        std::string CompleteMessage = SerialStatus + Board;

        int code = 8000 + (int)ArduinoType;

        std::cout << "\nBOARD INIT " << Board << " " << code;

        DC.SetMessageStatus(CompleteMessage, code, MESSAGE_ON);

    }
    else
    {

        SerialStatus = { "11: Warning, Ardunio not initialized " };
        std::string CompleteMessage = SerialStatus + Board;

        int code = 8000 + ((int)ArduinoType * 2);

        DC.SetMessageStatus(CompleteMessage, code, MESSAGE_ON);
    }


}

void ArduinoReceiver::SetArdPort(int PortName, DataConcentrator& DC)
{
/*
* Messages:
* Board initilization, cyan or red
* code 8001, 8002 Radar
* code 8003, 8004 Gyroscope
* code 8005, 8006 Motor steer
*/
    bool Serialinitialized = 0;

    Serialinitialized = Ard.InitializeSerialM(PortName);

    std::string SerialStatus;
    std::string Board = GetBoardMessage();

    if (Serialinitialized)
    {

        SerialStatus = { "13:Ardunio initialized " };
        std::string CompleteMessage = SerialStatus + Board;


        int code = 8000 + (int)ArduinoType;

        DC.SetMessageStatus(CompleteMessage, code, MESSAGE_ON);
    }
    else
    {

        SerialStatus = { "11: Warning, Ardunio not initialized " };
        std::string CompleteMessage = SerialStatus + Board;

        int code = 8000 + ((int)ArduinoType * 2);

        DC.SetMessageStatus(CompleteMessage, code, MESSAGE_ON);
    }
}

void ArduinoReceiver::SetBaudRate(SerialSpeed BaudRate) {

    Ard.SetBaudRate(BaudRate);

}

void ArduinoReceiver::SetBaudRate(int BaudRate)
{


    Ard.SetBaudRate(BaudRate);
}

void ArduinoReceiver::ArdInitialize() {

    FirstPass = true;
    ValidRoll = false;
    ValidPitch = false;
    ValidAccelX = false;
    ValidAccelY = false;
    ValidAccelZ = false;
}

void ArduinoReceiver::ArduinoFirstPass(){
}


void ArduinoReceiver::RequestReadData(SerialPort& Serial, SerialOrder Command, static bool PCReady){
    bool TransferFail;
    
    if (PCReady) {
        char buff[1] = { Command };

        TransferFail = Serial.writeSerialPort(buff, 1);

        /*if(TransferFail)
           std::cout << "\n " << Command <<" Transfer Fail" << std::endl;
        else
           std::cout << "\n " << Command << " Transfer Fail False" << std::endl;*/
    }
    /*else
       std::cout << "\n " << Command << " PCNOTReady" << std::endl;*/

}

void ArduinoReceiver::RequestReadData(SerialPort& Serial, SerialOrder Command) 
{
    bool TransferFail;

    char command = char(Command);
    
    char buff[3] = { START_MARKER, command, END_MARKER };



    TransferFail = Serial.writeSerialPort(buff, 3);

    //std::string message;
    //message = START_MARKER + char(Command) + END_MARKER;
    //std::cout << "\nmesage to be sent " << buff[0]<<" "<<buff[1]<<" "<<buff[2];

}

void ArduinoReceiver::RequestReadData(SerialPort& Serial, const int Command)
{
    bool TransferFail;

    char command = char(Command);

    char buff[3] = { START_MARKER, command, END_MARKER };



    TransferFail = Serial.writeSerialPort(buff, 3);

    //std::string message;
    //message = START_MARKER + char(Command) + END_MARKER;
    //std::cout << "\nmesage to be sent " << buff[0] << " " << buff[1] << " " << buff[2];
}

void ArduinoReceiver::RequestData(SerialPort& Serial, SerialOrder Command)
{
    bool TransferFail;

    char buff[1] = { Command };

    TransferFail = Serial.writeSerialPort(buff, 1);
}

int32_t ArduinoReceiver::LimitValueInt32(int32_t& Value, int32_t MAX, int32_t MIN) {

    if (Value > MAX) {
        return 0;
    }
    if (Value < MIN) {
        return 0;
    }
    else
        return Value;

}

int16_t ArduinoReceiver::BufferFilterInt16(int16_t MaxValue, int16_t MinValue, int16_t& ReadValue) {

    if (ReadValue <= MinValue)
        ReadValue = MinValue;

    else if (ReadValue >= MaxValue)
        ReadValue = MaxValue;

    return ReadValue;

}

void ArduinoReceiver::ReTryRequest(SerialPort& Serial, SerialOrder Command) {

    bool TransferFail;

    char buff[1] = { Command };
    TransferFail = Serial.writeSerialPort(buff, 1);
    //cout << "\n RETRYING COMMAND " << TransferFail << endl;


}

void ArduinoReceiver::KeepSerialOpen() {
    
    bool TransferFail;
    char buff[1] = { HELLO };
    TransferFail = Ard.writeSerialPort(buff, 1);
}

bool ArduinoReceiver::RequestedCommandReceived(SerialOrder Command) {
    bool ExpectedCommand = 0;

    SerialOrder ReceivedType;

    if (Ard.isConnected()) {

        ReceivedType = read_order(Ard);

        if (ReceivedType == Command)
            ExpectedCommand = 1;

        //std::cout << "\n RequestedCommandReceived ARD RETRYING COMMAND " << ReceivedType <<" "<< Command << std::endl;
    }
    return ExpectedCommand;

}

bool ArduinoReceiver::ReadAndSendRequestedData(SerialOrder CommandExpected, int8_t Command) {

    bool ExpectedCommand = 0;

    SerialOrder ReceivedType;

    if (Ard.isConnected()) {

        ReceivedType = read_order(Ard);

        if (ReceivedType == CommandExpected) {
            ExpectedCommand = 1;

            bool TransferFail, TF2;
            char buff[1] = { STEER_COMMAND };
            //char buff2[1] = { Command };
            TransferFail = Ard.writeSerialPort(buff, 1);
            //TF2= Ard.writeSerialPort(buff2, 1);

           
        }
        //std::cout << "\n RequestedCommandReceived ARD RETRYING COMMAND " << ReceivedType - 10 <<" "<< CommandExpected << std::endl;
    }
    return ExpectedCommand;

}

void ArduinoReceiver::CheckSentCommand(SerialOrder Command){
    SerialOrder ReceivedType;

    if (Ard.isConnected()) {

        ReceivedType = read_order(Ard);

        if (ReceivedType == Command) {
            
            if(STEER_COMMAND == Command)
             SteeringAngleSent = read_i8(Ard);


        }

    }

}

void ArduinoReceiver::ReadPitchRoll(SerialPort& Serial) {

    std::vector <int32_t> values;
    
    if (Serial.isConnected()) {
        values = read_i32(Serial, 4);

        ConvertedPitch = float(values[0]) / 1000;
        ConvertedRoll = float(values[1]) / 1000;

        std::cout << "\n VAL 1 "<< values[0];
        std::cout << "\n VAL 2 " << values[1];

    }
    else {
        //std::cout << "\n SERIAL CLOSED ";
    }

}

void ArduinoReceiver::ReadBuffer(SerialPort& Serial, SerialOrder Command, static bool& ExpectedCommand){

    int32_t Max = 8000000, Min = -8000000;

    SerialOrder ReceivedType;

    if (Serial.isConnected()) {

        ReceivedType = read_order(Serial);
        //std::cout << "\nENUM RECEVIED " << ReceivedType << std::endl;

        if ((Command - 10) == ReceivedType) {
            ExpectedCommand = true;
        }
        else
            ExpectedCommand = false;

        switch (ReceivedType)
        {
        case HELLO:
        {
            //cout << "\nHELLO" << endl;
            break;
        }
        case MEASURED_ROLL:
        {   //This sent as an int16_t
            //int16_t Roll_Int16 = read_i16(Serial);

            int32_t Roll_Int32 = read_i32(Serial);
            int32_t Roll_Int32_lim = LimitValueInt32(Roll_Int32, Max, Min);
            //RadarValue = MeasuredRadarDistance;
            //BufferFilterInt16(201, 0, MeasuredRadarDistance);
            ConvertedRoll = float(Roll_Int32_lim) / 1000;
            //std::cout << "\MEASURED_ROLL " << Roll_Int32_lim << " " << ConvertedRoll << std::endl;
            //cout << "\nRADAR_DISTANCE " << MeasuredRadarDistance << endl;
            break;
        }
        case MEASURED_PITCH:
        {   //This sent as an int16_t
           // int16_t Pitch_Int16 = read_i16(Serial);

            int32_t Pitch_Int32 = read_i32(Serial);
            int32_t Pitch_Int32_lim = LimitValueInt32(Pitch_Int32, Max, Min);
            //RadarValue = MeasuredRadarDistance;
            //BufferFilterInt16(201, 0, MeasuredRadarDistance);
            ConvertedPitch = float(Pitch_Int32_lim) / 1000;
            //std::cout << "\MEASURED_PITCH " << Pitch_Int32_lim << " " << ConvertedPitch << std::endl;
            //cout << "\nRADAR_DISTANCE " << MeasuredRadarDistance << endl;
            break;
        }
        case MEASURED_YAW:
        {   //This sent as an int16_t
            //int16_t Yaw_Int16 = read_i16(Serial);

            int32_t Yaw_Int32 = read_i32(Serial);
            int32_t Yaw_Int32_lim = LimitValueInt32(Yaw_Int32, Max, Min);
            //RadarValue = MeasuredRadarDistance;
            //BufferFilterInt16(201, 0, MeasuredRadarDistance);
            ConvertedYaw = float(Yaw_Int32_lim) / 1000;
            //std::cout << "\MEASURED_YAW " << Yaw_Int32_lim << " " << ConvertedYaw << std::endl;
            break;
        }
        case MEASURED_ACCEL_X:
        {   //This sent as an int16_t
            int32_t AC_x_Int32 = read_i32(Serial);
            int32_t AC_x_Int32_limit = LimitValueInt32(AC_x_Int32, Max, Min);
            //RadarValue = MeasuredRadarDistance;
            //BufferFilterInt16(201, 0, MeasuredRadarDistance);
            ConvertedXAccel = float(AC_x_Int32_limit) / 10000;
            //std::cout << "\MEASURED_X_ACCEL " << X_Accel_Int16 << " " << ConvertedXAccel << std::endl;
            //cout << "\nRADAR_DISTANCE " << MeasuredRadarDistance << endl;
            break;
        }
        case MEASURED_ACCEL_Y:
        {   //This sent as an int16_t
            int32_t AC_y_Int32 = read_i32(Serial);
            int32_t AC_y_Int32_limit = LimitValueInt32(AC_y_Int32, Max, Min);
            //RadarValue = MeasuredRadarDistance;
            //BufferFilterInt16(201, 0, MeasuredRadarDistance);
            ConvertedYAccel = float(AC_y_Int32_limit) / 10000;
            //std::cout << "\MEASURED_X_ACCEL " << X_Accel_Int16 << " " << ConvertedXAccel << std::endl;
            //cout << "\nRADAR_DISTANCE " << MeasuredRadarDistance << endl;
            break;
        }
        case MEASURED_ACCEL_Z:
        {   //This sent as an int16_t
            int32_t AC_z_Int32 = read_i32(Serial);
            int32_t AC_z_Int32_limit = LimitValueInt32(AC_z_Int32, Max, Min);
            //RadarValue = MeasuredRadarDistance;
            //BufferFilterInt16(201, 0, MeasuredRadarDistance);
            ConvertedZAccel = float(AC_z_Int32_limit) / 10000;
            //std::cout << "\MEASURED_X_ACCEL " << X_Accel_Int16 << " " << ConvertedXAccel << std::endl;
            //cout << "\nRADAR_DISTANCE " << MeasuredRadarDistance << endl;
            break;
        }
        case MEASURED_TIME:
        {   //This sent as an int16_t
            int32_t Time = read_i32(Serial);
            //int32_t AC_z_Int32_limit = LimitValueInt32(AC_z_Int32, Max, Min);
            //RadarValue = MeasuredRadarDistance;
            //BufferFilterInt16(201, 0, MeasuredRadarDistance);
            ElapsedTime = (unsigned long)Time;
            //std::cout << "\MEASURED_X_ACCEL " << X_Accel_Int16 << " " << ConvertedXAccel << std::endl;
            //cout << "\nRADAR_DISTANCE " << MeasuredRadarDistance << endl;
            break;
        }
        case RADAR_DISTANCE:
        {   //This sent as an int16_t
            int16_t MeasuredRadarDistance = read_i16(Serial);
            RadarValue = MeasuredRadarDistance;
            BufferFilterInt16(201, 0, MeasuredRadarDistance);
            //std::cout << "\nRADAR_DISTANCE " << MeasuredRadarDistance << std::endl;
            break;
        }
        case RADAR_POSITION:
        {   //This sent as an int16_t
            int16_t MeasuredRadarPosition = read_i16(Serial);
            RadarPosition = MeasuredRadarPosition;
            //std::cout << "\nRADAR_POSITION " << MeasuredRadarPosition << std::endl;
            break;
        }
        
        default:
        {
            //The PC is not getting any valid values so do not write into serial on ard.

            //SerialOrder OrderWait = PC_NOT_READY;

            //char buff[1] = { OrderWait };
            //bool TransferReceived = false;
            //bool TransferFail = Serial.writeSerialPort(buff, 1);
            //cout << "\nBAD ENUM RECEVIED " <<endl;
            //char InternalBuffer[MAX_DATA_LENGTH];
            //Serial.readSerialPort(InternalBuffer, MAX_DATA_LENGTH);
            //cout << "\nUnknown command buffer: "<< InternalBuffer[0] << endl;
            //cout << "\nUnknown command : " << ReceivedType << endl;

            ReTryRequest(Serial, Command);
        }

        }


        //cout << "\nCommand not received   " << Command << endl;
        //std::cout << "\nFrom Class ORDER " << ReceivedType << std::endl;
    }
    /*else
        std::cout << "\nARDUINO DISCONNECTED " << std::endl;*/
}

void ArduinoReceiver::ReadBufferArduino3Accel2Attitude() {
    char BufferIn[MAX_BUFFER];

    Ard.readSerialPort(BufferIn, 12);

    char *BuffPointer, 
         *BuffPointer2;

    int *OrderPointerPitch,
        *OrderPointerRoll;

    int32_t *PitchValuePointer,
            *RollValuePointer;

    BuffPointer2 = BufferIn;
    BuffPointer = BuffPointer2;

    OrderPointerPitch = (int*)BuffPointer;
    int EnumReadFromBufferAsInt = *OrderPointerPitch;
    BuffPointer += sizeof(int);

    PitchValuePointer = (int32_t*)BuffPointer;
    int32_t ValueReadFromBufferPitch = *PitchValuePointer;
    BuffPointer += sizeof(int32_t);

    OrderPointerRoll = (int*)BuffPointer;
    int EnumReadFromBufferAsIntRoll = *OrderPointerRoll;
    BuffPointer += sizeof(int);

    RollValuePointer = (int32_t*)BuffPointer;
    int32_t ValueReadFromBufferRoll = *RollValuePointer;

    ReadOrder1 = (SerialOrder)EnumReadFromBufferAsInt;
    ReadOrder2 = (SerialOrder)EnumReadFromBufferAsIntRoll;

    ConvertedPitch = (float)(ValueReadFromBufferPitch / 1000);
    ConvertedRoll = (float)(ValueReadFromBufferRoll / 1000);



}

void ArduinoReceiver::ReadArduino3Attitudes() {

    

    ///RequestReadDataFirstRequest(arduino, REQUEST_PITCH, FirstPass);
    RequestReadData(Ard, REQUEST_PITCH, ValidCommandRoll);
    ReadBuffer(Ard, REQUEST_PITCH, ValidCommandRoll);

    if (ValidCommandRoll) {
        ValidRoll = true;
    }


    if (ValidRoll) {
        RequestReadData(Ard, REQUEST_ROLL, ValidCommandPitch);
        ReadBuffer(Ard, REQUEST_ROLL, ValidCommandPitch);
    }

    if (ValidCommandPitch);
    ValidPitch = true;

    if (ValidPitch) {

        RequestReadData(Ard, REQUEST_YAW, ValidCommandYaw);
        ReadBuffer(Ard, REQUEST_YAW, ValidCommandYaw);
    }


}

void ArduinoReceiver::ReadArduinoAttitudeAccel() {


    ///RequestReadDataFirstRequest(arduino, REQUEST_PITCH, FirstPass);
    RequestReadData(Ard, REQUEST_PITCH, ValidCommandRoll);
    ReadBuffer(Ard, REQUEST_PITCH, ValidCommandRoll);

    if (ValidCommandRoll) {
        ValidRoll = true;
    }


    if (ValidRoll) {
        RequestReadData(Ard, REQUEST_ROLL, ValidCommandPitch);
        ReadBuffer(Ard, REQUEST_ROLL, ValidCommandPitch);
    }

    if (ValidCommandPitch);
    ValidPitch = true;

    if (ValidPitch) {

        RequestReadData(Ard, REQUEST_YAW, ValidCommandYaw);
        ReadBuffer(Ard, REQUEST_YAW, ValidCommandYaw);
    }

    if (ValidCommandYaw);
    ValidAccelX = true;

    if (ValidAccelX) {

        RequestReadData(Ard, REQUEST_ACCEL_X, ValidCommandAccelX);
        ReadBuffer(Ard, REQUEST_ACCEL_X, ValidCommandAccelX);
    }

    if (ValidCommandAccelX);
    ValidAccelY = true;

    if (ValidAccelY) {

        RequestReadData(Ard, REQUEST_ACCEL_Y, ValidCommandAccelY);
        ReadBuffer(Ard, REQUEST_ACCEL_Y, ValidCommandAccelY);
    }

    if (ValidCommandAccelZ);
    ValidAccelZ = true;

    if (ValidAccelZ) {

        RequestReadData(Ard, REQUEST_ACCEL_Z, ValidCommandAccelZ);
        ReadBuffer(Ard, REQUEST_ACCEL_Z, ValidCommandAccelZ);

    }

}

void ArduinoReceiver::PeekAndRead(SerialPort& Serial, SerialOrder Command, static bool& ExpectedCommand)
{
    //PeekForData
    int32_t Max = 8000000, Min = -8000000;
    SerialOrder ReceivedType;
    bool command;
    int data;


    if (Serial.isConnected()) {

        command = PeekForData(Command, data);
        ExpectedCommand = command;
        //std::cout << command;
        //ReceivedType = read_order(Serial);

        if (command)
        {
            switch (data)
            {
            case HELLO:
            {
                //cout << "\nHELLO" << endl;
                break;
            }
            case MEASURED_ROLL:
            {   //This sent as an int16_t
                //int16_t Roll_Int16 = read_i16(Serial);

                int32_t Roll_Int32 = read_i32(Serial);
                int32_t Roll_Int32_lim = LimitValueInt32(Roll_Int32, Max, Min);
                ConvertedRoll = float(Roll_Int32_lim) / 1000;
                //std::cout << "\MEASURED_ROLL " << Roll_Int32_lim << " " << ConvertedRoll << std::endl;
                break;
            }
            case MEASURED_PITCH:
            {   //This sent as an int16_t
               // int16_t Pitch_Int16 = read_i16(Serial);

                int32_t Pitch_Int32 = read_i32(Serial);
                int32_t Pitch_Int32_lim = LimitValueInt32(Pitch_Int32, Max, Min);
                ConvertedPitch = float(Pitch_Int32_lim) / 1000;
                //std::cout << "\MEASURED_PITCH " << Pitch_Int32_lim << " " << ConvertedPitch << std::endl;
                break;
            }
            }
        }

    }

}

void ArduinoReceiver::ReadPitchRoll() {
    
    ReadPitchRoll(Ard);
}

void ArduinoReceiver::RequestPitch()
{
    //RequestData(Ard, REQUEST_PITCH);

    RequestReadData(Ard, (int)REQUEST_PITCH);
}

void ArduinoReceiver::SendPCReadiness()
{
    RequestReadData(Ard, (int)BOARD_FOUND);
}

void ArduinoReceiver::ReadPitchRoll(static bool& ValidRoll, static bool& ValidPitch, static bool& FirstReading)
{
    
    //Send command
    //Peek and read

    //if (FirstReading || ValidRoll)
    //{
        
        RequestData(Ard, REQUEST_PITCH);
        PeekAndRead(Ard, MEASURED_PITCH, ValidPitch);
        
        //if(FirstReading && ValidPitch)
          //FirstReading = 0;
        
        //ValidRoll = 0;
        //std::cout << "\n Valid Pitch " << ValidPitch;
    //}
    //std::cout << "\n Valid Pitch " << ValidPitch;
    /*if (ValidPitch)
    {
        RequestData(Ard, REQUEST_ROLL);
        PeekAndRead(Ard, MEASURED_ROLL, ValidRoll);
        ValidPitch = 0;
        std::cout << "\n Valid Roll " << ValidRoll;
    }*/
    //std::cout << "\n Valid Pitch " << ValidPitch;
}

void  ArduinoReceiver::ReadArduino3Accel2Attitude() {

    RequestReadData(Ard, REQUEST_ROLL, true);
    ReadBuffer(Ard, REQUEST_ROLL, ValidCommandRoll);

    if (ValidCommandRoll) {
        ValidRoll = true;
    }


    if (ValidRoll) {
        RequestReadData(Ard, REQUEST_PITCH, ValidRoll);
        ReadBuffer(Ard, REQUEST_PITCH, ValidCommandPitch);
    }

    /*if (ValidCommandPitch);
     ValidPitch = true;

    if (ValidPitch);
     ValidAccelX = true;*/

    //if (ValidAccelX) {

        RequestReadData(Ard, REQUEST_ACCEL_X, ValidCommandAccelX);
        ReadBuffer(Ard, REQUEST_ACCEL_X, ValidCommandAccelX);
    //}

    if (ValidCommandAccelX);
     ValidAccelY = true;

    if (ValidAccelY) {

        RequestReadData(Ard, REQUEST_ACCEL_Y, ValidCommandAccelY);
        ReadBuffer(Ard, REQUEST_ACCEL_Y, ValidCommandAccelY);
    }

    if (ValidCommandAccelY);
     ValidAccelZ = true;

    if (ValidAccelZ) {

        RequestReadData(Ard, REQUEST_ACCEL_Z, ValidCommandAccelZ);
        ReadBuffer(Ard, REQUEST_ACCEL_Z, ValidCommandAccelZ);

    }

    if (ValidCommandAccelZ);
     ValidTime = true;

    if (ValidTime) {

       // RequestReadData(Ard, REQUEST_TIME, ValidCommandTime);
        //ReadBuffer(Ard, REQUEST_TIME, ValidCommandTime);

    }

}

void ArduinoReceiver::ReadRadar() {

    if (ValidCommandYaw) {
        RequestReadData(Ard, REQUEST_RADAR);
        ReadBuffer(Ard, REQUEST_RADAR, ValidRadarVal);
    }
    if (ValidRadarVal) {
        RequestReadData(Ard, REQUEST_RADAR_POS);
        ReadBuffer(Ard, REQUEST_RADAR_POS, ValidRadarPos);
    }

}

void ArduinoReceiver::ReadRadar2() {
    
    RequestReadData(Ard, REQUEST_RADAR);
    ReadBuffer(Ard, REQUEST_RADAR, ValidRadarVal);


   if (ValidRadarVal) {

      ValidRadarValue = true;

   }

   if (ValidRadarValue) {
       RequestReadData(Ard, REQUEST_RADAR_POS);
       ReadBuffer(Ard, REQUEST_RADAR_POS, ValidRadarPos);
   }
}

void ArduinoReceiver::ReadRadarDefaultPort() {

    RequestReadData(Ard, REQUEST_RADAR);
    ReadBuffer(Ard, REQUEST_RADAR, ValidRadarVal);


    if (ValidRadarVal) {

        ValidRadarValue = true;

    }

    if (ValidRadarValue) {
        RequestReadData(Ard, REQUEST_RADAR_POS);
        ReadBuffer(Ard, REQUEST_RADAR_POS, ValidRadarPos);
    }

}

void ArduinoReceiver::ReadAllVals() {

    ///RequestReadDataFirstRequest(arduino, REQUEST_PITCH, FirstPass);
    RequestReadData(Ard, REQUEST_PITCH, ValidCommandPitch);
    ReadBuffer(Ard, REQUEST_PITCH, ValidCommandPitch);

    if (ValidCommandPitch)
        ValidPitch = true;

    if (ValidPitch) {
        //cout << "VALID ROLL " << ValidRoll;
        RequestReadData(Ard, REQUEST_ROLL, ValidCommandRoll);
        ReadBuffer(Ard, REQUEST_ROLL, ValidCommandRoll);
    }

    if (ValidCommandRoll);
    ValidRoll = true;

    if (ValidRoll) {

        RequestReadData(Ard, REQUEST_YAW, ValidCommandYaw);
        ReadBuffer(Ard, REQUEST_YAW, ValidCommandYaw);
    }

    if (ValidCommandYaw) {

        ValidYaw = true;
    }

    if (ValidYaw) {
        RequestReadData(Ard, REQUEST_RADAR);
        ReadBuffer(Ard, REQUEST_RADAR, ValidRadarVal);
    }

    if (ValidRadarVal) {

        ValidRadarValue = true;

    }

    if (ValidRadarValue) {
        RequestReadData(Ard, REQUEST_RADAR_POS);
        ReadBuffer(Ard, REQUEST_RADAR_POS, ValidRadarPos);
    }
}

void ArduinoReceiver::SendCommandI8(SerialOrder CommandType, int8_t Command) {

    SerialOrder ReceivedType;
    bool ExpectedCommand=0;

    if (Ard.isConnected()) {

        ReceivedType = read_order(Ard);
        //cout << "\nENUM RECEVIED " << ReceivedType << endl;

        if ((Command - 10) == ReceivedType) {
            ExpectedCommand = true;
        }
        else
            ExpectedCommand = false;

        if (ExpectedCommand) {

            WriteCommandI8(Ard, CommandType);
            WriteCommandI8(Ard, Command);
            //std::cout << "\nENUM RECEVIED " << ReceivedType << std::endl;
        }

    }

}

bool ArduinoReceiver::MonitorArduinoReadiness()
{
   bool ArduinoReady = RequestedCommandReceived(ARDUINO_READY);

   //std::cout << "\n Comms established " << ArduinoReady;



   return ArduinoReady;
};
void ArduinoReceiver::SignalPCReadiness()
{
    RequestReadData(Ard, PC_READY);
    //RequestReadData(Ard, 5);
};

void ArduinoReceiver::EstablishComms(static bool &CommsEstablished)
{
    SignalPCReadiness();
    //CommsEstablished = MonitorArduinoReadiness();

    CommsEstablished = PeekForData(ARDUINO_READY);

    //std::cout << "\n Comms established " << CommsEstablished;

}

void ArduinoReceiver::EstablishComms(static bool& CommsEstablished, std::vector<std::string>& Packets)
{
    //SignalPCReadiness();
    //CommsEstablished = MonitorArduinoReadiness();
    //CommsEstablished = PeekForData(ARDUINO_READY);

    CircularBuffer<unsigned char> CircularBufferIn{ 50 };

    ReadIntoBuffer(CircularBufferIn);
    CommsEstablished = ParseBufferForItem(CircularBufferIn, Packets, ARDUINO_READY);
    
    /*if (CommsEstablished)
        std::cout << "\n Comms established Board type " << ArduinoType;
    else
        std::cout << "\n No comms available Board type " << ArduinoType;*/
}

void ArduinoReceiver::ListenForArduinoReadiness(static bool& ArduinoCommsEstablished, std::vector<std::string>& Packets)
{
    CircularBuffer<unsigned char> CircularBufferIn{ 50 };

    ReadIntoBuffer(CircularBufferIn);
    ArduinoCommsEstablished = ParseBufferForItem(CircularBufferIn, Packets, ARD_COMMS_EST);
}

bool ArduinoReceiver::ListenForRequest(SerialOrder order)
{
    std::vector<std::string> DataPackets;
    CircularBuffer<unsigned char> CircularBufferIn{ 50 };

    ReadIntoBufferMT(CircularBufferIn);

    return ParseBufferForItem2(CircularBufferIn, DataPackets, order);
}

void ArduinoReceiver::SendHeartBeat(DataConcentrator& DC)
{
 /*
 * Heartbeat not sent, amber if not sent, no messages indicating successfully sent HB. 
 * 3001 Gyroscope
 * 4001 Radar
 * 5001 Motor
 * 
 * Will also determine ardunio connected or not Red for not connected, cyan connected
 * 3002 Gyroscope, 3003 Connected
 * 4002 Radar, 4003 Connected
 * 5002 Motor, 5003 Connected
 * 
 */

    /*bool Transfer;
    char buff[1] = { PC_HEARTBEAT };
    Transfer = Ard.writeSerialPort(buff, 1);
    if(!Transfer)
        std::cout << "\n FAILED TO SEND DATA  " ;*/

    char dataschar = (char)PC_HEARTBEAT;
    bool Transfer;

    char buff[3] = { START_MARKER, dataschar, END_MARKER };
    Transfer = Ard.writeSerialPort(buff, 3);

    SignalPCReadiness();

    /*
    *  Messaging
    */

    std::string MessageHeartBeatCaution{ "12:Caution, heart beat not sent " };
    std::string Board = GetBoardMessage();

    std::string MessageHeartBeatCautionComplete = MessageHeartBeatCaution + Board;

    int MessageCodeHB = GetBoardMessageCode() + 1;

        if (!Transfer)
        {
            DC.SetMessageStatus(MessageHeartBeatCautionComplete, MessageCodeHB, MESSAGE_ON);
        }
        else
        {
            DC.SetMessageStatus(MessageHeartBeatCautionComplete, MessageCodeHB, MESSAGE_OFF);
        }

    std::string MessageArduinoConnectionOFF={ "11:Warning, Arduino not connected " };
    std::string MessageCompleteArduinoConnectionOFF = MessageArduinoConnectionOFF + Board;
    int MessageCodeConnectionOFF = GetBoardMessageCode() + 2;

    std::string MessageArduinoConnectionON = { "13:Arduino connected " };
    std::string MessageCompleteArduinoConnectionON = MessageArduinoConnectionON + Board;
    int MessageCodeConnectionON = GetBoardMessageCode() + 3;

        if (!Ard.isConnected())
        {
            DC.SetMessageStatus(MessageCompleteArduinoConnectionOFF, MessageCodeConnectionOFF, MESSAGE_ON);
            DC.SetMessageStatus(MessageCompleteArduinoConnectionON, MessageCodeConnectionON, MESSAGE_OFF);

        }
        else
        {
            DC.SetMessageStatus(MessageCompleteArduinoConnectionOFF, MessageCodeConnectionOFF, MESSAGE_OFF);
            DC.SetMessageStatus(MessageCompleteArduinoConnectionON, MessageCodeConnectionON, MESSAGE_ON);
        }


    //std::cout << "\nmesage to be sent " << buff[0] << " " << buff[1] << " " << buff[2];
}

void ArduinoReceiver::SendMessageToArd(char command)
{
    char buff[5] = { START_MARKER, command, END_MARKER };
    bool Transfer = Ard.writeSerialPort(buff, 3);
}

void ArduinoReceiver::SendMessageToArd(SerialOrder Type, int command)
{

    char CommandType = (char)Type;
    bool Transfer;

    char Command = (char)command;

    char buff[5] = { START_MARKER, CommandType, ':', Command, END_MARKER};
    //char buff[3] = { START_MARKER, CommandType, END_MARKER };
    Transfer = Ard.writeSerialPort(buff, 5);
}

void ArduinoReceiver::SendMessageToArdTotal(SerialOrder Type, int command, static DWORD& lastSendTime)
{
    bool Transfer,
         Transfer2,
         Transfer3;
    
    char CommandType = (char)Type;
    char Command = (char)command;

    char HeartBeat = (char)PC_HEARTBEAT;
    char ReadyInd = (char)PC_READY;

    DWORD currentTime = GetTickCount64();

    //char buff[] = { START_MARKER, HeartBeat, ',', ReadyInd, ',', CommandType, ':', Command, END_MARKER };

    //char buffHB[] = { START_MARKER, HeartBeat, END_MARKER };
    //char buffReadyInd[] = { START_MARKER, ReadyInd, END_MARKER };
    //char buff[] = { START_MARKER, CommandType, ':', Command, END_MARKER };

    char buff[] = { START_MARKER, HeartBeat, ReadyInd, CommandType, ':', Command, END_MARKER };
    //char buff[5] = { START_MARKER, CommandType, ':', Command, END_MARKER };
    //char buff[3] = { START_MARKER, CommandType, END_MARKER };

   /* std::string packet = std::string(1, START_MARKER)
        + std::to_string(PC_HEARTBEAT) + ","
        + std::to_string(PC_READY) + ","
        + (char)Type + ":"
        + std::to_string(command)
        + END_MARKER;*/

    //Transfer = Ard.writeSerialPort((char*)packet.c_str(), packet.length());

    if (currentTime - lastSendTime >= 20)
    {
        lastSendTime = currentTime;


        //Transfer = Ard.writeSerialPort((char*)packet.c_str(), packet.length());
        //std::cout << "\n50 MS passed";

        //Transfer2 = Ard.writeSerialPort(buffHB, sizeof(buffHB));
        //Transfer3 = Ard.writeSerialPort(buffReadyInd, sizeof(buffReadyInd));
        Transfer = Ard.writeSerialPort(buff, sizeof(buff));



        //Transfer = Ard.writeSerialPort((char*)packet.c_str(), packet.length());
    }


    //Transfer = Ard.writeSerialPort(buff, 9);
}

void ArduinoReceiver::SendMessageToArdTotal(SerialOrder Type, int command, SerialOrder Type2, int command2, SerialOrder Type3, int command3, static DWORD& lastSendTime)
{
    bool Transfer,
        Transfer2,
        Transfer3;

    char CommandType = (char)Type;
    char Command = (char)command;

    char CommandType2 = (char)Type2;
    char Command2 = (char)command2;

    char CommandType3 = (char)Type3;
    char Command3 = (char)command3;

    char HeartBeat = (char)PC_HEARTBEAT;
    char ReadyInd = (char)PC_READY;

    DWORD currentTime = GetTickCount64();


    char buff[] = { START_MARKER, HeartBeat, ReadyInd, CommandType, ':', Command, CommandType2, ':', Command2, CommandType3, ':', Command3, END_MARKER };


   /* if (currentTime - lastSendTime >= 50)
    {
        lastSendTime = currentTime;

        Transfer = Ard.writeSerialPort(buff, sizeof(buff));

        //std::cout << "\nTimeout, MSB ";

    }*/

    Transfer = Ard.writeSerialPort(buff, sizeof(buff));

}

void ArduinoReceiver::SendMessageToArdTotal(SerialOrder Type, int command, SerialOrder Type2, int command2, static DWORD& lastSendTime)
{
    bool Transfer,
        Transfer2,
        Transfer3;

    char CommandType = (char)Type;
    char Command = (char)command;

    char CommandType2 = (char)Type2;
    char Command2 = (char)command2;



    char HeartBeat = (char)PC_HEARTBEAT;
    char ReadyInd = (char)PC_READY;

    DWORD currentTime = GetTickCount64();


    char buff[] = { START_MARKER, HeartBeat, ReadyInd, CommandType, ':', Command, CommandType2, ':', Command2, END_MARKER };


    /* if (currentTime - lastSendTime >= 50)
     {
         lastSendTime = currentTime;

         Transfer = Ard.writeSerialPort(buff, sizeof(buff));

         //std::cout << "\nTimeout, MSB ";

     }*/

    Transfer = Ard.writeSerialPort(buff, sizeof(buff));

}

bool ArduinoReceiver::ListenForHeartBeat(const char &HeatBeatCharacter)
{
    bool HeartBeat;
    /*char BufferIn[MAX_BUFFER];

    unsigned char incomingbyte ;
    incomingbyte = Ard.Peek(BufferIn);
    int arda = Ard.ReadSerialPortAfterPeek(BufferIn, 1);

    

    //std::cout << "\nHeartbeat read "<< incomingbyte;
    //std::cout << "\nHeartbeat read " << BufferIn[0];

    if (HeatBeatCharacter == incomingbyte)
    {
        //int arda = Ard.ReadSerialPortAfterPeek(BufferIn, 1);
        //std::cout << "\nHeartbeat Detected! ";
        return 1;
    }
    else
        return 0;*/
    
    CircularBuffer<unsigned char> CircularBufferIn{ 50 };
    std::vector<std::string>Packets;

    const char* HBChar = &HeatBeatCharacter;

    ReadIntoBuffer(CircularBufferIn);
    HeartBeat = ParseBufferForItem(CircularBufferIn, Packets, HBChar);

    return HeartBeat;
}

bool ArduinoReceiver::PeekForData(unsigned char DataToCheck)
{
    char BufferIn[MAX_BUFFER];

    unsigned char incomingbyte;
    incomingbyte = Ard.Peek(BufferIn);
    int arda = Ard.ReadSerialPortAfterPeek(BufferIn, 1);

    //std::cout << "\nHeartbeat read "<< incomingbyte;
    //std::cout << "\nHeartbeat read " << BufferIn[0];

    if (DataToCheck == incomingbyte)
    {
        //int arda = Ard.ReadSerialPortAfterPeek(BufferIn, 1);
        //std::cout << "\nHeartbeat Detected! ";
        return 1;
    }
    else
        return 0;
}

bool ArduinoReceiver::PeekForData(unsigned char DataToCheck,  int& data)
{
    char BufferIn[MAX_BUFFER];

    unsigned char incomingbyte;
    incomingbyte = Ard.Peek(BufferIn);
    data = Ard.ReadSerialPortAfterPeek(BufferIn, 1);

    //std::cout << "\nPeeked read "<< incomingbyte;
    //std::cout << "\nData read " << BufferIn[0];

    if (DataToCheck == incomingbyte)
    {
        //int arda = Ard.ReadSerialPortAfterPeek(BufferIn, 1);
        //std::cout << "\nHeartbeat Detected! ";
        return 1;
    }
    else
        return 0;
}

void ArduinoReceiver::ReadIntoBuffer()
{
    char BufferIn[MAX_BUFFER];

    int BytesRead = Ard.readSerialPort(BufferIn, MAX_BUFFER);

    //std::cout << "\nBytes Read number "<< BytesRead;
    
    
    if (BytesRead > 0) 
    {
        std::lock_guard<std::mutex> lock(MutexArd);

        for (int i = 0; i < BytesRead; i++)
        {
            //std::string ConvertedToStringVal = std::to_string(BufferIn[i]);
            //std::cout << "\nString Read " << ConvertedToStringVal;

            //std::cout << "\n Data in buffer  " << BufferIn[i];
            CircularBufferIn.WriteByte((BufferIn[i]));
        }

        int SizeofCB = CircularBufferIn.size();

        //std::cout << "\n Size of CB " << SizeofCB;
        //std::cout << "\n Is CB full ? "<< CircularBufferIn.IsFull();

    }
    else
    {
        std::cout << "\n Bytes Read is 0";
    }

}

void ArduinoReceiver::ParseBuffer()
{
    if (CircularBufferIn.size() >= 2)
    {
        if (CircularBufferIn.peek(0) != '[')
        {
            //Start marker has not been found
            CircularBufferIn.ReadByte();
            std::cout << "\nStart not found ";
            return;
        }

        int EndMarkerIndex = -1;
        unsigned int BuffSize = CircularBufferIn.size();

        for (unsigned int i = 0; i < BuffSize; i++)
        {
            if (CircularBufferIn.peek(i) == ']')
                EndMarkerIndex = i;
        }

        if (EndMarkerIndex == -1)
        { 
            std::cout << "\nEnd has not been found ";
            return;
        }

        std::string DataPacket;

        for (int i = 0; i < EndMarkerIndex - 1; i++)
        {
            //Fill in data
            DataPacket.push_back(CircularBufferIn.ReadByte());
        }

        //Remove the endmarker
        CircularBufferIn.ReadByte();

        if(DataPacket.size() > 0)
        { 
            //std::cout << "\nData packet " << DataPacket;
        }


    }


}

bool ArduinoReceiver::ParseBufferForItem(CircularBuffer< unsigned char >& Buff, std::vector<std::string>& Packets, SerialOrder Order)
{//added
    bool run = true,
         ArdReady = false;

    int ArdReadySignals = 0;

    while (run)
    {
        if (Buff.size() > 2)
        {

            int EndMarkerIndex = -1,
                StartMarkerIndex = -1;

            unsigned int BuffSize = Buff.size();
            //std::cout << "\nSize of buffer at the start  " << Buff.size();


            /*for (int i = Buff.size() - 1; i > -1; i--)
            {
                std::cout << "\n In buffer at index  " << i << " " << Buff.peek(i);
            }*/



            for (int i = Buff.size() - 1; i > -1; i--)
            {
                //std::cout << "\n In buffer at index  " << i << " " << Buff.peek(i);

                if (Buff.peek(i) == ']')
                {
                    //std::cout << "\nStart at index  " << i;
                    StartMarkerIndex = i;
                    //Remove the Startmarker
                    //if (Buff.size() > 0)
                        //Buff.ReadByte();
                    break;
                }
                //if (Buff.size() > 0)
                    //Buff.ReadByte();
            }

            unsigned int BuffSize2 = Buff.size();
            //std::cout << "\n Size of CB2 " << BuffSize2;

            if (Buff.size() > 0)
            {
                for (int i = Buff.size() - 1; i > -1; i--)
                {
                    if (Buff.peek(i) == '[' && i < StartMarkerIndex) //Must come after
                    {
                        //std::cout << "\nEnd marker found at " << i;
                        EndMarkerIndex = i;
                        break;           // Exit the processing block cleanly
                    }
                }
            }

            std::string DataPacket;

            if (Buff.size() > 0)
            {         
                for (int i = StartMarkerIndex - 1 ; i > EndMarkerIndex; i--)
                {
                    if (!Buff.IsEmpty())
                    {
                        //std::cout << "\n Packing Data  " << i << " " << Buff.peek(i);
                        DataPacket.push_back(Buff.peek(i));
                    }
                }
                //now consume
                for (int i = StartMarkerIndex; i >= EndMarkerIndex; i--)
                {
                    if (!Buff.IsEmpty())
                    {
                        char data = Buff.ReadByte();
                        //std::cout << "\n Consuming at index  " << i << " Data consumed " << data;
                    }
                }
            }

            /*

            if (!DataPacket.empty())
            {
                for (int i = 0; i < DataPacket.size(); i++)
                {
                    std::cout << "\nData packet " << DataPacket[i];
                }
            }
            else
            {
                std::cout << "\nData packet empty ";
            }
            
            std::cout << "\nBuffer size  END "<<Buff.size();
            */
            //std::string DataPacket;

            /*for (int i = Buff.size() - 1; i > -1; i--)
            {
                if (Buff.peek(i) == '[' || Buff.peek(i) == ']')
                {
                    if (Buff.size() > 0)
                        Buff.ReadByte();
                    break;
                }
                

                if (Buff.size() > 2)
                {
                    
                    if (Buff.peek(i) == '?')
                    {
                        Buff.ReadByte();
                        std::cout << "\nHB DETECTED ";
                    }

                    unsigned char data = Buff.peek(i);

                    Buff.ReadByte();
                    //std::cout << "\nPacking buffer, for loop number at: " << i << " Buff size " << Buff.size() << " Buff content " << Buff.peek(i) << " Buff at data " << data;
                    
                    bool isitadigit = std::isdigit(data);

                    if (isitadigit)
                      DataPacket.push_back(data);
                }
            }


            //std::cout << "\n Size of CB3 " << Buff.size() << " Data packet size " << DataPacket.size();

            std::reverse(DataPacket.begin(), DataPacket.end());
            
            if (DataPacket.size() > 0)
            {
                

                for (int i = 0; i < DataPacket.size(); i++)
                {
                   // std::cout << "\nData packet " << DataPacket[i];
                }

            }*/

            Packets.push_back(DataPacket);

        }
        else
        {
            run = false;

        }



        if (Packets.size() > 0)
        {
            for (int i = 0; i < Packets.size(); i++)
            {
                int value = 0;

                if (!Packets.empty())
                {
                    if (!Packets[i].empty())
                    { 
                        //value = std::stoi(Packets[i]);
                        //Packets[i].re
                        std::reverse(Packets[i].begin(), Packets[i].end());
                        char Data;
                        Data = (char)Packets[i].at(0);
                        bool isitadigit = std::isdigit(Data);

                        if(isitadigit)
                            value = std::stoi(Packets[i]);

                        //std::cout << "\nPacket value  " << Packets[i] <<" at "<<i<<" digit detected "<< isitadigit<<" As an int "<< value;
                        //value = std::stoi(Packets[i]);
                    }

                    if (value == ARDUINO_READY)  //(value == ARDUINO_READY)
                        ArdReadySignals++;
                }

            }
        }
        if (ArdReadySignals > 1)
            ArdReady = 1;

        //std::cout << "\n Arduino ready signals " << ArdReadySignals <<" "<< ArduinoType;
    }
    
 
    return ArdReady;

    
}

bool ArduinoReceiver::ParseBufferForItem2(CircularBuffer< unsigned char >& Buff, std::vector<std::string>& Packets, SerialOrder Order)
{//added
    bool run = true,
        ArdReady = false;

    int ArdReadySignals = 0;

    while (run)
    {
        if (Buff.size() > 2)
        {

            int EndMarkerIndex = -1,
                StartMarkerIndex = -1;

            unsigned int BuffSize = Buff.size();
            //std::cout << "\nSize of buffer at the start  " << Buff.size();


            for (int i = Buff.size() - 1; i > -1; i--)
            {
                //std::cout << "\n In buffer at index  " << i << " " << Buff.peek(i);
            }



            for (int i = Buff.size() - 1; i > -1; i--)
            {
                //std::cout << "\n In buffer at index  " << i << " " << Buff.peek(i);

                if (Buff.peek(i) == ']')
                {
                    //std::cout << "\nStart at index  " << i;
                    StartMarkerIndex = i;
                    //Remove the Startmarker
                    //if (Buff.size() > 0)
                        //Buff.ReadByte();
                    break;
                }
                //if (Buff.size() > 0)
                    //Buff.ReadByte();
            }

            unsigned int BuffSize2 = Buff.size();
            //std::cout << "\n Size of CB2 " << BuffSize2;

            if (Buff.size() > 0)
            {
                for (int i = Buff.size() - 1; i > -1; i--)
                {
                    if (Buff.peek(i) == '[' && i < StartMarkerIndex) //Must come after
                    {
                        //std::cout << "\nEnd marker found at " << i;
                        EndMarkerIndex = i;
                        break;           // Exit the processing block cleanly
                    }
                }
            }

            std::string DataPacket;

            if (Buff.size() > 0)
            {
                for (int i = StartMarkerIndex - 1; i > EndMarkerIndex; i--)
                {
                    if (!Buff.IsEmpty())
                    {
                        //std::cout << "\n Packing Data  " << i << " " << Buff.peek(i);
                        DataPacket.push_back(Buff.peek(i));
                    }
                }
                //now consume
                for (int i = StartMarkerIndex; i >= EndMarkerIndex; i--)
                {
                    if (!Buff.IsEmpty())
                    {
                        char data = Buff.ReadByte();
                        //std::cout << "\n Consuming at index  " << i << " Data consumed " << data;
                    }
                }
            }

            /*

            if (!DataPacket.empty())
            {
                for (int i = 0; i < DataPacket.size(); i++)
                {
                    std::cout << "\nData packet " << DataPacket[i];
                }
            }
            else
            {
                std::cout << "\nData packet empty ";
            }

            std::cout << "\nBuffer size  END "<<Buff.size();
            */
            //std::string DataPacket;

            /*for (int i = Buff.size() - 1; i > -1; i--)
            {
                if (Buff.peek(i) == '[' || Buff.peek(i) == ']')
                {
                    if (Buff.size() > 0)
                        Buff.ReadByte();
                    break;
                }


                if (Buff.size() > 2)
                {

                    if (Buff.peek(i) == '?')
                    {
                        Buff.ReadByte();
                        std::cout << "\nHB DETECTED ";
                    }

                    unsigned char data = Buff.peek(i);

                    Buff.ReadByte();
                    //std::cout << "\nPacking buffer, for loop number at: " << i << " Buff size " << Buff.size() << " Buff content " << Buff.peek(i) << " Buff at data " << data;

                    bool isitadigit = std::isdigit(data);

                    if (isitadigit)
                      DataPacket.push_back(data);
                }
            }


            //std::cout << "\n Size of CB3 " << Buff.size() << " Data packet size " << DataPacket.size();

            std::reverse(DataPacket.begin(), DataPacket.end());

            if (DataPacket.size() > 0)
            {


                for (int i = 0; i < DataPacket.size(); i++)
                {
                   // std::cout << "\nData packet " << DataPacket[i];
                }

            }*/

            Packets.push_back(DataPacket);

        }
        else
        {
            run = false;

        }



        if (Packets.size() > 0)
        {
            for (int i = 0; i < Packets.size(); i++)
            {
                int value = 0;

                if (!Packets.empty())
                {
                    if (!Packets[i].empty())
                    {
                        //value = std::stoi(Packets[i]);
                        //Packets[i].re
                        std::reverse(Packets[i].begin(), Packets[i].end());
                        char Data;
                        Data = (char)Packets[i].at(0);
                        bool isitadigit = std::isdigit(Data);

                        if (isitadigit)
                            value = std::stoi(Packets[i]);

                        //std::cout << "\nPacket value  " << Packets[i] <<" at "<<i<<" digit detected "<< isitadigit<<" As an int "<< value;
                        //value = std::stoi(Packets[i]);
                    }

                    if ((SerialOrder)value == Order)  //(value == ARDUINO_READY)
                    {
                       
                        ArdReadySignals++;
                    }
                }

            }
        }
        if (ArdReadySignals > 0)
            ArdReady = 1;

        //std::cout << "\n Arduino ready signals " << ArdReadySignals <<" "<< ArduinoType;
    }


    return ArdReady;


}


bool ArduinoReceiver::ParseBufferForItem(CircularBuffer< unsigned char >& Buff, std::vector<std::string>& Packets, std::string item)
{
    bool run = true,
        ItemDetected = false;

    int ItemInstances = 0;
    while (run)
    {
        if (Buff.size() > 1)
        {

            int EndMarkerIndex = -1,
                StartMarkerIndex = -1;

            unsigned int BuffSize = Buff.size();
            //std::cout << "\nSize of buffer at the start  " << Buff.size();


            for (int i = Buff.size() - 1; i > -1; i--)
            {
                //std::cout << "\n In buffer at index  " << i << " " << Buff.peek(i);
            }



            for (int i = Buff.size() - 1; i > -1; i--)
            {
                //std::cout << "\n In buffer at index  " << i << " " << Buff.peek(i);

                if (Buff.peek(i) == ']')
                {
                    //std::cout << "\nStart at index  " << i;
                    StartMarkerIndex = i;
                    //Remove the Startmarker
                    //if (Buff.size() > 0)
                        //Buff.ReadByte();
                    break;
                }
                //if (Buff.size() > 0)
                    //Buff.ReadByte();
            }

            unsigned int BuffSize2 = Buff.size();
            //std::cout << "\n Size of CB2 " << BuffSize2;

            if (Buff.size() > 0)
            {
                for (int i = Buff.size() - 1; i > -1; i--)
                {
                    if (Buff.peek(i) == '[' && i < StartMarkerIndex) //Must come after
                    {
                        //std::cout << "\nEnd marker found at " << i;
                        EndMarkerIndex = i;
                        break;           // Exit the processing block cleanly
                    }
                }
            }

            std::string DataPacket;

            if (Buff.size() > 0)
            {
                for (int i = StartMarkerIndex - 1; i > EndMarkerIndex; i--)
                {
                    //std::cout << "\n Packing Data  " << i << " " << Buff.peek(i);
                    DataPacket.push_back(Buff.peek(i));
                }
                //now consume
                for (int i = StartMarkerIndex; i >= EndMarkerIndex; i--)
                {
                    char data = Buff.ReadByte();
                    //std::cout << "\n Consuming at index  " << i << " Data consumed " << data;

                }
            }



            if (!DataPacket.empty())
            {
                for (int i = 0; i < DataPacket.size(); i++)
                {
                    //std::cout << "\nData packet " << DataPacket[i];
                }
            }
            else
            {
                //std::cout << "\nData packet empty ";
            }

            //std::cout << "\nBuffer size  END " << Buff.size();



            Packets.push_back(DataPacket);

        }
        else
        {
            run = false;

        }



        if (Packets.size() > 0)
        {
            for (int i = 0; i < Packets.size(); i++)
            {
                int value = 0;

                if (!Packets.empty())
                {
                    const char* data = Packets[i].c_str();
                    bool det =0 ;

                    if (strcmp(Packets[i].c_str(),item.c_str()))
                    {
                        ItemInstances++;
                        det = 1;
                    }
                    //std::cout << "\n Item and data " << item<<"  "<<data<<" "<<det;
                }
            }
        }
        if (ItemInstances > 1)
            ItemDetected = 1;
       //std::cout << "\n HB Instances " << ItemInstances;
    }
    return ItemDetected;
}

void ArduinoReceiver::ReadIntoBuffer(CircularBuffer< unsigned char >& Buff)
{
    const int BufSz = 15;
    char BufferIn[MAX_BUFFER_GYRO];

    int BytesRead = Ard.readSerialPort(BufferIn, MAX_BUFFER_GYRO);

    //std::cout << "\nBytes Read number "<< BytesRead;


    if (BytesRead > 0)
    {
        std::lock_guard<std::mutex> lock(MutexArd);

        for (int i = 0; i < BytesRead; i++)
        {
            //std::string ConvertedToStringVal = std::to_string(BufferIn[i]);
            //std::cout << "\nString Read " << ConvertedToStringVal;

            //std::cout << "\n Data added " << BufferIn[i];
            Buff.WriteByte((BufferIn[i]));
        }

        
        //std::cout << "\n Is CB full ? "<< Buff.IsFull();

    }
    else
    {
        //std::cout << "\n Bytes Read is 0";
    }
}

void ArduinoReceiver::ReadIntoBufferMT(CircularBuffer< unsigned char >& Buff)
{
    const int BufSz = 15;
    char BufferIn[MAX_BUFFER_GYRO];

    int BytesRead = Ard.readSerialPort(BufferIn, MAX_BUFFER_GYRO);

    //std::cout << "\nBytes Read number "<< BytesRead;


    if (BytesRead > 0)
    {
        std::lock_guard<std::mutex> lock(MutexArd);

        for (int i = 0; i < BytesRead; i++)
        {
            //std::string ConvertedToStringVal = std::to_string(BufferIn[i]);
            //std::cout << "\nString Read " << ConvertedToStringVal;

            //std::cout << "\n Data added " << BufferIn[i];
            Buff.WriteByte((BufferIn[i]));
        }


        //std::cout << "\n Is CB full ? "<< Buff.IsFull();

    }
    else
    {
        //std::cout << "\n Bytes Read is 0";
    }
}

void ArduinoReceiver::ReadIntoBufferT(CircularBuffer< unsigned char >& Buff)
{
    const int BufSz = 15;
    char BufferIn[MAX_BUFFER_GYRO];

    int BytesRead = Ard.readSerialPort(BufferIn, MAX_BUFFER_GYRO);

    //std::cout << "\nBytes Read number "<< BytesRead;


    if (BytesRead > 0)
    {
        std::lock_guard<std::mutex> lock(MutexArd);

        for (int i = 0; i < BytesRead; i++)
        {
            //std::string ConvertedToStringVal = std::to_string(BufferIn[i]);
            //std::cout << "\nString Read " << ConvertedToStringVal;

            //std::cout << "\n Data added " << BufferIn[i];
            Buff.WriteByte((BufferIn[i]));
        }


        //std::cout << "\n Is CB full ? "<< Buff.IsFull();

    }
    else
    {
        //std::cout << "\n Bytes Read is 0";
    }
}

void ArduinoReceiver::ParseBuffer(CircularBuffer< unsigned char >& Buff, bool& HeartBeat, std::vector<std::string>& Packets)
{
    bool run = true;

    while (run)
    {
        if (Buff.size() > 3)
        {

            int EndMarkerIndex = -1,
                StartMarkerIndex = -1;

            unsigned int BuffSize = Buff.size();
            //std::cout << "\n Size of CB " << BuffSize;

            for (int i = Buff.size() - 1; i > -1; i--)
            {
                //std::cout << "\n In buffer at index  " << i << " " << Buff.peek(i);

                if (Buff.peek(i) == ']')
                {
                    //std::cout << "\nEnd at index  " << i;
                    EndMarkerIndex = i;
                    //Remove the Startmarker
                    if (Buff.size() > 0)
                        Buff.ReadByte();
                    break;
                }
                //if (Buff.size() > 0)
                    //Buff.ReadByte();
            }

            for (int i = Buff.size() - 1; i > -1; i--)
            {
                //std::cout << "\n In buffer at index  " << i << " " << Buff.peek(i);
            }

            unsigned int BuffSize2 = Buff.size();
            //std::cout << "\n Size of CB2 " << BuffSize2;

            if (Buff.size() > 0)
            {
                for (int i = Buff.size() - 1; i > -1; i--)
                {
                    if (Buff.peek(i) == '[')
                    {
                        // std::cout << "\nStart marker found at " << i;
                        StartMarkerIndex = i;
                        break;           // Exit the processing block cleanly
                    }
                }
            }

            for (int i = Buff.size() - 1; i > -1; i--)
            {
                if (Buff.size() > 0) {
                    /* if (Buff.peek(i) == '[')
                         std::cout << "\n After first removal of initial data in buffer at index  " << i << " " << Buff.peek(i);*/
                }
            }

            for (int i = Buff.size() - 1; i > -1; i--)
            {
                if (Buff.size() > 0)
                {
                    if (Buff.peek(i) == '?')   //HeartBeat Monitoring
                    {
                        HeartBeat = 1;
                        //Buff.ReadByte();
                    }
                }
            }


            std::string DataPacket;

            for (int i = BuffSize2 - 1; i > -1; i--)
            {
                if (Buff.peek(i) == '[')
                {
                    if (Buff.size() > 0)
                        Buff.ReadByte();
                    break;
                }
                unsigned char data = Buff.peek(i);

                if (Buff.size() > 2)
                {
                    Buff.ReadByte();
                    //std::cout << "\nPacking buffer, for loop number at: " << i << " Buff size " << Buff.size() << " Buff content " << Buff.peek(i) << " Buff at data " << data;
                    DataPacket.push_back(data);
                }
            }

            for (int i = Buff.size() - 1; i > -1; i--)
            {
                //if (Buff.size() > 2)
                  //std::cout << "\nAfter removal of packet, in buffer at index  " << i << " " << Buff.peek(i);
            }


            //std::cout << "\n Size of CB3 " << Buff.size() << " Data packet size " << DataPacket.size();

            std::reverse(DataPacket.begin(), DataPacket.end());

            if (DataPacket.size() > 0)
            {
                for (int i = 0; i < DataPacket.size(); i++)
                {
                   //std::cout << "\nData packet " << DataPacket[i];
                }
            }

            Packets.push_back(DataPacket);

        }
        else
        {
            run = false;

        }
    }
}

void ArduinoReceiver::ParseBufferRad(CircularBuffer< unsigned char >& Buff, bool& HeartBeat, std::vector<std::string>& Packets)
{
    bool run = true;

    while (run)
    {
        if (Buff.size() > 3)
        {

            int EndMarkerIndex = -1,
                StartMarkerIndex = -1;

            unsigned int BuffSize = Buff.size();
            //std::cout << "\n Size of CB " << BuffSize;

            for (int i = Buff.size() - 1; i > -1; i--)
            {
                //std::cout << "\n In buffer at index  " << i << " " << Buff.peek(i);

                if (Buff.peek(i) == ']')
                {
                    //std::cout << "\nEnd at index  " << i;
                    EndMarkerIndex = i;
                    //Remove the Startmarker
                    if (Buff.size() > 0)
                        Buff.ReadByte();
                    break;
                }
                //if (Buff.size() > 0)
                    //Buff.ReadByte();
            }

            for (int i = Buff.size() - 1; i > -1; i--)
            {
                //std::cout << "\n In buffer at index  " << i << " " << Buff.peek(i);
            }

            unsigned int BuffSize2 = Buff.size();
            //std::cout << "\n Size of CB2 " << BuffSize2;

            if (Buff.size() > 0)
            {
                for (int i = Buff.size() - 1; i > -1; i--)
                {
                    if (Buff.peek(i) == '[')
                    {
                        // std::cout << "\nStart marker found at " << i;
                        StartMarkerIndex = i;
                        break;           // Exit the processing block cleanly
                    }
                }
            }

            for (int i = Buff.size() - 1; i > -1; i--)
            {
                if (Buff.size() > 0) {
                    /* if (Buff.peek(i) == '[')
                         std::cout << "\n After first removal of initial data in buffer at index  " << i << " " << Buff.peek(i);*/
                }
            }

            for (int i = Buff.size() - 1; i > -1; i--)
            {
                if (Buff.size() > 0)
                {
                    if (Buff.peek(i) == '?')   //HeartBeat Monitoring
                    {
                        HeartBeat = 1;
                        //Buff.ReadByte();
                    }
                }
            }


            std::string DataPacket;

            for (int i = Buff.size() - 1; i > -1; i--)
            {
                if (Buff.peek(i) == '[')
                {
                    if (Buff.size() > 0)
                        Buff.ReadByte();
                    break;
                }
                unsigned char data = Buff.peek(i);

                if (Buff.size() > 1)
                {
                    if (!Buff.IsEmpty()) 
                    {
                        Buff.ReadByte();
                        //std::cout << "\nPacking buffer, for loop number at: " << i << " Buff size " << Buff.size() << " Buff content " << Buff.peek(i) << " Buff at data " << data;
                        DataPacket.push_back(data);
                    }
                }
            }

            for (int i = Buff.size() - 1; i > -1; i--)
            {
                //if (Buff.size() > 2)
                  //std::cout << "\nAfter removal of packet, in buffer at index  " << i << " " << Buff.peek(i);
            }


            //std::cout << "\n Size of CB3 " << Buff.size() << " Data packet size " << DataPacket.size();

            std::reverse(DataPacket.begin(), DataPacket.end());

            if (DataPacket.size() > 0)
            {
                //for (int i = 0; i < DataPacket.size(); i++)
                //{
                    //std::cout << "\nData packet " << DataPacket;
                //}
            }

            Packets.push_back(DataPacket);

        }
        else
        {
            run = false;

        }
    }
}

void ArduinoReceiver::ParseBufferRadT(CircularBuffer< unsigned char >& Buff, std::vector<std::string>& Packets)
{
    bool run = true;
    std::string DataPacket;

    while (run)
    {
        bool FoundStart = false;
        

        if (Buff.size() > 3)
        {

            int EndMarkerIndex = -1,
                StartMarkerIndex = -1;

            unsigned int BuffSize = Buff.size();
            //std::cout << "\n Size of CB " << BuffSize;

            char c = Buff.ReadByte();
            std::cout << "\n Read " << c;

            if (c == START_MARKER)
            {
                FoundStart = 1;

                if (!DataPacket.empty())
                {
                    DataPacket.clear();
                }
            }
            else if (FoundStart)
            {
                DataPacket.push_back(c);

                if (c == END_MARKER)
                {
                    Packets.push_back(DataPacket);
                    FoundStart = 0;
                }
            }
            
        }
        else
        {
            run = false;

        }

        if (!Packets.empty())
        {
            for (int i = 0; i < Packets.size(); i++)
            {
                
                /*if (!Packets[i].empty() && std::all_of(Packets[i].begin(), Packets[i].end(), ::isdigit))
                {
                    if ((SerialOrder)std::stoi(Packets[i]) == ARD_COMMS_EST)
                        BoardReadiness = 1;
                }*/

                std::cout << "\nData packet in vector " << Packets[i]<<" at "<<i <<" Ard type "<< ArduinoType;

            }
        }
        else
        {
            //std::cout << "\nEMPTY ";
        }
    }
}

void ArduinoReceiver::ParseBufferRadT(CircularBuffer< unsigned char >& Buff, bool& HeartBeat, bool& BoardReadiness, std::vector<std::string>& Packets)
{
    bool run = true;
    std::string DataPacket;
    bool FoundStart = false;

    while (run)
    {
       


        if (Buff.size() > 3)
        {

            int EndMarkerIndex = -1,
                StartMarkerIndex = -1;

            unsigned int BuffSize = Buff.size();
            //std::cout << "\n Size of CB " << BuffSize;

            char c = Buff.ReadByte();
            //std::cout << "\n Read " << c;

            if (c == '?')
            {
                HeartBeat = 1;
            }

            if (c == START_MARKER)
            {
                FoundStart = 1;

                //std::cout << "\nStrt found ";

                if (!DataPacket.empty())
                {
                    DataPacket.clear();
                }
            }
            else if (FoundStart)
            {
                
                if (c != END_MARKER)
                  DataPacket.push_back(c);

                //std::cout << "\nAdding "<<c;

                if (c == END_MARKER)
                {
                    Packets.push_back(DataPacket);
                    FoundStart = 0;
                    //std::cout << "\nAdded ";
                }
            }

        }
        else
        {
            run = false;

        }

        if (!Packets.empty())
        {
            for (int i = 0; i < Packets.size(); i++)
            {

                if (!Packets[i].empty() && std::all_of(Packets[i].begin(), Packets[i].end(), ::isdigit))
                {
                    if ((SerialOrder)std::stoi(Packets[i]) == ARD_COMMS_EST)
                        BoardReadiness = 1;
                }

                //std::cout << "\nData packet in vector " << Packets[i] << " at " << i << " Ard type " << ArduinoType;

            }
        }
        else
        {
            //std::cout << "\nEMPTY ";
        }
    }

    //std::cout << "\nPackets size "<< Packets.size();
    if (!Packets.empty())
    {
        for (int i = 0; i < Packets.size(); i++)
        {
            std::cout << "\nData packet in vector " << Packets[i];
            /*
            if (Packets[i].find("Nan"))
            {
                std::cout << "\nNan ";
            }
            */
        }
    }
    else
    {
        std::cout << "\nEMPTY GYRO PACKS ";
    }
}

void ArduinoReceiver::ParseBufferRad(CircularBuffer< unsigned char >& Buff, bool& HeartBeat, bool& BoardReadiness, std::vector<std::string>& Packets)
{
    bool run = true;

    while (run)
    {
        if (Buff.size() > 3)
        {

            int EndMarkerIndex = -1,
                StartMarkerIndex = -1;

            unsigned int BuffSize = Buff.size();
            //std::cout << "\n Size of CB " << BuffSize;

            for (int i = Buff.size() - 1; i > -1; i--)
            {
                //std::cout << "\n In buffer at index  " << i << " " << Buff.peek(i);


                //if (Buff.size() > 0)
            }

            for (int i = Buff.size() - 1; i > -1; i--)
            {
               // std::cout << "\n In buffer at index  " << i << " " << Buff.peek(i);

                if (Buff.peek(i) == ']')
                {
                    //std::cout << "\nEnd at index  " << i;
                    EndMarkerIndex = i;
                    //Remove the Startmarker
                    if (Buff.size() > 0)
                        Buff.ReadByte();
                    break;
                }
                //if (Buff.size() > 0)
                    //Buff.ReadByte();
            }

            for (int i = Buff.size() - 1; i > -1; i--)
            {
                //std::cout << "\n In buffer at index  " << i << " " << Buff.peek(i);
            }

            unsigned int BuffSize2 = Buff.size();
            //std::cout << "\n Size of CB2 " << BuffSize2;

            if (Buff.size() > 0)
            {
                for (int i = Buff.size() - 1; i > -1; i--)
                {
                    if (Buff.peek(i) == '[')
                    {
                        // std::cout << "\nStart marker found at " << i;
                        StartMarkerIndex = i;
                        break;           // Exit the processing block cleanly
                    }
                }
            }

            for (int i = Buff.size() - 1; i > -1; i--)
            {
                if (Buff.size() > 0) {
                    /* if (Buff.peek(i) == '[')
                         std::cout << "\n After first removal of initial data in buffer at index  " << i << " " << Buff.peek(i);*/
                }
            }

            for (int i = Buff.size() - 1; i > -1; i--)
            {
                if (Buff.size() > 0)
                {
                    if (Buff.peek(i) == '?')   //HeartBeat Monitoring
                    {
                        HeartBeat = 1;
                        //Buff.ReadByte();
                    }
                }
            }

            /*for (int i = Buff.size() - 1; i > -1; i--)
            {
                if (Buff.size() > 0)
                {
                    
                    char data = Buff.peek(i);

                    //std::cout << "\nComms radar peek buff "<< data;

                    if ((SerialOrder)data == ARD_COMMS_EST)
                    {
                        BoardReadiness = 1;
                        //std::cout << "\nComms established radar ";
                    }
                   
                }
            }*/


            std::string DataPacket;

            for (int i = Buff.size() - 1; i > -1; i--)
            {
                if (Buff.peek(i) == '[')
                {
                    if (Buff.size() > 0)
                        Buff.ReadByte();
                    break;
                }
                unsigned char data = Buff.peek(i);

                if (Buff.size() > 1)
                {
                    if (!Buff.IsEmpty())
                    {
                        Buff.ReadByte();
                        //std::cout << "\nPacking buffer, for loop number at: " << i << " Buff size " << Buff.size() << " Buff content " << Buff.peek(i) << " Buff at data " << data;
                        DataPacket.push_back(data);
                    }
                }
            }

            for (int i = Buff.size() - 1; i > -1; i--)
            {
                //if (Buff.size() > 2)
                  //std::cout << "\nAfter removal of packet, in buffer at index  " << i << " " << Buff.peek(i);
            }


            //std::cout << "\n Size of CB3 " << Buff.size() << " Data packet size " << DataPacket.size();

            std::reverse(DataPacket.begin(), DataPacket.end());
            /*
            if (DataPacket.size() > 0)
            {
                for (int i = 0; i < DataPacket.size(); i++)
                {
                    std::cout << "\nData packet " << DataPacket;
                }
            }
            */
            Packets.push_back(DataPacket);

        }
        else
        {
            run = false;

        }

        if (!Packets.empty())
        {
            for (int i = 0; i < Packets.size(); i++)
            {
                
                if (!Packets[i].empty() && std::all_of(Packets[i].begin(), Packets[i].end(),::isdigit))
                {
                    if ((SerialOrder)std::stoi(Packets[i]) == ARD_COMMS_EST)
                        BoardReadiness = 1;
                }

                //std::cout << "\nData packet in vector " << Packets[i]<<" at "<<i <<" Ard type "<< ArduinoType;

            }
        }
    }
}

void ArduinoReceiver::ParseBuffer(CircularBuffer< unsigned char >& Buff, std::vector<std::string>& Packets)
{
    bool run = true;

    while (run)
    {
        if (Buff.size() > 3)
        {

            int EndMarkerIndex = -1,
                StartMarkerIndex = -1;

            unsigned int BuffSize = Buff.size();
            //std::cout << "\n Size of CB " << BuffSize;

            /*for (int i = Buff.size() - 1; i > -1; i--)
            {
                if (Buff.size() > 0)
                  std::cout << "\n In buffer at index  " << i << " " << Buff.peek(i);
            }*/

            

            for (int i = Buff.size() - 1; i > -1; i--)
            {
                //std::cout << "\n In buffer at index  " << i << " " << Buff.peek(i);

                if (Buff.peek(i) == ']')
                {
                    //std::cout << "\nEnd at index  " << i;
                    EndMarkerIndex = i;
                    //Remove the Startmarker
                    if (Buff.size() > 0)
                     Buff.ReadByte();
                    break;
                }
                if (Buff.size() > 0)
                 Buff.ReadByte();
            }

            unsigned int BuffSize2 = Buff.size();
            //std::cout << "\n Size of CB2 " << BuffSize2;

            if (Buff.size() > 0)
            {
                for (int i = Buff.size() - 1; i > -1; i--)
                {
                    if (Buff.peek(i) == '[')
                    {
                       // std::cout << "\nStart marker found at " << i;
                        StartMarkerIndex = i;
                        break;           // Exit the processing block cleanly
                    }
                }
            }

            for (int i = Buff.size() - 1; i > -1; i--)
            {
                if (Buff.size() > 0) {
                   /* if (Buff.peek(i) == '[')
                        std::cout << "\n After first removal of initial data in buffer at index  " << i << " " << Buff.peek(i);*/
                }
            }


            std::string DataPacket;

            for (int i = BuffSize2 - 1; i > -1; i--)
            {
                if (Buff.peek(i) == '[')
                {
                    if (Buff.size() > 0)
                     Buff.ReadByte();
                    break;
                }
                unsigned char data = Buff.peek(i);

                if (Buff.size() > 2)
                {
                    Buff.ReadByte();
                    //std::cout << "\nPacking buffer, for loop number at: " << i << " Buff size " << Buff.size() << " Buff content " << Buff.peek(i) << " Buff at data " << data;
                    DataPacket.push_back(data);
                }
            }

            for (int i = Buff.size() - 1; i > -1; i--)
            {
                //if (Buff.size() > 2)
                  //std::cout << "\nAfter removal of packet, in buffer at index  " << i << " " << Buff.peek(i);
            }


            //std::cout << "\n Size of CB3 " << Buff.size() << " Data packet size " << DataPacket.size();

            std::reverse(DataPacket.begin(), DataPacket.end());

            if (DataPacket.size() > 0)
            {
                for (int i = 0; i < DataPacket.size(); i++)
                {
                    //std::cout << "\nData packet " << DataPacket[i];
                }
            }

            Packets.push_back(DataPacket);

            //for (int i = 0; i < Packets.size(); i++)
            //{
                //std::cout << "\nData packet " << Packets[i];
            //}

        }
        else
        {
            run = false;

        }
    }

    /*for (int i = 0; i < Packets.size(); i++)
    {
      std::cout << "\nData packet " << Packets[i];
    }*/

    //std::cout << "\n Number of data packets " << Packets.size();
}

void ArduinoReceiver::RefineDataPackets(std::vector<std::string>& Packets)
{
    /*
     * The incoming data must be off the format:
     * Serial order : float
     * The data packet will be rejected if the first member is not a vaild serial order,
     * the succeeding member is not ':', there is more than one '.', there is a'[' or ']'
     *
     */

     std::vector<std::string> RefinedPackets;

    if (Packets.size() > 1)
    {
        for (int i = 0; i < Packets.size(); i++)
        {
            bool ValidOrder = 0,
                SecondMemberValid = 0,
                ValidNumberOfCommaColons = 0,
                NoSquareBrackets = 1,
                DataPacketValid = 0;

            int DecimalColonCounter = 0;

            
            std::string DataPacket = Packets[i];
            char OrderRecievedChar = 0;
            int OrderReceived = 0;
            SerialOrder Order = HELLO;

            if (DataPacket.size() > 1)
            {
                OrderRecievedChar = (char)DataPacket[0];
                OrderReceived = OrderRecievedChar - '0';
                Order = (SerialOrder)OrderReceived;
                
                ValidOrder = ValidSerialOrder(Order);
                SecondMemberValid = (DataPacket[1] == ':');

                for (int j = 0; j < DataPacket.size(); j++)
                {
                    if (DataPacket[j] == '.' || DataPacket[j] == ':')
                    {
                        DecimalColonCounter++;
                    }
                }

                if (DecimalColonCounter == 2)
                    ValidNumberOfCommaColons = 1;

                for (int j = 0; j < DataPacket.size(); j++)
                {
                    if (DataPacket[j] == '[' || DataPacket[j] == ']')
                    {
                        NoSquareBrackets = 0;
                    }
                }

                if (ValidOrder && SecondMemberValid && ValidNumberOfCommaColons && NoSquareBrackets)
                    DataPacketValid = 1;
            }
            //std::cout << "\n data packet " << DataPacket << " at index " << i << " Data packet valid " << DataPacketValid;
            //std::cout << "\n data packet " << DataPacket << " at index " << i << " valid order " << ValidOrder<<" Second member "<< SecondMemberValid<<" no. comma "<< ValidNumberOfCommaColons << " Sqr brk " << NoSquareBrackets;
            //std::cout << "\n data packet " << DataPacket << " at index " << i << " order as int " << OrderReceived<<" Order as char "<< OrderRecievedChar<<" Order as SerialOrder "<< Order;
            //std::cout << "\n Data packet " << DataPacket << " Is valid " << DataPacketValid;

            if (DataPacketValid)
                RefinedPackets.push_back(DataPacket);

        }
        
        Packets = RefinedPackets;

        
        for (int i = 0; i < Packets.size(); i++)
        {
           // std::cout << "\n Filtered data packet "<< Packets[i] ;
        }


    }

}

void ArduinoReceiver::ProcessDataPackets(std::vector<std::string>& Packets)
{
    if (Packets.size() > 0) //Minimum allowable size of a packet 
    {
        for (int i = 0; i < Packets.size(); i++)
        {
            std::string DataPacket = Packets[i];
            SortAndUpdateFromPacket(DataPacket);

            //std::cout << "\n Packet contents process function " << DataPacket;
        }

    }
    else
    {
        //std::cout << "\n Packets are small " << Packets.size();
    }
}

void ArduinoReceiver::ProcessDataPackets(std::vector<std::string>& Packets, BoardSelection Board)
{
    if (Packets.size() > 0) //Minimum allowable size of a packet 
    {
        for (int i = 0; i < Packets.size(); i++)
        {
            std::string DataPacket = Packets[i];
            SortAndUpdateFromPacket(DataPacket, Board);

            //std::cout << "\n Packet contents process function " << DataPacket;
        }

    }
    else
    {
        //std::cout << "\n Packets are small " << Packets.size();
    }
}

bool ArduinoReceiver::isValidFloat(const std::string& str)
{
    if (str.empty()) return false;

    size_t i = 0;
    // Skip leading whitespace
    while (i < str.size() && std::isspace(str[i])) i++;

    // Check optional sign
    if (i < str.size() && (str[i] == '+' || str[i] == '-')) i++;

    bool hasDigits = false;
    bool hasDecimal = false;

    for (; i < str.size(); ++i) {
        if (std::isdigit(str[i])) {
            hasDigits = true;
        }
        else if (str[i] == '.' && !hasDecimal) {
            hasDecimal = true; // Allow only one decimal point
        }
        else {
            // Check if remaining characters are just trailing spaces
            while (i < str.size() && std::isspace(str[i])) i++;
            return (i == str.size()) && hasDigits;
        }
    }
    return hasDigits;
}

void ArduinoReceiver::SortAndUpdateFromPacket(std::string& DataPacket)
{
    if (!DataPacket.empty()) 
    {
        char OrderRecievedChar = 0;
        int OrderReceived = 0;
        SerialOrder Order = HELLO;

        OrderRecievedChar = (char)DataPacket[0];
        OrderReceived = OrderRecievedChar - '0';
        Order = (SerialOrder)OrderReceived;

        switch (Order)
        {

        case MEASURED_ROLL:
        {
            std::reverse(DataPacket.begin(), DataPacket.end());
            DataPacket.pop_back();
            std::reverse(DataPacket.begin(), DataPacket.end());

            if (DataPacket[0] != ':')
            {
                //std::cout << "\n Second member no good " << DataPacket; //If its made it to this point, the preceeding memeber was a serial order,
                                                                          //and can be removed. Now check that the ':' is the following member 
                break;
            }

            std::reverse(DataPacket.begin(), DataPacket.end());
            DataPacket.pop_back();
            std::reverse(DataPacket.begin(), DataPacket.end());

            float measureval = 0;

            if (isValidFloat(DataPacket))
                measureval = std::stof(DataPacket);  //Might have an error here

            ConvertedRoll = measureval;
            //std::cout << "\n Roll " << measureval << " Order " << Order;
            break;
        }
        case MEASURED_PITCH:
        {

            std::reverse(DataPacket.begin(), DataPacket.end());
            DataPacket.pop_back();
            std::reverse(DataPacket.begin(), DataPacket.end());

            if (DataPacket[0] != ':')
            {
                //std::cout << "\n Second member no good " << DataPacket;
                break;
            }
            
            std::reverse(DataPacket.begin(), DataPacket.end());
            DataPacket.pop_back();
            std::reverse(DataPacket.begin(), DataPacket.end());

            
            float measureval = 0;

            if (isValidFloat(DataPacket))
                measureval = std::stof(DataPacket);  //Might have an error here, likely bad format

            ConvertedPitch = measureval;
            //std::cout << "\n Pitch " << measureval << " Order " << Order;
            break;
        }
        case MEASURED_YAW:
        {
            std::reverse(DataPacket.begin(), DataPacket.end());
            DataPacket.pop_back();
            std::reverse(DataPacket.begin(), DataPacket.end());

            if (DataPacket[0] != ':')
            {
                //std::cout << "\n Second member no good " << DataPacket;
                break;
            }

            std::reverse(DataPacket.begin(), DataPacket.end());
            DataPacket.pop_back();
            std::reverse(DataPacket.begin(), DataPacket.end());


            float measureval = 0;

            if (isValidFloat(DataPacket))
                measureval = std::stof(DataPacket);

            //std::cout << "\n Packet contents as float " << measureval << " Order " << Order;
            break;
        }
        case MEASURED_ACCEL_X:
        {
            std::reverse(DataPacket.begin(), DataPacket.end());
            DataPacket.pop_back();
            std::reverse(DataPacket.begin(), DataPacket.end());

            if (DataPacket[0] != ':')
            {
                //std::cout << "\n Second member no good " << DataPacket;
                break;
            }

            std::reverse(DataPacket.begin(), DataPacket.end());
            DataPacket.pop_back();
            std::reverse(DataPacket.begin(), DataPacket.end());

            float measureval = std::stof(DataPacket);

            //std::cout << "\n Packet contents as float " << measureval << " Order " << Order;
            break;
        }
        case MEASURED_ACCEL_Y:
        {
            std::reverse(DataPacket.begin(), DataPacket.end());
            DataPacket.pop_back();
            std::reverse(DataPacket.begin(), DataPacket.end());

            if (DataPacket[0] != ':')
            {
                //std::cout << "\n Second member no good " << DataPacket;
                break;
            }

            std::reverse(DataPacket.begin(), DataPacket.end());
            DataPacket.pop_back();
            std::reverse(DataPacket.begin(), DataPacket.end());

            float measureval = std::stof(DataPacket);

            //std::cout << "\n Packet contents as float " << measureval << " Order " << Order;
            break;
        }
        case MEASURED_ACCEL_Z:
        {
            std::reverse(DataPacket.begin(), DataPacket.end());
            DataPacket.pop_back();
            std::reverse(DataPacket.begin(), DataPacket.end());

            if (DataPacket[0] != ':')
            {
                //std::cout << "\n Second member no good " << DataPacket;
                break;
            }

            std::reverse(DataPacket.begin(), DataPacket.end());
            DataPacket.pop_back();
            std::reverse(DataPacket.begin(), DataPacket.end());

            float measureval = std::stof(DataPacket);

            //std::cout << "\n Packet contents as float " << measureval << " Order " << Order;
            break;
        }
        default:
        {
            //std::cout << "\n No valid order has been found ";
            break; //No valid order has been found.
        }

        }


    }

}

void ArduinoReceiver::SortAndUpdateFromPacket(std::string& DataPacket, BoardSelection Board)
{
    if (!DataPacket.empty())
    {
        switch (Board)
        {
        
        case RADAR_BOARD:
        {
            UpdateRadarPacket(DataPacket);
            break;
        }

        case GYROSCOPE_BOARD:
        {
            UpdateGyroPacket(DataPacket);
            break;
        }

        case MOTOR_STEER_BOARD:
        {
            UpdateMotorSteerPacket(DataPacket);
            break;
        }


        default:
        {
            //std::cout << "\n Invalid board ";
            break; //No valid order has been found.
        }

        }
    }
}

void ArduinoReceiver::UpdateGyroPacket(std::string& DataPacket) 
{
    if (!DataPacket.empty())
    {
        char OrderRecievedChar = 0;
        int OrderReceived = 0;
        SerialOrder Order = HELLO;

        OrderRecievedChar = (char)DataPacket[0];
        OrderReceived = OrderRecievedChar - '0';
        Order = (SerialOrder)OrderReceived;

        switch (Order)
        {
        case MEASURED_ROLL:
        {
            ConvertedRoll = ValueFromPacket(DataPacket);
            break;
        }
        case MEASURED_PITCH:
        {
            ConvertedPitch = ValueFromPacket(DataPacket);
            break;
        }
        case MEASURED_YAW:
        {
            ConvertedYaw = ValueFromPacket(DataPacket);
            break;
        }
        case MEASURED_ACCEL_X:
        {
            ConvertedXAccel = ValueFromPacket(DataPacket);
            break;
        }
        case MEASURED_ACCEL_Y:
        {
            ConvertedYAccel = ValueFromPacket(DataPacket);
            break;
        }
        case MEASURED_ACCEL_Z:
        {
            ConvertedZAccel = ValueFromPacket(DataPacket);
            break;
        }
        default:
        {
            //std::cout << "\n Invalid Order ";
            break; //No valid order has been found.
        }

        }

    }
}

void ArduinoReceiver::UpdateRadarPacket(std::string& DataPacket)
{
    if (!DataPacket.empty())
    {
        char OrderRecievedChar = 0;
        int OrderReceived = 0;
        SerialOrder Order = HELLO;

        OrderRecievedChar = (char)DataPacket[0];
        OrderReceived = OrderRecievedChar - '0';
        Order = (SerialOrder)OrderReceived;

        switch (Order)
        {
        case RADAR_DISTANCE:
        {
            RadarValue = (int)ValueFromPacket(DataPacket);
            break;
        }
        case RADAR_POSITION:
        {
            RadarPosition = (int)ValueFromPacket(DataPacket);
            break;
        }

        default:
        {
            //std::cout << "\n Invalid Order ";
            break; //No valid order has been found.
        }

        }

    }
}

void ArduinoReceiver::UpdateMotorSteerPacket(std::string& DataPacket) 
{

}

float ArduinoReceiver::ValueFromPacket(std::string& DataPacket)
{
    float value;

    std::reverse(DataPacket.begin(), DataPacket.end());
    DataPacket.pop_back();
    std::reverse(DataPacket.begin(), DataPacket.end());

    if (DataPacket[0] == ':')
    {
        //std::cout << "\n Second member no good " << DataPacket;
        //break;


        std::reverse(DataPacket.begin(), DataPacket.end());
        DataPacket.pop_back();
        std::reverse(DataPacket.begin(), DataPacket.end());

        value = std::stof(DataPacket);
    }
    else
        value = 0;

    return value;
}

bool ArduinoReceiver::ValidSerialOrder(SerialOrder order)
{
    //std::cout << "\n Serial order fed "<< order;
    
    switch (order)
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
    case RADAR_DISTANCE:
    {
        return true;
    }
    case RADAR_POSITION:
    {
        return true;
    }
    default:
    {
        return false; //No valid order has been found.
    }
    }
}

void ArduinoReceiver::SendCommand2I8(SerialOrder CommandType, int8_t Command) {


    Write2CommandsI8(Ard, Command, (int8_t)CommandType); 

}

void ArduinoReceiver::SendCommand4I8(int8_t* Values) {

    Write4CommandsI8(Ard, Values);

}

void ArduinoReceiver::SteeringI8Command(SerialOrder CommandType, int8_t Command) {
   

   SendCommandI8(CommandType, Command);

}


void ArduinoReceiver::KeepSerialActive() {
    KeepSerialOpen();
}

float ArduinoReceiver::GetPitch() {
    return ConvertedPitch;
    //std::cout << "\n PITCH " << ConvertedPitch;
}

float ArduinoReceiver::GetRoll() {
    return ConvertedRoll;
    //std::cout << "\n ROLL " << ConvertedRoll;
}

float ArduinoReceiver::GetYaw() {

    return ConvertedYaw;
}

float ArduinoReceiver::GetAccelX() {
    return ConvertedXAccel;
}

float ArduinoReceiver::GetAccelY() {
    return ConvertedYAccel;
}

float ArduinoReceiver::GetAccelZ() {
    return ConvertedZAccel;
}

void ArduinoReceiver::CloaseSerial() {
    Ard.SerialClose();
}

int16_t ArduinoReceiver::GetRadarVal() {

    return RadarValue;
}

int16_t ArduinoReceiver::GetRadarPos() {

    return RadarPosition;
}

int8_t ArduinoReceiver::GetSteeringSent() {
    return SteeringAngleSent;
}

unsigned long ArduinoReceiver::GetTime() {
    return ElapsedTime;
}

ArduinoReceiver::~ArduinoReceiver() {

    //Ard.SerialClose();
}

char ArduinoReceiver::GetBoardSpecialChar()
{
    char board;

    switch (ArduinoType)
    {
    case RADAR_BOARD:
    {

        board = 'R';

        return  board;
        break;
    }
    case GYROSCOPE_BOARD:
    {

        board = 'G';

        return  board;
        break;
    }
    case MOTOR_STEER_BOARD:
    {

        board = 'M';

        return  board;
        break;
    }

    }
}

void ArduinoReceiver::InitializedBoard()
{
    char InitValue = GetBoardSpecialChar();

    DWORD StartTimeSend = GetTickCount();

    while (GetTickCount() - StartTimeSend < 20)
    {
        SendMessageToArd(InitValue);
    }

}
bool ArduinoReceiver::FindArduinoBoardPort(const std::string& expectedName, int& outPortNumber, int buad, int PrevPort, int PrevPort2)
{
    HANDLE hSerial;
    std::string BoardName = expectedName;

    char Specialchar = GetBoardSpecialChar();

    std::cout << "\nSpecial char " << Specialchar;

    for (int port = 1; port <= 32; port++)
    {
        if (port != PrevPort || port != PrevPort2) //Skip ports that have already been marked
        {
            std::cout << "\nTrying COM " << port << "...";

            if (Ard.IndetifySerialPort(port, expectedName, hSerial, buad, Specialchar))
            {
                outPortNumber = port;
                std::cout << "Found " << BoardName << " on COM" << port << "\n";
                return true;
            }
        }
    }

    return false;
}

bool ArduinoReceiver::FindArduinoBoardPort(const std::string& expectedName, int& outPortNumber, int buad, int PrevPort)
{
    HANDLE hSerial;
    std::string BoardName = expectedName;

    char Specialchar = GetBoardSpecialChar();

    std::cout << "\nSpecial char " << Specialchar;

    for (int port = 1; port <= 32; port++)
    {
        if (port != PrevPort) //Skip ports that have already been marked
        {
            //std::cout << "\nTrying COM " << port << "...";

            if (Ard.IndetifySerialPort(port, expectedName, hSerial, buad, Specialchar))
            {
                outPortNumber = port;
                std::cout << "Found " << BoardName << " on COM" << port << "\n";
                return true;
            }
        }
    }

    return false;
}

bool ArduinoReceiver::FindArduinoBoardPort(const std::string& expectedName, int& outPortNumber, int buad)
{
    HANDLE hSerial;
    std::string BoardName = expectedName;

    char Specialchar = GetBoardSpecialChar();

    std::cout << "\nSpecial char " << Specialchar;

    for (int port = 1; port <= 32; port++)
    {
        //std::cout << "\nTrying COM " << port << "...";

        if (Ard.IndetifySerialPort(port, expectedName, hSerial, buad, Specialchar))
        {
            outPortNumber = port;
            std::cout << "Found " << BoardName << " on COM" << port << "\n";
            return true;
        }

    }

    return false;
}

bool ArduinoReceiver::FindArduinoBoardPortHserial(const std::string& expectedName, int& outPortNumber, int buad, int PrevPort)
{
    HANDLE hSerial;
    std::string BoardName = expectedName;

    char Specialchar = GetBoardSpecialChar();

    std::cout << "\nSpecial char " << Specialchar;

    for (int port = 1; port <= 32; port++)
    {
        if (port != PrevPort) //Skip ports that have already been marked
        {
            //std::cout << "\nTrying COM " << port << "...";

            if (Ard.IndetifySerialPortHserial(port, expectedName, hSerial, buad, Specialchar))
            {
                outPortNumber = port;
                std::cout << "Found " << BoardName << " on COM" << port << "\n";
                return true;
            }
        }
    }

    Ard.SetHandle(hSerial);

    return false;
}