
#include <iostream>
#include <fstream>

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
#include <SerialConnectionSpeed.h>

#include <CircularBuffer.h>

#include <TestWindow.h>
#include <Windows.h>
#include <BoardSelection.h>

#include <DataConcentrator.h>

#include <GUICallBackSelection.h>

#include "ArduinoBoard.h"
#include "GyroBoard.h"
#include "RadarBoard.h"
#include "MSBoard.h"

TextRender* T1;
VehicleModel* VM1;

static int NumberOfMessagesCommPorts = 0;

static bool PrintDiagnosisMessages = 0,
                  MessageAdded = 0,
            TryConnection;

bool TelemetryUI::BoardsCommsEstablished[5];
bool TelemetryUI::BoardsCommsEstablishedTimer[5];
bool TelemetryUI::BoardReadyTimer[5];
bool TelemetryUI::HeartBeatTimer[5];
bool TelemetryUI::HeartBeatTimerElapsed;
bool TelemetryUI::BoardReadyTimerElpased;
bool TelemetryUI::CommsEstTimerElapsed;

int TelemetryUI::GYROidx = 0;
int TelemetryUI::RADARidx = 0;
int TelemetryUI::MSBidx = 0;

int TelemetryUI::BoardReadinessCounter[5];
int TelemetryUI::BoardHeartBeatCounter[5];
int TelemetryUI::BoardCommsEstCounter[5];

float TelemetryUI::ReturnTotalTime() {

    return TotalElapsedTime;

}

TelemetryUI::TelemetryUI() {


}


std::unique_ptr<ArduinoBoard> TelemetryUI::MakeBoard(BoardSelection Name)
{
    switch (Name)
    {
    case MOTOR_STEER_BOARD: return std::make_unique<MSBoard>();
    case GYROSCOPE_BOARD:   return std::make_unique<GyroBoard>();
    case RADAR_BOARD:      return std::make_unique<RadarBoard>();
    }
    return nullptr;
}


