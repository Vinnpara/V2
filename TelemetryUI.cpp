
#include <iostream>

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<GL/GL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <TextRender.h>
#include <ResourceHandle.h>
#include <ShaderVision.h>

#include<TelemetryUI.h>
#include <VehicleModel.h>

#include<serial/SerialPort.h>
#include<serial/SerialOrder.h>
#include <ArduinoReceiver.h>

TextRender* T1;
VehicleModel* VM1;

//char output[MAX_DATA_LENGTH];
//char incomingData[MAX_DATA_LENGTH];
char* Port = "\\\\.\\COM3";
//char input[MAX_DATA_LENGTH];

ArduinoReceiver Ard1;

TelemetryUI::TelemetryUI() {



}

void TelemetryUI::InitializeTelemetry() {
    Ard1.ArdInitialize();

	T1 = new TextRender();
    VM1 = new VehicleModel();

	T1->PrepareTextVS();

}

void TelemetryUI::ReTryRequest(SerialPort& Serial, SerialOrder Command) {

    bool TransferFail;

    char buff[1] = { Command };
    TransferFail = Serial.writeSerialPort(buff, 1);
    //cout << "\n RETRYING COMMAND " << TransferFail << endl;


}

int16_t  TelemetryUI::LimitValueInt32(int32_t MaxValue, int32_t MinValue, int32_t& ReadValue) {

    if (ReadValue <= MinValue)
        ReadValue = MinValue;

    else if (ReadValue >= MaxValue)
        ReadValue = MaxValue;

    return ReadValue;

}

float TelemetryUI::maxval(float max, float min, float& val) {
    float value;

    if (val <= min)
        value = min;
    else if (val >= max)
        value = max;
    else
        value = val;
    return value;
}

void TelemetryUI::FilterVal(float max, float min, float& val){

    if (val <= min)
        val = min;
    else if (val >= max)
        val = max;


}

void TelemetryUI::UpdateValues6Axis(float y, float p, float r, float Ax, float Ay, float Az) {

    this->Yaw = y;
    this->Roll = r;
    this->Pitch = p;

    this->AccelX = Ax;
    this->AccelY = Ay;
    this->AccelZ = Az;

    sprintf_s(RollRead, "%f", Roll);
    sprintf_s(PtchRead, "%f", Pitch);
    sprintf_s(YawRead, "%f", Yaw);

    sprintf_s(AccelXRead, "%f", AccelX);
    sprintf_s(AccelYRead, "%f", AccelY);
    sprintf_s(AccelZRead, "%f", AccelZ);


}

void TelemetryUI::UpdateValues3Attitude(float y, float p, float r) {

    
    Ard1.ReadArduino3Attitudes();
    
    //FilterVal(180.0f, -180.0f, y);
    //FilterVal(180.0f, -180.0f, p);
    //FilterVal(180.0f, -180.0f, r);

    this->Yaw = Ard1.GetYaw();
    this->Roll = Ard1.GetRoll();
    this->Pitch = Ard1.GetPitch();

    FilterVal(180.0f, -180.0f, Yaw);
    FilterVal(180.0f, -180.0f, Pitch);
    FilterVal(180.0f, -180.0f, Roll);

    sprintf_s(RollRead, "%f", Roll);
    sprintf_s(PtchRead, "%f", Pitch);
    sprintf_s(YawRead, "%f", Yaw);

    VM1->UpdateAttitudeValues(Yaw,Pitch,Roll);

}

void TelemetryUI::UpdateValues3Accel(float Ax, float Ay, float Az) {

    this->AccelX = Ax;
    this->AccelY = Ay;
    this->AccelZ = Az;

    sprintf_s(AccelXRead, "%f", AccelX);
    sprintf_s(AccelYRead, "%f", AccelY);
    sprintf_s(AccelZRead, "%f", AccelZ);

}

void TelemetryUI::UpdateValuesRadar(int16_t RadarValue, int16_t RadarPosition) {

    this->RadarVal = RadarValue;
    this->RadarPos = RadarPosition;

    Radval = std::to_string(RadarVal);
    Radpos = std::to_string(RadarPos);

}

int16_t  TelemetryUI::BufferFilterInt16(int16_t MaxValue, int16_t MinValue, int16_t& ReadValue) {

    if (ReadValue <= MinValue)
        ReadValue = MinValue;

    else if (ReadValue >= MaxValue)
        ReadValue = MaxValue;

    return ReadValue;

}

