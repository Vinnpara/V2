#include <iostream>
#include <windows.h>

#include <fstream>
#include<serial/SerialPort.h>
#include<serial/SerialOrder.h>
#include "SerialComms.h"

#include<ArduinoReceiver.h>

char* ARDPort = "\\\\.\\COM3";
SerialPort Serial(ARDPort);
static bool ValidCommandRoll, ValidCommandPitch, ValidCommandYaw, ValidRadarVal, ValidRadarPos;

ArduinoReceiver::ArduinoReceiver() {
	

	Ard = Serial;

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

void ArduinoReceiver::ReadBuffer(SerialPort& Serial, SerialOrder Command, static bool& ExpectedCommand){

    int32_t Max = 8000000, Min = -8000000;

    SerialOrder ReceivedType;

    if (Serial.isConnected()) {

        ReceivedType = read_order(Serial);
        //cout << "\nENUM RECEVIED " << ReceivedType << endl;
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

        if ((Command - 10) == ReceivedType) {
            ExpectedCommand = true;

        }
        else
            ExpectedCommand = false;
        //cout << "\nCommand not received   " << Command << endl;
        std::cout << "\nFrom Class ORDER " << ReceivedType << std::endl;
    }
    else
        std::cout << "\nARDUINO DISCONNECTED " << std::endl;
}

void ArduinoReceiver::ReadArduino3Attitudes() {

    

    ///RequestReadDataFirstRequest(arduino, REQUEST_PITCH, FirstPass);
    RequestReadData(Serial, REQUEST_PITCH, ValidCommandRoll);
    ReadBuffer(Serial, REQUEST_PITCH, ValidCommandRoll);

    if (ValidCommandRoll)
        ValidRoll = true;

    if (ValidRoll) {
        //cout << "VALID ROLL " << ValidRoll;
        RequestReadData(Serial, REQUEST_ROLL, ValidCommandPitch);
        ReadBuffer(Serial, REQUEST_ROLL, ValidCommandPitch);
    }

    if (ValidCommandPitch);
    ValidPitch = true;

    if (ValidPitch) {

        RequestReadData(Serial, REQUEST_YAW, ValidCommandYaw);
        ReadBuffer(Serial, REQUEST_YAW, ValidCommandYaw);
    }



}

void ArduinoReceiver::ReadRadar() {

    if (ValidCommandYaw) {
        RequestReadData(Serial, REQUEST_RADAR);
        ReadBuffer(Serial, REQUEST_RADAR, ValidRadarVal);
    }
    if (ValidRadarVal) {
        RequestReadData(Serial, REQUEST_RADAR_POS);
        ReadBuffer(Serial, REQUEST_RADAR_POS, ValidRadarPos);
    }

}

void ArduinoReceiver::ReadRadar2() {
    
    RequestReadData(Serial, REQUEST_RADAR);
    ReadBuffer(Serial, REQUEST_RADAR, ValidRadarVal);


   if (ValidRadarVal) {

      ValidRadarValue = true;

     //RequestReadData(Serial, REQUEST_RADAR_POS);
     //ReadBuffer(Serial, REQUEST_RADAR_POS, ValidRadarPos);
     //std::cout << "\n----------------------------------------------------VALID_RADAR_VALUE ------------------------------------------------------" << std::endl;
   }

   if (ValidRadarValue) {
       RequestReadData(Serial, REQUEST_RADAR_POS);
       ReadBuffer(Serial, REQUEST_RADAR_POS, ValidRadarPos);
   }
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
    Serial.SerialClose();
}

int16_t ArduinoReceiver::GetRadarVal() {

    return RadarValue;
}

int16_t ArduinoReceiver::GetRadarPos() {

    return RadarPosition;
}

ArduinoReceiver::~ArduinoReceiver() {

    //Ard.SerialClose();
}