void TelemetryUI::AssignBoards() {
/*
* Messages:
* Board assignments, cyan
* code 1001
*/
    CallBackSelection Sel = ARD_SELECTION_AUTO_PORT_WINDOW;

   TestWindow* tWindow = new TestWindow(Sel);
   vector<string> Boards;

    bool run = true;

    DataConcentrator DC;

    bool MessageAdd;

    std::string MSBName,
        GYROName,
        RADARName;

    int MSBBaud = 0,
        FoundPort = 0,
        GYROBaud = 0,
        RADARBaud = 0,
        GYROFoundPort = 0,
        RADARFoundPort = 0;

    std::vector<BoardSelection> BoardsSelected;

    while (run)
    {
        tWindow->Assignments();

        tWindow->GetBoardNames();
        tWindow->SetBoardParamaters();

        if (tWindow->ArduinoConfirmCommPorts())
        {
            tWindow->UpdateCommPortsMessages(DC);

            BoardAssingmentAndBuadRate = tWindow->GetBoardsAndRates();

            for (const auto& pair : BoardAssingmentAndBuadRate)
            {
                //std::cout << "\nBoard and rate " << pair.first << ": " << pair.second << "\n";

                if (pair.first == "MSB")
                {
                    MSBName = pair.first;
                    MSBBaud = pair.second;

                    BoardsSelected.push_back(MOTOR_STEER_BOARD);
                }
                if (pair.first == "GYRO")
                {
                    GYROName = pair.first;
                    GYROBaud = pair.second;

                    BoardsSelected.push_back(GYROSCOPE_BOARD);
                }
                if (pair.first == "RADAR")
                {
                    RADARName = pair.first;
                    RADARBaud = pair.second;

                    BoardsSelected.push_back(RADAR_BOARD);
                }

            }

            for (int i = 0; i < BoardsSelected.size(); i++)
            {
                std::cout << "\n Board sels " << BoardsSelected[i];
            }

            tWindow->ResetArduinoCommPorts();
        }


        if (tWindow->ArduinoConnectionRequest())
        {
            
            TryConnection = 1;
            
            tWindow->ResetArduinoConnectionRequest();
        }

        if (TryConnection)
        {
            
            for (int i = 0; i < BoardsSelected.size(); i++)
            {
               // std::cout << "\n Board sels " << BoardsSelected[i];

                BoardsSelection.push_back(MakeBoard(BoardsSelected[i]));
            }

            //set baud rates
            for (int i = 0; i < BoardsSelection.size(); i++)
            {
                if (BoardsSelection[i]->GetName() == "MSB")
                {
                    BoardsSelection[i]->SetBaudRate(MSBBaud);
                    MSBidx = i;

                    std::pair<int, int>MSBPair;
                    MSBPair.first = MOTOR_STEER_BOARD;
                    MSBPair.second = i;
                    BoardsAndIndex.push_back(MSBPair);
                }
                if (BoardsSelection[i]->GetName() == "GYRO")
                {
                    BoardsSelection[i]->SetBaudRate(GYROBaud);
                    GYROidx = i;

                    std::pair<int, int>GYROPair;
                    GYROPair.first = GYROSCOPE_BOARD;
                    GYROPair.second = i;
                    BoardsAndIndex.push_back(GYROPair);
                }
                if (BoardsSelection[i]->GetName() == "RADAR")
                {
                    BoardsSelection[i]->SetBaudRate(RADARBaud);
                    RADARidx = i;

                    std::pair<int, int>RADARPair;
                    RADARPair.first = RADAR_BOARD;
                    RADARPair.second = i;
                    BoardsAndIndex.push_back(RADARPair);
                }

            }

            std::string Att = "13:Connecting to ";
            std::string MSB = " MSB ",
                        GYRO = " GYRO ",
                        RADAR = " RADAR ";
            std::string Conn = "13:Connected to port ";
            std::string NotCon = "11:Not Connected ";

            
            //try connection
            for (int i = 0; i < BoardsSelection.size(); i++)
            {
                
                //std::cout << "\nBoard Number " << BoardsSelection.size();
                
                ShowConnectionMessage(BoardsSelection[i]->GetBoardIdent(), DC);

                DC.CheckMapIncrement(NumberOfMessagesCommPorts);
                DC.PreparePrintMessage();
                DC.ReprintMessage(PrintDiagnosisMessages, MessageAdded);
                tWindow->GetMessageToprint(DC.GetPrintMessage());
                tWindow->ShowCommsPortData(PrintDiagnosisMessages);

                bool Connected = BoardsSelection[i]->ConnectArduino();

                ShowConnectionSuccessFul(BoardsSelection[i]->GetBoardIdent(), BoardsSelection[i]->GetPort(), Connected, DC);

                DC.CheckMapIncrement(NumberOfMessagesCommPorts);
                DC.PreparePrintMessage();
                DC.ReprintMessage(PrintDiagnosisMessages, MessageAdded);
                tWindow->GetMessageToprint(DC.GetPrintMessage());
                tWindow->ShowCommsPortData(PrintDiagnosisMessages);
            }
            

            TryConnection = 0;
        }

        

        if (!tWindow->ProcessMessages())
        {
            run = false;

        }

        DC.CheckMapIncrement(NumberOfMessagesCommPorts);
        DC.PreparePrintMessage();
        DC.ReprintMessage(PrintDiagnosisMessages, MessageAdded);

        tWindow->GetMessageToprint(DC.GetPrintMessage());
        tWindow->ShowCommsPortData(PrintDiagnosisMessages);

        //Sleep(1);

    }



    int ArdBoard1 = tWindow->board1;
    int ArdBoard2 = tWindow->board2;
    int ArdBoard3 = tWindow->board3;


    delete tWindow;
    
}


void TelemetryUI::InitializeTelemetry() {
    //Ard1.ArdInitialize();



	T1 = new TextRender();
    VM1 = new VehicleModel();

	T1->PrepareTextVS();

}


void TelemetryUI::MonitorCommsStatus(DataConcentrator& DC)
{
    std::vector<std::pair<int, bool>> BoardHBStatus;
    std::vector<std::pair<int, bool>> BoardReadinessStatus;
    std::vector<std::pair<int, bool>> BoardCommsEstStatus;

    for (int i = 0; i < BoardsSelection.size(); i++)
    {
        
        std::pair<int, bool>HbStatus;
        std::pair<int, bool>BoardReadyStatus;
        std::pair<int, bool>BoardCommsStatus;

        HbStatus.first = BoardsSelection[i]->GetBoardIdent();
        HbStatus.second = BoardsSelection[i]->GetHearBeat();

        BoardReadyStatus.first = BoardsSelection[i]->GetBoardIdent();
        BoardReadyStatus.second = BoardsSelection[i]->GetBoardReadiness();

        BoardCommsStatus.first = BoardsSelection[i]->GetBoardIdent();
        BoardCommsStatus.second = BoardsSelection[i]->GetCommsEstState();

        BoardHBStatus.push_back(HbStatus);
        BoardReadinessStatus.push_back(BoardReadyStatus);
        BoardCommsEstStatus.push_back(BoardCommsStatus);

    }

    DetectHB(2, DC, BoardHBStatus);
    DetectBoardReadiness(2, DC, BoardReadinessStatus);
    DetectCommsEst(2, DC, BoardCommsEstStatus);


    for (int i = 0; i < BoardsSelection.size(); i++)
    {
        bool tst = 0;
        BoardsSelection[i]->SetHeartBeat(tst);
        BoardsSelection[i]->SetBoardReadiness(tst);
        BoardsSelection[i]->SetCommsEstState(tst);
    }

}