void TelemetryUI::ReadBuffer(SerialPort& Serial, SerialOrder Command, static bool& ExpectedCommand) {
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
        case RADAR_DISTANCE:
        {   //This sent as an int16_t
            int16_t MeasuredRadarDistance = read_i16(Serial);
            RadarVal = MeasuredRadarDistance;
            BufferFilterInt16(201, 0, MeasuredRadarDistance);
            //cout << "\nRADAR_DISTANCE " << MeasuredRadarDistance << endl;
            break;
        }
        case RADAR_POSITION:
        {   //This sent as an int16_t
            int16_t MeasuredRadarPosition = read_i16(Serial);
            RadarPos = MeasuredRadarPosition;
            //cout << "\nRADAR_POSITION " << MeasuredRadarPosition << endl;
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
            std::cout << "\MEASURED_ROLL " << Roll_Int32_lim << " " << ConvertedRoll << std::endl;
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
            std::cout << "\MEASURED_PITCH " << Pitch_Int32_lim << " " << ConvertedPitch << std::endl;
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
            std::cout << "\MEASURED_YAW " << Yaw_Int32_lim << " " << ConvertedYaw << std::endl;
            break;
        }
        case MEASURED_ACCEL_X:
        {   //This sent as an int16_t
            int16_t X_Accel_Int16 = read_i16(Serial);
            //RadarValue = MeasuredRadarDistance;
            //BufferFilterInt16(201, 0, MeasuredRadarDistance);
            float ConvertedXAccel = float(X_Accel_Int16) / 10000;
            std::cout << "\MEASURED_X_ACCEL " << X_Accel_Int16 << " " << ConvertedXAccel << std::endl;
            //cout << "\nRADAR_DISTANCE " << MeasuredRadarDistance << endl;
            break;
        }
        case MEASURED_ACCEL_Y:
        {   //This sent as an int16_t
            int16_t Y_Pitch_Int16 = read_i16(Serial);
            //RadarValue = MeasuredRadarDistance;
            //BufferFilterInt16(201, 0, MeasuredRadarDistance);
            float ConvertedYAccel = float(Y_Pitch_Int16) / 10000;
            std::cout << "\MEASURED_Y_ACCEL " << Y_Pitch_Int16 << " " << ConvertedYAccel << std::endl;
            //cout << "\nRADAR_DISTANCE " << MeasuredRadarDistance << endl;
            break;
        }
        case MEASURED_ACCEL_Z:
        {   //This sent as an int16_t
            int16_t Z_Pitch_int16 = read_i16(Serial);
            //RadarValue = MeasuredRadarDistance;
            //BufferFilterInt16(201, 0, MeasuredRadarDistance);
            float ConvertedZAccel = float(Z_Pitch_int16) / 10000;
            std::cout << "\MEASURED_Y_ACCEL " << Z_Pitch_int16 << " " << ConvertedZAccel << std::endl;
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
        std::cout << "\nORDER " << ReceivedType << std::endl;
    }
    else
    std::cout << "\nARDUINO DISCONNECTED " << std::endl;

}

void TelemetryUI::RequestReadData(SerialPort& Serial, SerialOrder Command, static bool &CommandNotReady) {

    if (CommandNotReady) {

        bool TransferFail;
        char buff[1] = { Command };
        TransferFail = Serial.writeSerialPort(buff, 1);

        //CommandNotReady = false;
    }
}

void TelemetryUI::RequestReadData(SerialPort& Serial, SerialOrder Command) {
    bool TransferFail;

    char buff[1] = { Command };

    TransferFail = Serial.writeSerialPort(buff, 1);
}

void TelemetryUI::UpdateValuues(SerialPort& Serial) {


    static bool ValidCommandRoll, 
                ValidCommandPitch, 
                ValidCommandYaw, 
                ValidRadarVal, 
                ValidRadarPos,
                ValidRoll,
                ValidPitch;

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

    if (ValidCommandYaw) {
        RequestReadData(Serial, REQUEST_RADAR);
        ReadBuffer(Serial, REQUEST_RADAR, ValidRadarVal);
    }
    if (ValidRadarVal) {
        RequestReadData(Serial, REQUEST_RADAR_POS);
        ReadBuffer(Serial, REQUEST_RADAR_POS, ValidRadarPos);
    }

    UpdateValues3Attitude(ConvertedYaw, ConvertedPitch, ConvertedRoll);
    //UpdateValuesRadar(RadarValue, RadarPosition);

}



void TelemetryUI::RenderYaw() {

	T1->RenderTextVS(YawRead, 155.0f, 125.0f, 1.0f, Color);

}

void TelemetryUI::RenderPitch() {

	T1->RenderTextVS(PtchRead, 155.0f, 75.0f, 1.0f, Color);

}

void TelemetryUI::RenderRoll() {

	T1->RenderTextVS(RollRead, 145.0f, 25.0f, 1.0f, Color);

}

void TelemetryUI::RenderAccelX() {

	T1->RenderTextVS(AccelXRead, 195.0f, 25.0f, 1.0f, Color);

}

void TelemetryUI::RenderAccelY() {

	T1->RenderTextVS(AccelYRead, 205.0f, 125.0f, 1.0f, Color);

}

void TelemetryUI::RenderAccelZ() {

	T1->RenderTextVS(AccelZRead, 205.0f, 75.0f, 1.0f, Color);

}

void TelemetryUI::RenderRadar() {

	T1->RenderTextVS(Radval, 605.0f, 125.0f, 1.0f, RadColor);
	T1->RenderTextVS(Radpos, 605.0f, 75.0f, 1.0f, RadColor);
    T1->RenderTextVS(OrderFromArd, 605.0f, 25.0f, 1.0f, RadColor);
}

void TelemetryUI::RenderModel() {

    

    VM1->Draw();

}
void TelemetryUI::CloseSerial() {

    Ard1.CloaseSerial();

}

TelemetryUI::~TelemetryUI() {
	
	delete T1;
    delete VM1;
    //arduino.SerialClose();
}