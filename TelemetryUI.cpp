
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
#include<SerialPortSelection.h>

TextRender* T1;
VehicleModel* VM1;

//char output[MAX_DATA_LENGTH];
//char incomingData[MAX_DATA_LENGTH];
char* Port = "\\\\.\\COM3";
//char input[MAX_DATA_LENGTH];

//ArduinoReceiver Ard1;

ArduinoReceiver ArdRadar(COM5);
ArduinoReceiver ArdGyro(COM3);
ArduinoReceiver ArdMotorSteer(COM8);

TelemetryUI::TelemetryUI() {



}

void TelemetryUI::InitializeTelemetry() {
    //Ard1.ArdInitialize();

    ArdRadar.ArdInitialize();
    ArdGyro.ArdInitialize();
    ArdMotorSteer.ArdInitialize();

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

float TelemetryUI::ConvertValue(float RadarValX, float m, float C) {
    
    float ConvertedVal = (m * RadarValX) + C;

    return ConvertedVal;
 

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

void TelemetryUI::UpdateValuesRadar(int16_t RadarValue, int16_t RadarPosition) {
    
    ArdRadar.ReadRadar2();

    this->RadarVal = RadarValue;
    this->RadarPos = RadarPosition;

    Radval = std::to_string(ArdRadar.GetRadarVal());
    Radpos = std::to_string(ArdRadar.GetRadarPos());

}

void TelemetryUI::UpdateValuesRadar() {
    
    ArdRadar.ReadRadarDefaultPort();


    Radval = std::to_string(ArdRadar.GetRadarVal());
    Radpos = std::to_string(ArdRadar.GetRadarPos());

}


void TelemetryUI::UpdateValues3Attitude(float y, float p, float r) {

    ArdGyro.ReadArduino3Attitudes();

    //Ard1.ReadArduino3Attitudes();
    //Ard1.ReadRadar();
    //Ard1.ReadAllVals();
    //FilterVal(180.0f, -180.0f, y);
    //FilterVal(180.0f, -180.0f, p);
    //FilterVal(180.0f, -180.0f, r);

    //this->Yaw = Ard1.GetYaw();
    //this->Roll = Ard1.GetRoll();
    //this->Pitch = Ard1.GetPitch();

    float Yaw, Pitch ,Roll;

    Yaw = ArdGyro.GetYaw();
    Pitch = ArdGyro.GetPitch();
    Roll = ArdGyro.GetRoll();

    FilterVal(180.0f, -180.0f, Yaw);
    FilterVal(180.0f, -180.0f, Pitch);
    FilterVal(180.0f, -180.0f, Roll);

    sprintf_s(RollRead, "%f", Roll);
    sprintf_s(PtchRead, "%f", Pitch);
    sprintf_s(YawRead, "%f", Yaw);

    VM1->UpdateAttitudeValues(Yaw,Pitch,Roll);
    //UpdateValuesRadar(Ard1.GetRadarVal(), Ard1.GetRadarPos());

}



void TelemetryUI::UpdateValues3Accel(float Ax, float Ay, float Az) {

    this->AccelX = Ax;
    this->AccelY = Ay;
    this->AccelZ = Az;

    sprintf_s(AccelXRead, "%f", AccelX);
    sprintf_s(AccelYRead, "%f", AccelY);
    sprintf_s(AccelZRead, "%f", AccelZ);

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

void TelemetryUI::RenderControllerState(int InputDetected) {


    if (InputDetected)
        T1->RenderTextVS("CNTRL CON", 345.0f, 75.0f, 1.0f, Color);
    else 
        T1->RenderTextVS("CNTRL NOT CON", 345.0f, 75.0f, 1.0f, Color);
}

void TelemetryUI::RenderAxis(const float* AxesArr) {

   

    float Axis1 = AxesArr[0];

    //T1->RenderTextVS(std::to_string(AxesArr[0]), 14.0f, 375.0f, 1.0f, Color); //top left cntrl L-R axis
    //T1->RenderTextVS(std::to_string(AxesArr[1]), 14.0f, 325.0f, 1.0f, Color); // top left cntrl U-D axis
    //T1->RenderTextVS(std::to_string(AxesArr[2]), 14.0f, 275.0f, 1.0f, Color); // right cntrl L-R axis
    //T1->RenderTextVS(std::to_string(AxesArr[3]), 14.0f, 255.0f, 1.0f, Color); // right cntrl U-D axis
    //T1->RenderTextVS(std::to_string(AxesArr[4]), 14.0f, 225.0f, 1.0f, Color); // LT
    //T1->RenderTextVS(std::to_string(AxesArr[5]), 14.0f, 200.0f, 1.0f, Color); // LR

}

void  TelemetryUI::RenderRawSteerAngle(const float* AxesArr) {

    float JoyStickSteer = AxesArr[0];
    float ThrottleAngle = AxesArr[4];

    float RawSteerCommand = ConvertValue(JoyStickSteer, 48.0f, 58.0f);
    int8_t SteerIn = (int8_t)RawSteerCommand;

    //float RawThrottleCommand = ConvertValue(ThrottleAngle, 127.5f, 127.5f);
    float RawThrottleCommand = ConvertValue(ThrottleAngle, 50.0f, 50.0f);
    int8_t ThrottleIn = (int8_t)RawThrottleCommand;



    //ArdMotorSteer.KeepSerialActive();

    //ArdMotorSteer.SteeringI8Command(STEER_COMMAND, SteerIn);

    bool SteeringReq=0;
        
    //SteeringReq = ArdMotorSteer.ReadAndSendRequestedData(REQUEST_STEER, SteerIn);

    int8_t Vals[4];

    Vals[0] = (int8_t)STEER_COMMAND;
    Vals[1] = SteerIn;
    Vals[2] = (int8_t)MOTOR_SPEED;
    Vals[3] = ThrottleIn;

    //Vals[0] = SteerIn;
    //Vals[1] = (int8_t)STEER_COMMAND;
    //Vals[2] = ThrottleIn;
    //Vals[3] = (int8_t)MOTOR_SPEED;

    T1->RenderTextVS(std::to_string(Vals[1]), 14.0f, 375.0f, 1.0f, Color);
    T1->RenderTextVS(std::to_string(Vals[3]), 14.0f, 415.0f, 1.0f, Color);

    //ArdMotorSteer.SendCommand2I8(STEER_COMMAND, SteerIn);
    //ArdMotorSteer.SendCommand2I8(MOTOR_SPEED, ThrottleIn);

     ArdMotorSteer.SendCommand4I8(Vals);

    if (SteeringReq) {
        T1->RenderTextVS("SR", 14.0f, 330.0f, 1.0f, Color);
        //ArdMotorSteer.SteeringI8Command(STEER_COMMAND, SteerIn);
        //ArdMotorSteer.CheckSentCommand(STEER_COMMAND);

        //int8_t SteerSent = ArdMotorSteer.GetSteeringSent();

        //T1->RenderTextVS(std::to_string(SteerIn), 14.0f, 285.0f, 1.0f, Color);
    }
}

void TelemetryUI::RenderRawSteerAngleAndMotorSpeed(const float* AxesArr) {

    float JoyStickSteer = AxesArr[0];

    float RawSteerCommand = ConvertValue(JoyStickSteer, 48.0f, 58.0f);
    int8_t SteerIn = (int8_t)RawSteerCommand;

    T1->RenderTextVS(std::to_string(SteerIn), 14.0f, 375.0f, 1.0f, Color);

    //ArdMotorSteer.KeepSerialActive();

    //ArdMotorSteer.SteeringI8Command(STEER_COMMAND, SteerIn);

    bool SteeringReq = 0;

    //SteeringReq = ArdMotorSteer.ReadAndSendRequestedData(REQUEST_STEER, SteerIn);



    ArdMotorSteer.SendCommand2I8(STEER_COMMAND, SteerIn);

    if (SteeringReq) {
        T1->RenderTextVS("SR", 14.0f, 330.0f, 1.0f, Color);
        //ArdMotorSteer.SteeringI8Command(STEER_COMMAND, SteerIn);
        //ArdMotorSteer.CheckSentCommand(STEER_COMMAND);

        //int8_t SteerSent = ArdMotorSteer.GetSteeringSent();

        //T1->RenderTextVS(std::to_string(SteerIn), 14.0f, 285.0f, 1.0f, Color);
    }

}

int16_t TelemetryUI::GetRadarPos() { 
    
    return ArdRadar.GetRadarPos(); std::cout << "\nR_POS" << ArdRadar.GetRadarPos();
}

int16_t TelemetryUI::GetRadarVal() {

    return ArdRadar.GetRadarVal(); std::cout << "\nR_VAL" << ArdRadar.GetRadarVal();
}

void TelemetryUI::OpenSerial() {

    ArdMotorSteer.KeepSerialActive();
}

void TelemetryUI::RenderModel() {

    

    VM1->Draw();

}
void TelemetryUI::CloseSerial() {

    //Ard1.CloaseSerial();

}

TelemetryUI::~TelemetryUI() {
	
	delete T1;
    delete VM1;
    //arduino.SerialClose();
}