void TelemetryUI::BoardsSendAndReceive()
{
    for (int i = 0; i < BoardsSelection.size(); i++)
    {
        //auto t0 = std::chrono::steady_clock::now();
        BoardsSelection[i]->SendAndReceive();
        //auto ms = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - t0).count();
        //std::cout << "\nTime elapsed " << ms << " " << BoardsSelection[i]->GetName();
    }
}

void TelemetryUI::SignalPCReadiness()
{
    for (int i = 0; i < BoardsSelection.size(); i++)
    {
        BoardsSelection[i]->SendHeartBeat();
        BoardsSelection[i]->SendPCReadiness();
        BoardsSelection[i]->SignalBoardFound();
    }
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

int16_t  TelemetryUI::BufferFilterInt16(int16_t MaxValue, int16_t MinValue, int16_t& ReadValue) {

    if (ReadValue <= MinValue)
        ReadValue = MinValue;

    else if (ReadValue >= MaxValue)
        ReadValue = MaxValue;

    return ReadValue;

}

void TelemetryUI::UpdateGyroData()
{
    for (int i = 0; i < BoardsSelection.size(); i++)
    {
        if (BoardsSelection[i]->GetName() == "GYRO")
        {
            if (GyroBoard* Gy = dynamic_cast<GyroBoard*>(BoardsSelection[i].get()))
            {
                Pitch = Gy->GetPitch();
                Roll = Gy->GetRoll();
                Yaw = Gy->GetYaw();
            }
        }
    }

    VM1->UpdateAttitudeValues(Yaw, Pitch, Roll);
    VM1->Draw();
    sprintf_s(RollRead, "%f", Roll); //Move to telem class? why is this a random function
    sprintf_s(PtchRead, "%f", Pitch);
    sprintf_s(YawRead, "%f", Yaw);
}

void TelemetryUI::UpdateRadar()
{
    for (int i = 0; i < BoardsSelection.size(); i++)
    {
        if (BoardsSelection[i]->GetName() == "RADAR")
        {
            if (RadarBoard* Rd = dynamic_cast<RadarBoard*>(BoardsSelection[i].get()))
            {
                RadarVal = Rd->GetRadarVal();
                RadarPos = Rd->GetRadarPosition();
            }
            
        }
    }
}

void TelemetryUI::UpdateMotorSteerCommand(static DWORD& lastSendTime)
{
    int SteerCommand = (int)SteerAngle;

    //float RawSteerCommand = ConvertValue(SteerAngle, 48.0f, 58.0f);
    //int SteerIn = (int)RawSteerCommand;

    /*
    float RawThrottleCommand = ConvertValue(ThrottleAngle, 50.0f, 50.0f);
    int ThrottleFWD = (int)RawThrottleCommand + 1;   //Adding offset

    float RawReverseCommand = ConvertValue(ReverseAngle, 50.0f, 50.0f);
    int ThrottleREV = (int)RawReverseCommand + 1;

    */
    float RawThrottleCommand = ThrottleAngle * -100;
    int ThrottleFWD = (int)RawThrottleCommand + 1;   //Adding offset

    float RawReverseCommand = (float)ReverseAngle;
    int ThrottleREV = (int)RawReverseCommand + 1;

    DWORD now = GetTickCount64();
    const DWORD SEND_INTERVAL_MS = 10;   // 20 == 50 Hz, 10 == 100 Hz. Both seem fine
    
    //std::cout << "\nThrottle, reverse, steer " << SteerAngle << " " << ThrottleAngle << " "<< ReverseAngle;

    for (int i = 0; i < BoardsSelection.size(); i++)
    {
        if (BoardsSelection[i]->GetName() == "MSB")
        {
            if (MSBoard* M = dynamic_cast<MSBoard*>(BoardsSelection[i].get()))
            {
                if (now - lastSendTime >= SEND_INTERVAL_MS)
                {
                    M->SetSteer((float)SteerAngle);
                    M->SetThrottle(ThrottleAngle);
                    M->SetReverse(ReverseAngle);

                    lastSendTime = now;
                }
            }
        }
    }
}

void TelemetryUI::ClearDataPackets()
{
    for (int i = 0; i < BoardsSelection.size(); i++)
    {
        BoardsSelection[i]->ClearDataPacket();
    }
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

    T1->RenderTextVS(std::to_string(SteerAngle), 14.0f, 375.0f, 1.0f, Color);
    T1->RenderTextVS(std::to_string(TrothleAngle), 14.0f, 415.0f, 1.0f, Color);
    T1->RenderTextVS(std::to_string(ReverseAngle), 14.0f, 455.0f, 1.0f, Color);
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

    JoyStickSteer = AxesArr[0];
    ThrottleAngle = AxesArr[4];
    ReverseAngle = AxesArr[5];

    SteerAngle = JoyStickSteer;
    TrothleAngle = ThrottleAngle;

    float RawSteerCommand = ConvertValue(JoyStickSteer, 48.0f, 58.0f);
    int8_t SteerIn = (int8_t)RawSteerCommand;

    //float RawThrottleCommand = ConvertValue(ThrottleAngle, 127.5f, 127.5f);
    float RawThrottleCommand = ConvertValue(ThrottleAngle, 50.0f, 50.0f);
    int8_t ThrottleIn = (int8_t)RawThrottleCommand;

    float RawReverseCommand = ConvertValue(ReverseAngle, 50.0f, 50.0f);
    int8_t ReverseIn = (int8_t)RawReverseCommand;

    SteerAngle = SteerIn;
    TrothleAngle = ThrottleIn;
    ReverseAngle = ReverseIn;

    //ArdMotorSteer.KeepSerialActive();

    //ArdMotorSteer.SteeringI8Command(STEER_COMMAND, SteerIn);

    bool SteeringReq=0;
        
    //SteeringReq = ArdMotorSteer.ReadAndSendRequestedData(REQUEST_STEER, SteerIn);

    //int8_t Vals[4];
    int8_t Vals[6];

    Vals[0] = (int8_t)STEER_COMMAND;
    Vals[1] = SteerIn;
    Vals[2] = (int8_t)MOTOR_SPEED;
    Vals[3] = ThrottleIn;

    Vals[4] = (int8_t)MOTOR_REV_RIGHT;
    Vals[5] = ReverseIn;

    //Vals[0] = SteerIn;
    //Vals[1] = (int8_t)STEER_COMMAND;
    //Vals[2] = ThrottleIn;
    //Vals[3] = (int8_t)MOTOR_SPEED;

    T1->RenderTextVS(std::to_string(Vals[1]), 14.0f, 375.0f, 1.0f, Color);
    T1->RenderTextVS(std::to_string(Vals[3]), 14.0f, 415.0f, 1.0f, Color);
    T1->RenderTextVS(std::to_string(Vals[5]), 14.0f, 455.0f, 1.0f, Color);

    //ArdMotorSteer.SendCommand2I8(STEER_COMMAND, SteerIn);
    //ArdMotorSteer.SendCommand2I8(MOTOR_SPEED, ThrottleIn);

   

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



    if (SteeringReq) {
        T1->RenderTextVS("SR", 14.0f, 330.0f, 1.0f, Color);
        //ArdMotorSteer.SteeringI8Command(STEER_COMMAND, SteerIn);
        //ArdMotorSteer.CheckSentCommand(STEER_COMMAND);

        //int8_t SteerSent = ArdMotorSteer.GetSteeringSent();

        //T1->RenderTextVS(std::to_string(SteerIn), 14.0f, 285.0f, 1.0f, Color);
    }

}

void TelemetryUI::CalcVelocity(){

    //float AccelXCMPS = AccelX * MS_2_TO_CMS_2;
    float TimeInSec = ElapsedTime * MILISEC_TO_SEC;

    float AccelXInertia = (AccelX * cos(Pitch)) - (AccelY * sin(Roll) * sin(Pitch)) - (AccelZ * cos(Roll) * sin(Pitch));
    float AccelYInertia = (AccelY * cos(Roll)) + (AccelZ * cos(Pitch) * sin(Roll)) - (AccelX *sin(Pitch) *sin(Roll));
    //float AccelZInertia = ;

    //AccelXInertia = (AccelXInertia-1) * MS_2_TO_CMS_2;
    //AccelYInertia = (AccelYInertia - 1) * MS_2_TO_CMS_2;

    //VelocityX = VelocityX + AccelXInertia * TimeInSec;
    //VelocityY = VelocityY + AccelYInertia * TimeInSec;
    VelocityX = AccelXInertia;
    VelocityY = AccelYInertia;

    VelocityCombined = sqrt((VelocityX * VelocityX) + (VelocityY * VelocityY));


    //std::cout << "\nVELOCITY X " << VelocityX;
}

int16_t TelemetryUI::GetRadarPos() { 
    
    return RadarPos;
}

int16_t TelemetryUI::GetRadarVal() {

    return RadarVal;
}



void TelemetryUI::OpenSerial() {

    //ArdMotorSteer.KeepSerialActive();
    //ArdRadar.KeepSerialActive();

    TotalElapsedTime = 0;
}

void TelemetryUI::RenderModel() {

    

    VM1->Draw();

}
void TelemetryUI::CloseSerial() {

    //ArdGyro.CloaseSerial();
    //ArdMotorSteer.CloaseSerial();
    //ArdRadar.CloaseSerial();

    std::cout << "\n Arduino Close ";
}

void TelemetryUI::GetRecStartStopCommand(bool start, bool stop)
{
    RecStart = start;
    RecStop = stop;

    //std::cout << "\n START  " << RecStart;
    //std::cout << "\n STOP  " << RecStop;

}

void TelemetryUI::InitializeDataFile()
{
    PitchData.SetName(PitchFile);
    PitchData.OpenFileStr();

    RollData.SetName(RollFile);
    RollData.OpenFileStr();

    AccXData.SetName(AccXFile);
    AccXData.OpenFileStr();

    AccYData.SetName(AccYFile);
    AccYData.OpenFileStr();

    AccZData.SetName(AccZFile);
    AccZData.OpenFileStr();

    SteerData.SetName(SteerCommandFile);
    SteerData.OpenFileStr();

    ThrottleData.SetName(ThrottleCommandFile);
    ThrottleData.OpenFileStr();
}

void TelemetryUI::RecordData() {

    if(RecStart && !RecStop)
    {
    
        PitchData.RecordDataFloat(TotalElapsedTime,Pitch);
        RollData.RecordDataFloat(TotalElapsedTime, Roll);

        AccXData.RecordDataFloat(TotalElapsedTime, AccelX);
        AccYData.RecordDataFloat(TotalElapsedTime, AccelY);
        AccZData.RecordDataFloat(TotalElapsedTime, AccelZ);
       
        SteerFloat = (float)SteerAngle;
        ThrottleFloat = (float)TrothleAngle;

        SteerData.RecordDataFloat(TotalElapsedTime, SteerFloat);
        ThrottleData.RecordDataFloat(TotalElapsedTime, ThrottleFloat);


        //std::cout << "\n START  " << SteerFloat;
        //std::cout << "\n STOP  " << ThrottleFloat;

    }

}

void TelemetryUI::CloseDataFile() {

    PitchData.CloseFile();
    RollData.CloseFile();

    AccXData.CloseFile();
    AccYData.CloseFile();
    AccZData.CloseFile();

    SteerData.CloseFile();
    ThrottleData.CloseFile();

}

TelemetryUI::~TelemetryUI() {
	
	delete T1;
    delete VM1;
    //arduino.SerialClose();
}

void TelemetryUI::TimerAnchorPoint()
{
    ResetHB = std::chrono::steady_clock::now();
    ResetBoardReady = std::chrono::steady_clock::now();
    ResetBoardCommsEst = std::chrono::steady_clock::now();
}
void TelemetryUI::TimerFunction(int Duration)
{
    /*
    Now = std::chrono::steady_clock::now();
    std::chrono::seconds TimerDuration(Duration);

    
    if (Now - LastReset >= TimerDuration)
    {
        std::cout << "[Reset] 2 seconds have passed! TL window\n";

        LastReset += TimerDuration;
    }
    */

}


void TelemetryUI::ShowConnectionMessage(BoardSelection board, DataConcentrator& Dc)
{
    std::string Att = "13:Connecting to ",
        MSB = " MSB ",
        GYRO = " GYRO ",
        RADAR = " RADAR ";

    int MSBConn = 5001,
        GyroConn = 3001,
        RadarConn = 4001;

    std::string TotalMessage;

    switch (board)
    {
    case MOTOR_STEER_BOARD:
    {
        TotalMessage = Att + MSB;
        Dc.AddMessage(TotalMessage, 5001);
        break;
    }
    case GYROSCOPE_BOARD:
    {
        TotalMessage = Att + GYRO;
        Dc.AddMessage(TotalMessage, 3001);
        break;
    }
    case RADAR_BOARD:
    {
        TotalMessage = Att + RADAR;
        Dc.AddMessage(TotalMessage, 4001);
        break;
    }

    }


}

void TelemetryUI::ShowConnectionSuccessFul(BoardSelection board, int port, bool success, DataConcentrator& Dc)
{
    std::string Att = "13:Connected ",
        Attf = "11:Warning, ",
        NtCon = "NotConnected ",
        MSB = " MSB ",
        GYRO = " GYRO ",
        RADAR = " RADAR ",
        Port = " to port ",
        PortNum = std::to_string(port);

    int MSBConnState = 5002,
        GyroConnState = 3002,
        RadarConnState = 4002;

    switch (board)
    {
    case MOTOR_STEER_BOARD:
    {
        std::string TotalMessage;
        if (success)
        {
            TotalMessage = Att + MSB + Port + PortNum;
        }
        else
            TotalMessage = Attf + MSB + NtCon;

        Dc.AddMessage(TotalMessage, 5002);
        break;
    }
    case GYROSCOPE_BOARD:
    {
        std::string TotalMessage;
        if (success)
        {
            TotalMessage = Att + GYRO + Port + PortNum;
        }
        else
            TotalMessage = Attf + GYRO + NtCon;

        Dc.AddMessage(TotalMessage, 3002);
        break;
    }
    case RADAR_BOARD:
    {
        std::string TotalMessage;
        if (success)
        {
            TotalMessage = Att + RADAR + Port + PortNum;
        }
        else
            TotalMessage = Attf + RADAR + NtCon;

        Dc.AddMessage(TotalMessage, 4002);
        break;
    }

    }


}

int TelemetryUI::GetIdx(BoardSelection board)
{
    int idx = 0;
    for (int i = 0; i < BoardsAndIndex.size(); i++)
    {
        if (BoardsAndIndex[i].first == board)
            idx = BoardsAndIndex[i].second;
    }

    return idx;
}

int TelemetryUI::GetCode(BoardSelection board)
{
    int code = 0;
    
    switch (board)
    {
    case MOTOR_STEER_BOARD:
    {
        code = MSBMsgCode;
        break;
    }
    case GYROSCOPE_BOARD:
    {
        code = GyroMsgCode;
        break;
    }
    case RADAR_BOARD:
    {
        code = RadarMsgCode;
        break;
    }
    default:
    {
        code = 1000;
        break;
    }
    }

    return code;
}

void TelemetryUI::DetectHB(int duration,  DataConcentrator& DC, std::vector<std::pair<int, bool>> BoardHeartBeat)
{
//3004 Gyro HB detected (cyan), 3005, HB not detected (red)
//4004 Radar HB detected (cyan), 4005, HB not detected (red)
//Use Can use a Pair, first variable is the board selection, second is index
/*
BoardHeartBeat[i] -> first memeber is the board ident, second is bool for HB

this function will run outside the for loop in MonitorCommsStatus. It will iterate thru the BoardHeartBeat and select the
Indx based on the BoardHeartBeat[i]  first member . 3004, 3005 gyro HB 4004, 4005 Radar HB
Investigate the logic here, maybe timer needs to be out side of for loop. func not very realiable
*/

    ArdHeartbeat = std::chrono::steady_clock::now();
    std::chrono::seconds TimerDuration(duration);
    
    if (ArdHeartbeat - ResetHB >= TimerDuration)
    {
        ResetHB += TimerDuration;
        HeartBeatTimerElapsed = 1;
    }
    else
    {
       HeartBeatTimerElapsed = 0;
    }

    for (int i = 0; i < BoardHeartBeat.size(); i++)
    {
        int Indx = BoardHeartBeat[i].first;
        bool HBDetected = BoardHeartBeat[i].second;

        if (HBDetected)
            BoardHeartBeatCounter[Indx]++;

        if (HeartBeatTimerElapsed && (BoardHeartBeatCounter[Indx] < 1))
        {

            AddHBMessage((BoardSelection)BoardHeartBeat[i].first, 0, DC);
            //std::cout << "\nHeartbeat NOT detected ";
            //HBDetected = 0;
        }
        if (HeartBeatTimerElapsed && (BoardHeartBeatCounter[Indx] > 0))
        {

            AddHBMessage((BoardSelection)BoardHeartBeat[i].first, 1, DC);
            //std::cout << "\nHeartbeat detected, index "<< Indx;
            //HBDetected = 0;
        }

        if (HeartBeatTimerElapsed)
            BoardHeartBeatCounter[Indx] = 0;

    }

}

void TelemetryUI::DetectBoardReadiness(int duration, DataConcentrator& DC, std::vector<std::pair<int, bool>> BoardReadiness)
{
 /*
 * Board readiness detection.
 * 5006 MSB ready detected (cyan), 5007 MSB ready not detected (red)
 * 4006 Radar ready detected (cyan), 4007 radar ready not detected (red)
 * 3006 Gyro ready detected (cyan), 3007 gyro ready not detected (red)
 * 
 */
    
    ArdBoardReady = std::chrono::steady_clock::now();
    std::chrono::seconds TimerDuration(duration);

    if (ArdBoardReady - ResetBoardReady >= TimerDuration)
    {
        ResetBoardReady += TimerDuration;
        BoardReadyTimerElpased = 1;
    }
    else
    {
        BoardReadyTimerElpased = 0;
    }

    for (int i = 0; i < BoardReadiness.size(); i++)
    {
        int Indx = BoardReadiness[i].first;
        bool BaordReadyDetected = BoardReadiness[i].second;

        if (BaordReadyDetected)
            BoardReadinessCounter[Indx]++;

        if (BoardReadyTimerElpased && (BoardReadinessCounter[Indx] < 1))
        {

            AddBoardReadyMessage((BoardSelection)BoardReadiness[i].first, 0, DC);
            //std::cout << "\nHeartbeat NOT detected ";
            //HBDetected = 0;
        }
        if (BoardReadyTimerElpased && (BoardReadinessCounter[Indx] > 0))
        {

            AddBoardReadyMessage((BoardSelection)BoardReadiness[i].first, 1, DC);
            //std::cout << "\nHeartbeat detected, index "<< Indx;
            //HBDetected = 0;
        }

        if (BoardReadyTimerElpased)
            BoardReadinessCounter[Indx] = 0;

    }
}

void TelemetryUI::DetectCommsEst(int duration, DataConcentrator& DC, std::vector<std::pair<int, bool>> BoardCommsEst)
{    
/*
* Comms est detection.
* 5008 MSB comms est detected (cyan), 5009  MSB comms est  not detected (red)
* 4008 Radar comms est detected (cyan), 4009 comms est ready not detected (red)
* 3008 Gyro comms est detected (cyan), 3009 comms est ready not detected (red)
*/

    ArdCommsEst = std::chrono::steady_clock::now();
    std::chrono::seconds TimerDuration(duration);

    if (ArdCommsEst - ResetBoardCommsEst >= TimerDuration)
    {
        ResetBoardCommsEst += TimerDuration;
        CommsEstTimerElapsed = 1;
    }
    else
    {
        CommsEstTimerElapsed = 0;
    }

    for (int i = 0; i < BoardCommsEst.size(); i++)
    {
        int Indx = BoardCommsEst[i].first;
        bool CommsEstDetected = BoardCommsEst[i].second;

        if (CommsEstDetected)
            BoardCommsEstCounter[Indx]++;

        if (CommsEstTimerElapsed && (BoardCommsEstCounter[Indx] < 1))
        {

            AddCommsEstMessage((BoardSelection)BoardCommsEst[i].first, 0, DC);
            //std::cout << "\nHeartbeat NOT detected ";
            //HBDetected = 0;
        }
        if (CommsEstTimerElapsed && (BoardCommsEstCounter[Indx] > 0))
        {

            AddCommsEstMessage((BoardSelection)BoardCommsEst[i].first, 1, DC);
            //std::cout << "\nHeartbeat detected, index "<< Indx;
            //HBDetected = 0;
        }

        if (CommsEstTimerElapsed)
            BoardCommsEstCounter[Indx] = 0;

    }

}

std::string TelemetryUI::GetBoardNameStr(BoardSelection board)
{
    std::string name;

    switch (board)
    {
    case MOTOR_STEER_BOARD:
    {
        name = "MSB";
        break;
    }
    case GYROSCOPE_BOARD:
    {
        name = "Gyro";
        break;
    }
    case RADAR_BOARD:
    {
        name = "Radar";
        break;
    }
    default:
    {

        name = "board";
        break;
    }
    }

    return name;
}

void TelemetryUI::AddHBMessage(BoardSelection board, bool detected, DataConcentrator& DC)
{
    std::string MessageON = { "13:HB detected " },
        MessageOFF = { "11:Warning, HB not detected, " };

    std::string Board = GetBoardNameStr(board);

    int CodeBody = GetCode(board);

    int CodeOn = CodeBody + 4;
    int CodeOff = CodeBody + 5;

    MessageON = MessageON + Board;
    MessageOFF = MessageOFF + Board;

    if (!detected)
    {

        if (DC.CheckMessageExistence(CodeOn))
        {
            DC.SwapMessage(MessageOFF, CodeOff, CodeOn);
        }
        else
        {
            DC.SetMessageStatus(MessageOFF, CodeOff, MESSAGE_ON);
            DC.SetMessageStatus(MessageON, CodeOn, MESSAGE_OFF);
        }
        //std::cout << "\nWARNING: Heartbeat not detected ";
    }
    else
    {

        if (DC.CheckMessageExistence(CodeOff))
        {
            DC.SwapMessage(MessageON, CodeOn, CodeOff);
        }
        else
        {
            DC.SetMessageStatus(MessageOFF, CodeOff, MESSAGE_OFF);
            DC.SetMessageStatus(MessageON, CodeOn, MESSAGE_ON);
        }

        //std::cout << "\nHeartbeat detected ";
    }
}

void TelemetryUI::AddBoardReadyMessage(BoardSelection board, bool detected, DataConcentrator& DC)
{
    std::string MessageON = { "13:Board readiness detected " },
        MessageOFF = { "11:Warning, Board readiness not detected, " };

    std::string Board = GetBoardNameStr(board);

    int CodeBody = GetCode(board);

    int CodeOn = CodeBody + 6;
    int CodeOff = CodeBody + 7;

    MessageON = MessageON + Board;
    MessageOFF = MessageOFF + Board;

    if (!detected)
    {

        if (DC.CheckMessageExistence(CodeOn))
        {
            DC.SwapMessage(MessageOFF, CodeOff, CodeOn);
        }
        else
        {
            DC.SetMessageStatus(MessageOFF, CodeOff, MESSAGE_ON);
            DC.SetMessageStatus(MessageON, CodeOn, MESSAGE_OFF);
        }
        //std::cout << "\nWARNING: Readiness not detected ";
    }
    else
    {

        if (DC.CheckMessageExistence(CodeOff))
        {
            DC.SwapMessage(MessageON, CodeOn, CodeOff);
        }
        else
        {
            DC.SetMessageStatus(MessageOFF, CodeOff, MESSAGE_OFF);
            DC.SetMessageStatus(MessageON, CodeOn, MESSAGE_ON);
        }

        //std::cout << "\Readiness detected ";
    }
}

void TelemetryUI::AddCommsEstMessage(BoardSelection board, bool detected, DataConcentrator& DC)
{
    std::string MessageON = { "13:Board Comms est. " },
        MessageOFF = { "11:Warning, Board Comms not est., " };

    std::string Board = GetBoardNameStr(board);

    int CodeBody = GetCode(board);

    int CodeOn = CodeBody + 8;
    int CodeOff = CodeBody + 9;

    MessageON = MessageON + Board;
    MessageOFF = MessageOFF + Board;

    if (!detected)
    {

        if (DC.CheckMessageExistence(CodeOn))
        {
            DC.SwapMessage(MessageOFF, CodeOff, CodeOn);
        }
        else
        {
            DC.SetMessageStatus(MessageOFF, CodeOff, MESSAGE_ON);
            DC.SetMessageStatus(MessageON, CodeOn, MESSAGE_OFF);
        }
        //std::cout << "\nWARNING: Readiness not detected ";
    }
    else
    {

        if (DC.CheckMessageExistence(CodeOff))
        {
            DC.SwapMessage(MessageON, CodeOn, CodeOff);
        }
        else
        {
            DC.SetMessageStatus(MessageOFF, CodeOff, MESSAGE_OFF);
            DC.SetMessageStatus(MessageON, CodeOn, MESSAGE_ON);
        }

        //std::cout << "\Readiness detected ";
    }
}