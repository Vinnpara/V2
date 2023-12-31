#include <iostream>
#include <windows.h>

#include <fstream>
#include<serial/SerialPort.h>
#include<serial/SerialOrder.h>
#include "SerialComms.h"

#include<ArduinoReceiver.h>
#include<SerialPortSelection.h>

char* ARDPort = "\\\\.\\COM3";
char* ARDPort2 = "\\\\.\\COM1";
char* ARDPort3 = "\\\\.\\COM5";
char* ARDPort4 = "\\\\.\\COM4";
char* ARDPort5 = "\\\\.\\COM7";
char* ARDPort6 = "\\\\.\\COM8";

static bool ValidCommandRoll, ValidCommandPitch, ValidCommandYaw, ValidRadarVal, ValidRadarPos, ValidYaw;

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

    }

}

void ArduinoReceiver::ArdInitialize() {

    FirstPass = true;
    ValidRoll = false;
    ValidPitch = false;
}

void ArduinoReceiver::ArduinoFirstPass(){
}


void ArduinoReceiver::RequestReadData(SerialPort& Serial, SerialOrder Command, static bool PCReady){
    bool TransferFail;
    
    if (PCReady) {
        char buff[1] = { Command };

        TransferFail = Serial.writeSerialPort(buff, 1);

        std::cout << "\n " << TransferFail << std::endl;

    }

}

void ArduinoReceiver::RequestReadData(SerialPort& Serial, SerialOrder Command) {
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

        //std::cout << "\n RequestedCommandReceived ARD RETRYING COMMAND " << ReceivedType << std::endl;
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
        std::cout << "\n RequestedCommandReceived ARD RETRYING COMMAND " << ReceivedType - 10 <<" "<< CommandExpected << std::endl;
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

void ArduinoReceiver::ReadBuffer(SerialPort& Serial, SerialOrder Command, static bool& ExpectedCommand){

    int32_t Max = 8000000, Min = -8000000;

    SerialOrder ReceivedType;

    if (Serial.isConnected()) {

        ReceivedType = read_order(Serial);
        //cout << "\nENUM RECEVIED " << ReceivedType << endl;

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
            int16_t X_Accel_Int16 = read_i16(Serial);
            //RadarValue = MeasuredRadarDistance;
            //BufferFilterInt16(201, 0, MeasuredRadarDistance);
            float ConvertedXAccel = float(X_Accel_Int16) / 10000;
            //std::cout << "\MEASURED_X_ACCEL " << X_Accel_Int16 << " " << ConvertedXAccel << std::endl;
            //cout << "\nRADAR_DISTANCE " << MeasuredRadarDistance << endl;
            break;
        }
        case MEASURED_ACCEL_Y:
        {   //This sent as an int16_t
            int16_t Y_Pitch_Int16 = read_i16(Serial);
            //RadarValue = MeasuredRadarDistance;
            //BufferFilterInt16(201, 0, MeasuredRadarDistance);
            float ConvertedYAccel = float(Y_Pitch_Int16) / 10000;
            //std::cout << "\MEASURED_Y_ACCEL " << Y_Pitch_Int16 << " " << ConvertedYAccel << std::endl;
            //cout << "\nRADAR_DISTANCE " << MeasuredRadarDistance << endl;
            break;
        }
        case MEASURED_ACCEL_Z:
        {   //This sent as an int16_t
            int16_t Z_Pitch_int16 = read_i16(Serial);
            //RadarValue = MeasuredRadarDistance;
            //BufferFilterInt16(201, 0, MeasuredRadarDistance);
            float ConvertedZAccel = float(Z_Pitch_int16) / 10000;
            //std::cout << "\MEASURED_Y_ACCEL " << Z_Pitch_int16 << " " << ConvertedZAccel << std::endl;
            break;
        }
        case RADAR_DISTANCE:
        {   //This sent as an int16_t
            int16_t MeasuredRadarDistance = read_i16(Serial);
            RadarValue = MeasuredRadarDistance;
            BufferFilterInt16(201, 0, MeasuredRadarDistance);
            std::cout << "\nRADAR_DISTANCE " << MeasuredRadarDistance << std::endl;
            break;
        }
        case RADAR_POSITION:
        {   //This sent as an int16_t
            int16_t MeasuredRadarPosition = read_i16(Serial);
            RadarPosition = MeasuredRadarPosition;
            std::cout << "\nRADAR_POSITION " << MeasuredRadarPosition << std::endl;
            break;
        }
        
        default:
        {
            //The PC is not getting any valid values so do not write into serial on ard.

            SerialOrder OrderWait = PC_NOT_READY;

            char buff[1] = { OrderWait };
            //bool TransferReceived = false;
            bool TransferFail = Serial.writeSerialPort(buff, 1);
            //cout << "\nBAD ENUM RECEVIED " <<endl;
            //char InternalBuffer[MAX_DATA_LENGTH];
            //Serial.readSerialPort(InternalBuffer, MAX_DATA_LENGTH);
            //cout << "\nUnknown command buffer: "<< InternalBuffer[0] << endl;
            //cout << "\nUnknown command : " << ReceivedType << endl;

            ReTryRequest(Serial, Command);
        }

        }


        //cout << "\nCommand not received   " << Command << endl;
        std::cout << "\nFrom Class ORDER " << ReceivedType << std::endl;
    }
    else
        std::cout << "\nARDUINO DISCONNECTED " << std::endl;
}

void ArduinoReceiver::ReadArduino3Attitudes() {

    

    ///RequestReadDataFirstRequest(arduino, REQUEST_PITCH, FirstPass);
    RequestReadData(Ard, REQUEST_PITCH, ValidCommandRoll);
    ReadBuffer(Ard, REQUEST_PITCH, ValidCommandRoll);

    if (ValidCommandRoll)
        ValidRoll = true;

    if (ValidRoll) {
        //cout << "VALID ROLL " << ValidRoll;
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
}

float ArduinoReceiver::GetRoll() {
    return ConvertedRoll;
}

float ArduinoReceiver::GetYaw() {

    return ConvertedYaw;
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

ArduinoReceiver::~ArduinoReceiver() {

    //Ard.SerialClose();
}