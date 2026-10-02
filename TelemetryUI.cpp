
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

TextRender* T1;
VehicleModel* VM1;

//char output[MAX_DATA_LENGTH];
//char incomingData[MAX_DATA_LENGTH];
char* Port = "\\\\.\\COM3";
//char input[MAX_DATA_LENGTH];

//ArduinoReceiver Ard1;

//ArduinoReceiver ArdRadar(COM8);
//ArduinoReceiver ArdGyro(COM9);
//ArduinoReceiver ArdMotorSteer(COM5);

ArduinoReceiver ArdRadar(RADAR_BOARD);
ArduinoReceiver ArdGyro(GYROSCOPE_BOARD);
ArduinoReceiver ArdMotorSteer(MOTOR_STEER_BOARD);

//static float TotalElapsedTime;

float TelemetryUI::ReturnTotalTime() {

    return TotalElapsedTime;

}

TelemetryUI::TelemetryUI() {


}

void TelemetryUI::DirectConnectArd(DataConcentrator& DC)
{
    //Diagnosis only, directly connect an arduino
    
    char* GyroPort = "\\\\.\\COM19";
    ArdGyro.SetArdPort(GyroPort, DC);
    ArdGyro.SetBaudRate(BAUD_RATE_115200);
    ArdGyro.InitializedBoard();

}

void TelemetryUI::AssignBoards(DataConcentrator& DC) {
/*
* Messages:
* Board assignments, cyan
* code 1001
*/
    CallBackSelection Sel = ARD_SELECTION_AUTO_PORT_WINDOW;

   TestWindow* tWindow = new TestWindow(Sel);
   vector<string> Boards;

    bool run = true;

    while (run)
    {
        tWindow->Assignments();
        tWindow->GetBoardNames();

        tWindow->SetBoardParamaters();

        if (!tWindow->ProcessMessages())
        {
            run = false;
            //std::cout << "Close window";
            //std::cout << "\nBOARD_1 " << tWindow->board1;
            //std::cout << "\nBOARD_2 " << tWindow->board2;
            //std::cout << "\nBOARD_3 " << tWindow->board3;

            
            
        }

        Sleep(1);

    }

    /*
    if (tWindow->GetNumberOfBoards() > 0)
    {
        std::cout << "\nBoards selected";

        Boards = tWindow->ReturnBoardNames();

        for (int i = 0; i < tWindow->GetNumberOfBoards(); i++)
        {
            std::cout << "\nBoard " <<i<<" "<< Boards[i];
        }

    }
    */

   // tWindow->ShowBoardParams();

    int ArdBoard1 = tWindow->board1;
    int ArdBoard2 = tWindow->board2;
    int ArdBoard3 = tWindow->board3;

    BoardAssingmentAndBuadRate = tWindow->GetBoardsAndRates();

    std::string MSBName,
                GYROName,
                RADARName;

    int MSBBaud = 0,
        FoundPort = 0,
        GYROBaud = 0,
        RADARBaud = 0,
        GYROFoundPort = 0,
        RADARFoundPort = 0;

    for (const auto& pair : BoardAssingmentAndBuadRate) 
    {
        //std::cout << "\nBoard and rate " << pair.first << ": " << pair.second << "\n";

        if (pair.first == "MSB")
        {
            MSBName = pair.first;
            MSBBaud = pair.second;
        }
        if (pair.first == "GYRO")
        {
            GYROName = pair.first;
            GYROBaud = pair.second;
        }
        if (pair.first == "RADAR")
        {
            RADARName = pair.first;
            RADARBaud = pair.second;
        }

    }

    std::cout << "\nMSB and rate demand found as " << MSBName << " " << MSBBaud;
    std::cout << "\nGYRO and rate demand found as " << GYROName << " " << GYROBaud;
    std::cout << "\nRADAR and rate demand found as " << RADARName << " " << RADARBaud;

    //ArdMotorSteer.AssignPort((SerialName)ArdBoard1); Older code used to this 
                                                     // Changed to SetArdPort to 
                                                     //Maintain consistency.
                                                     // 
    if (ArdMotorSteer.FindArduinoBoardPort(MSBName, FoundPort, MSBBaud)) 
    {
        ArdMotorSteer.SetArdPort(FoundPort, DC);
        ArdMotorSteer.SetBaudRate(MSBBaud);
        ArdMotorSteer.InitializedBoard();

    }

    /*
    if (ArdGyro.FindArduinoBoardPort(GYROName, GYROFoundPort, GYROBaud, FoundPort))
    {
        ArdGyro.SetArdPort(GYROFoundPort, DC);
        ArdGyro.SetBaudRate(GYROBaud);
        ArdGyro.InitializedBoard();
    }
    */

    if (ArdGyro.FindArduinoBoardPortHserial(GYROName, GYROFoundPort, GYROBaud, FoundPort))
    {
        //ArdGyro.SetArdPort(GYROFoundPort, DC);
        //ArdGyro.SetBaudRate(GYROBaud);
        ArdGyro.InitializedBoard();
    }

    if (ArdGyro.FindArduinoBoardPort(RADARName, RADARFoundPort, RADARBaud, GYROFoundPort))
    {
        ArdRadar.SetArdPort(RADARFoundPort, DC);
        ArdRadar.SetBaudRate(RADARBaud);
        ArdRadar.InitializedBoard();
    }



    /*
    ArdMotorSteer.SetArdPort((SerialName)ArdBoard1, DC);
    ArdMotorSteer.SetBaudRate(BAUD_RATE_115200);
    */
    //ArdRadar.AssignPort((SerialName)ArdBoard2);
    //ArdRadar.SetBaudRate(BAUD_RATE_9600);

    //ArdRadar.SetArdPort((SerialName)ArdBoard2, DC);
    //ArdRadar.SetBaudRate(BAUD_RATE_9600);

    //std::cout << "\n Board Selected " << (SerialName)ArdBoard2 << " " << tWindow->board2;

    //ArdGyro.AssignPort((SerialName)ArdBoard3, BAUD_RATE_57600);

    //char* GyroPort = "\\\\.\\COM19";
    //ArdGyro.SetArdPort(GyroPort, DC);
    //ArdGyro.SetBaudRate(BAUD_RATE_115200);
    //ArdGyro.InitializedBoard();

    delete tWindow;
    

    /*
    * Updating messages
    */

    std::string Message = {"13:Board ports are "};
    std::string P1, P2, P3;
    P1 = std::to_string((SerialName)ArdBoard1);
    P2 = std::to_string((SerialName)ArdBoard2);
    P3 = std::to_string((SerialName)ArdBoard3);

    std::string MessageFinal = Message + P1 + ", " + P2 +", " + P3;

    DC.SetMessageStatus(MessageFinal, 1001, MESSAGE_ON);

}

void TelemetryUI::ViewDiagnostics() {

    //TestWindow* tWindow = new TestWindow(Diagnostic);

   /*bool windowrun = true;

    if (windowrun)
    {
        //tWindow->Assignments();

        double RollVal = Yaw;
        //DiagWindow->UpdateDaignostcs(RollVal);
        DiagWindow->DisplayDiagnostics();
        
        if (!DiagWindow->ProcessMessages())
        {
            windowrun = false;
            //std::cout << "Close window";
            //std::cout << "\nBOARD_1 " << tWindow->board1;
            //std::cout << "\nBOARD_2 " << tWindow->board2;
           // std::cout << "\nBOARD_3 " << tWindow->board3;

        }

        //Sleep(10);

    }*/


}

void TelemetryUI::UpdateDiagnosticsWindow(TestWindow window) {

    double PitchVal = Pitch;
    double RollVal = Roll;
    double YawVal = Yaw;
    unsigned long TimeVal = ElapsedTime;

    window.UpdateDaignostcs(PitchVal,RollVal,YawVal, TimeVal);


}

void TelemetryUI::DrawDiagnosticsData(TestWindow window) {

    
    
    //DiagWindow->DrawDiagnostic(DiagWindow->ReturnWindowHandle());


    RECT PitchText;
    PitchText.left = 30,
    PitchText.top = 10,
        PitchText.right = 100,
        PitchText.bottom = 25
        ;

    RECT RollText;
    RollText.left = 30,
        RollText.top = 40,
        RollText.right = 100,
        RollText.bottom = 25
        ;

    RECT YawText;
    YawText.left = 30,
        YawText.top = 70,
        YawText.right = 100,
        YawText.bottom = 25
        ;

    RECT rect;
    rect.left = PitchText.left + 210; //where is appears
    rect.top = PitchText.top;
    rect.right = PitchText.right;
    rect.bottom = PitchText.bottom;

    RECT rect1;
    rect1.left = RollText.left + 210; //where is appears
    rect1.top = RollText.top + 35;
    rect1.right = RollText.right;
    rect1.bottom = RollText.bottom;

    RECT rect2;
    rect2.left = YawText.left + 210; //where is appears
    rect2.top = YawText.top + 60;
    rect2.right = YawText.right;
    rect2.bottom = YawText.bottom;


    HWND hWnd  = window.ReturnWindowHandle();
    HDC dc = GetDC(hWnd);
    RECT rc;
    GetClientRect(hWnd, &rc);

    /*DrawText(dc, window.GetPitchWritten(), -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    DrawText(dc, window.GetRollWritten(), -1, &rect1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    DrawText(dc, window.GetYawWritten(), -1, &rect2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);*/
    ReleaseDC(hWnd, dc);

    //std::cout << "\nWINDOW PITCH VALUE.......  " << window.GetPitchWritten();

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

void TelemetryUI::EstablishComms(static bool& CommsEstablished, static bool& ReadyforFirstReading)
{
    ArdGyro.EstablishComms(CommsEstablished);

    if (CommsEstablished)
        ReadyforFirstReading = 1;
}

void TelemetryUI::EstablishedCommunicationsArdGyro(static bool &GyroArduinoCommsEstablished, static bool& TimerElapsed, DataConcentrator& DC)
{
 /*
 * Comms established Cyan, and amber. Cyan if comms established (1002), 
 * Amber (1003) if not.
 */

    
    std::vector<std::string> DataPackets;

    if (!GyroArduinoCommsEstablished)
        ArdGyro.EstablishComms(GyroArduinoCommsEstablished, DataPackets);
    
    ArdGyroCommsEst = std::chrono::steady_clock::now();

    std::chrono::seconds TimerDuration(1);

    if (ArdGyroCommsEst - LastReset7 >= TimerDuration)
    {
        //std::cout << "\nTwo seconds passed" << "Hartbeat "<< HeartBeatDetected;
        LastReset7 += TimerDuration;
        TimerElapsed = 1;
    }
    else
    {
        // std::cout << "Arduino Gyro heartbeat detected\n";
        TimerElapsed = 0;
    }

   // std::string MessageON = { "13:Arduino Gyro Comms est " },
              //  MessageOFF = { "12:Caution, Arduino Gyro Comms not est " };
    
    if (TimerElapsed)
    {
        HandleCommsEstMessaging(GyroArduinoCommsEstablished, DC);
    }



}

void TelemetryUI::EstablishedCommunicationsArdRadar(static bool& RadarArduinoCommsEstablished, static bool& TimerElapsed, DataConcentrator& DC)
{
 /*
 * Comms established Cyan, and amber. Cyan if comms established (1004),
 * Amber (1005) if not.
 */
    
    
    std::vector<std::string> DataPackets;

    if (!RadarArduinoCommsEstablished)
        ArdRadar.EstablishComms(RadarArduinoCommsEstablished, DataPackets);

    //ArdRadar.EstablishComms(RadarArduinoCommsEstablished, DataPackets);

    std::string MessageON = { "13:Arduino Radar Comms est " },
                MessageOFF = { "12:Caution, Arduino Radar Comms not est " };

    ArdRadarCommsEst = std::chrono::steady_clock::now();

    std::chrono::seconds TimerDuration(1);

    if (ArdRadarCommsEst - LastReset6 >= TimerDuration)
    {
        //std::cout << "\nTwo seconds passed" << "Hartbeat "<< HeartBeatDetected;
        LastReset6 += TimerDuration;
        TimerElapsed = 1;
    }
    else
    {
        // std::cout << "Arduino Gyro heartbeat detected\n";
        TimerElapsed = 0;
    }

    if (TimerElapsed)
    {
        
        
        if (RadarArduinoCommsEstablished)
        {
            if (DC.CheckMessageExistence(1005))
            {
                DC.SwapMessage(MessageON, 1004, 1005);
                //std::cout << "\nThe OFF DOES exist ";
            }
            else
            {
                DC.SetMessageStatus(MessageON, 1004, MESSAGE_ON);
                DC.SetMessageStatus(MessageOFF, 1005, MESSAGE_OFF);
                //std::cout << "\nThe OFF did not exist ";
            }
        }
        else
        {
            if (DC.CheckMessageExistence(1004))
            {
                DC.SwapMessage(MessageOFF, 1005, 1004);
                //std::cout << "\nThe ON DOES exist ";
            }
            else
            {
                DC.SetMessageStatus(MessageON, 1004, MESSAGE_OFF);
                DC.SetMessageStatus(MessageOFF, 1005, MESSAGE_ON);
                //std::cout << "\nThe ON Did not exist ";
            }
            //std::cout << "\n No comms GYRO";
        }

        
        /*
        if (RadarArduinoCommsEstablished)
        {
            DC.SetMessageStatus(MessageON, 1004, MESSAGE_ON);
            DC.SetMessageStatus(MessageOFF, 1005, MESSAGE_OFF);
        }
        else
        {
            DC.SetMessageStatus(MessageON, 1004, MESSAGE_OFF);
            DC.SetMessageStatus(MessageOFF, 1005, MESSAGE_ON);
        }
        */
    }
}

void TelemetryUI::EstablishedCommunicationsMSB(static bool& MSBCommsEstablished, static bool& TimerElapsed, DataConcentrator& DC)
{
 /*
 * Initial MSB readiness, and amber. Cyan if comms established (1014),
 * Amber (1015) if not.
 */
    std::vector<std::string> DataPackets;

    if (!MSBCommsEstablished)
        ArdMotorSteer.EstablishComms(MSBCommsEstablished, DataPackets);

    std::string MessageON = { "13:Arduino MSB ready flag det " },
                MessageOFF = { "12:Caution, MSB ready flag not det " };

    if (MSBCommsEstablished)
    {
        if (DC.CheckMessageExistence(1015))
        {
            DC.SwapMessage(MessageON, 1014, 1015);
            //std::cout << "\nThe OFF DOES exist ";
        }
        else
        {
            DC.SetMessageStatus(MessageON, 1014, MESSAGE_ON);
            DC.SetMessageStatus(MessageOFF, 1015, MESSAGE_OFF);
            //std::cout << "\nThe OFF did not exist ";
        }
    }
    else
    {
        if (DC.CheckMessageExistence(1014))
        {
            DC.SwapMessage(MessageOFF, 1015, 1014);
            //std::cout << "\nThe ON DOES exist ";
        }
        else
        {
            DC.SetMessageStatus(MessageON, 1014, MESSAGE_OFF);
            DC.SetMessageStatus(MessageOFF, 1015, MESSAGE_ON);
            //std::cout << "\nThe ON Did not exist ";
        }
        //std::cout << "\n No comms GYRO";
    }

}

void TelemetryUI::ListenForMSBRequest(bool& RequestReceived, static bool& NoFirstRequest, static int& RequestCounter, static DWORD& lastSendTime, DataConcentrator& DC)
{
    int currentRequestReceived = RequestCounter;

    DWORD currentTime = GetTickCount64(); 

    RequestReceived = ArdMotorSteer.ListenForRequest(REQUEST_COMMAND_MSB);

    if (RequestReceived)
    {
        NoFirstRequest = 0;
        RequestCounter++;
        //std::cout << "\nMSB requested data ";
    }

    //std::cout << "\nMSB current and prev "<< RequestCounter<<" "<< currentRequestReceived;
    /*
    if (currentTime - lastSendTime >= 25)
    {
        lastSendTime = currentTime;
        if (RequestCounter - currentRequestReceived == 0)
        {
            NoFirstRequest = 1;
            RequestCounter = 0;
            //std::cout << "\n--------------MSB requested data timeout-----------------";
        }
    }
    */

}

void TelemetryUI::ListenForArduinoReadiness(static bool& RadarArduinoReady, DataConcentrator& DC)
{
 /*
  * Initial radar readiness, and amber. Cyan if comms established (1006),
  * Amber (1007) if not.
  */
    
    std::vector<std::string> DataPackets;

    if (!RadarArduinoReady)
        ArdRadar.ListenForArduinoReadiness(RadarArduinoReady, DataPackets);

    std::string MessageON = { "13:Arduino Radar ready flag det " },
                MessageOFF = { "12:Caution, Radar ready flag not det " };

    
    if (RadarArduinoReady)
    {
        if (DC.CheckMessageExistence(1007))
        {
            DC.SwapMessage(MessageON, 1006, 1007);
            //std::cout << "\nThe OFF DOES exist ";
        }
        else
        {
            DC.SetMessageStatus(MessageON, 1006, MESSAGE_ON);
            DC.SetMessageStatus(MessageOFF, 1007, MESSAGE_OFF);
            //std::cout << "\nThe OFF did not exist ";
        }
    }
    else
    {
        if (DC.CheckMessageExistence(1006))
        {
            DC.SwapMessage(MessageOFF, 1007, 1006);
            //std::cout << "\nThe ON DOES exist ";
        }
        else
        {
            DC.SetMessageStatus(MessageON, 1006, MESSAGE_OFF);
            DC.SetMessageStatus(MessageOFF, 1007, MESSAGE_ON);
            //std::cout << "\nThe ON Did not exist ";
        }
        //std::cout << "\n No comms GYRO";
    }


    
    /*
    
    if (RadarArduinoReady)
    {
        DC.SetMessageStatus(MessageON, 1006, MESSAGE_ON);
        DC.SetMessageStatus(MessageOFF, 1007, MESSAGE_OFF);
    }
    else
    {
        DC.SetMessageStatus(MessageON, 1006, MESSAGE_OFF);
        DC.SetMessageStatus(MessageOFF, 1007, MESSAGE_ON);
    }

    */
}

void TelemetryUI::ListenForArduinoReadiness(BoardSelection Board, static bool& ArduinoReady, DataConcentrator& DC)
{
 /*
  * Initial Gyro, MSB readiness, and amber. (Radar has its custom function)
  * Cyan if comms established (1010) Gyro, (1012) MSB
  * Amber (1011), (1013) MSB if not. 
 */
    std::vector<std::string> DataPackets;

    std::string board;
    int MessageON = 0,
        MessageOFF = 0;

    switch (Board)
    {
       case RADAR_BOARD:
       {
           if (!ArduinoReady)
               ArdRadar.ListenForArduinoReadiness(ArduinoReady, DataPackets);

           board = ArdRadar.GetBoardMessage();

       }
       case GYROSCOPE_BOARD:
       {
           if (!ArduinoReady)
               ArdGyro.ListenForArduinoReadiness(ArduinoReady, DataPackets);

           board = ArdGyro.GetBoardMessage();

           MessageON = 1010;
           MessageOFF = 1011;
       }
       case MOTOR_STEER_BOARD:
       {
           if (!ArduinoReady)
               ArdMotorSteer.ListenForArduinoReadiness(ArduinoReady, DataPackets);

           board = ArdMotorSteer.GetBoardMessage();

           MessageON = 1012;
           MessageOFF = 1013;

       }
    }


    std::string MessageONStr = { "13:Arduino ready flag det " },
                MessageOFFStr = { "12:Caution, ready flag not det " };

    std::string MessageONComp = MessageONStr + board;
    std::string MessageOFFComp = MessageOFFStr + board;
    
    if (ArduinoReady)
    {
        if (DC.CheckMessageExistence(MessageOFF))
        {
            
            DC.SwapMessage(MessageONComp, MessageON, MessageOFF);
            //std::cout << "\nThe OFF DOES exist ";
        }
        else
        {
            
            DC.SetMessageStatus(MessageONComp, MessageON, MESSAGE_ON);
            DC.SetMessageStatus(MessageOFFComp, MessageOFF, MESSAGE_OFF);
            //std::cout << "\nThe OFF did not exist ";
        }
    }
    else
    {
        if (DC.CheckMessageExistence(MessageON))
        {
            DC.SwapMessage(MessageOFFComp, MessageOFF, MessageON);
            //std::cout << "\nThe ON DOES exist ";
        }
        else
        {
            DC.SetMessageStatus(MessageONComp, MessageON, MESSAGE_OFF);
            DC.SetMessageStatus(MessageOFFComp, MessageOFF, MESSAGE_ON);
            //std::cout << "\nThe ON Did not exist ";
        }
        //std::cout << "\n No comms GYRO";
    }

    
    /*
    
    if (RadarArduinoReady)
    {
        DC.SetMessageStatus(MessageON, 1006, MESSAGE_ON);
        DC.SetMessageStatus(MessageOFF, 1007, MESSAGE_OFF);
    }
    else
    {
        DC.SetMessageStatus(MessageON, 1006, MESSAGE_OFF);
        DC.SetMessageStatus(MessageOFF, 1007, MESSAGE_ON);
    }
    */
}

void TelemetryUI::HandleCommsEstMessaging(static bool CommsEst, DataConcentrator& DC)
{
    std::string MessageON = { "13:Arduino Gyro Comms est " },
        MessageOFF = { "12:Caution, Arduino Gyro Comms not est " };


        if (CommsEst)
        {
            if (DC.CheckMessageExistence(1003))
            {
                DC.SwapMessage(MessageON, 1002, 1003);
                //std::cout << "\nThe OFF DOES exist ";
            }
            else
            {
                DC.SetMessageStatus(MessageON, 1002, MESSAGE_ON);
                DC.SetMessageStatus(MessageOFF, 1003, MESSAGE_OFF);
                //std::cout << "\nThe OFF did not exist ";
            }
        }
        else
        {
            if (DC.CheckMessageExistence(1002))
            {
                DC.SwapMessage(MessageOFF, 1003, 1002);
                //std::cout << "\nThe ON DOES exist ";
            }
            else
            {
                DC.SetMessageStatus(MessageON, 1002, MESSAGE_OFF);
                DC.SetMessageStatus(MessageOFF, 1003, MESSAGE_ON);
                //std::cout << "\nThe ON Did not exist ";
            }
            //std::cout << "\n No comms GYRO";
        }
    
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

void TelemetryUI::UpdateValuesRadar() {
    
    //ArdRadar.ReadRadarDefaultPort();


    Radval = std::to_string(ArdRadar.GetRadarVal());
    Radpos = std::to_string(ArdRadar.GetRadarPos());

}


void TelemetryUI::UpdateValues3Attitude(float y, float p, float r) {

    ArdGyro.ReadArduinoAttitudeAccel();
    //ArdGyro.ReadArduino3Attitudes();

    //Ard1.ReadArduino3Attitudes();
    //Ard1.ReadRadar();
    //Ard1.ReadAllVals();
    //FilterVal(180.0f, -180.0f, y);
    //FilterVal(180.0f, -180.0f, p);
    //FilterVal(180.0f, -180.0f, r);

    //this->Yaw = Ard1.GetYaw();
    //this->Roll = Ard1.GetRoll();
    //this->Pitch = Ard1.GetPitch();

    //float Yaw, Pitch ,Roll;

    Yaw = ArdGyro.GetYaw();
    Pitch = ArdGyro.GetPitch();
    Roll = ArdGyro.GetRoll();

    AccelX = ArdGyro.GetAccelX();
    AccelY = ArdGyro.GetAccelY();
    AccelZ = ArdGyro.GetAccelZ();

    FilterVal(180.0f, -180.0f, Yaw);
    FilterVal(180.0f, -180.0f, Pitch);
    FilterVal(180.0f, -180.0f, Roll);

    sprintf_s(RollRead, "%f", Roll);
    sprintf_s(PtchRead, "%f", Pitch);
    sprintf_s(YawRead, "%f", Yaw);

    VM1->UpdateAttitudeValues(Yaw,Pitch,Roll);
    //UpdateValuesRadar(Ard1.GetRadarVal(), Ard1.GetRadarPos());

}

void TelemetryUI::Update2Axis3Accel() {
    
    float TempTime,
          TimeInSeconds;
    
    ArdGyro.ReadArduino3Accel2Attitude();
    //ArdGyro.ReadPitchRoll();

    //Yaw = ArdGyro.GetYaw();
    Pitch = ArdGyro.GetPitch();
    Roll = ArdGyro.GetRoll();

    AccelX = ArdGyro.GetAccelX();
    AccelY = ArdGyro.GetAccelY();
    AccelZ = ArdGyro.GetAccelZ();

    PitchValid = ArdGyro.GetValidPitch();
    RollValid = ArdGyro.GetValidRoll();

    ElapsedTime = ArdGyro.GetTime();

    /*if (ElapsedTime >= 1000 || ElapsedTime <= -1000)
        ElapsedTime = 0;*/

    //FilterVal(180.0f, -180.0f, Yaw);
    FilterVal(180.0f, -180.0f, Pitch);
    FilterVal(180.0f, -180.0f, Roll);

    sprintf_s(RollRead, "%f", Roll);
    sprintf_s(PtchRead, "%f", Pitch);
    //sprintf_s(YawRead, "%f", Yaw);
    VM1->Update2AttiudeValues(Pitch, Roll);

    TempTime = (float)ElapsedTime;
    TimeInSeconds = (TempTime / 100);


    TotalElapsedTime += TimeInSeconds;
}

void TelemetryUI::Update2Axis3Accel(static bool& RollReceived, static bool& PitchReceived, static bool& GyroArduinoCommsEstablished, static bool& FirstReading)
{
    float TempTime,
        TimeInSeconds;

    CircularBuffer<unsigned char> GyroscopeBufferIn{ 50 };
    std::vector<std::string> DataPackets;
    
    ArdGyro.RequestPitch();
    ArdGyro.ReadIntoBuffer(GyroscopeBufferIn);
    ArdGyro.ParseBuffer(GyroscopeBufferIn, DataPackets);
    ArdGyro.RefineDataPackets(DataPackets);
    ArdGyro.ProcessDataPackets(DataPackets);

    Pitch = ArdGyro.GetPitch();
    Roll = ArdGyro.GetRoll();

    if (GyroArduinoCommsEstablished)
    {

        //ArdGyro.ReadPitchRoll(RollReceived, PitchReceived, FirstReading);
        //ArdGyro.ReadIntoBuffer();

        //ArdGyro.ReadArduino3Accel2Attitude();
        //ArdGyro.ReadPitchRoll();

        //Yaw = ArdGyro.GetYaw();
        //Pitch = ArdGyro.GetPitch();
        // Roll = ArdGyro.GetRoll();

        AccelX = ArdGyro.GetAccelX();
        AccelY = ArdGyro.GetAccelY();
        AccelZ = ArdGyro.GetAccelZ();

        PitchValid = ArdGyro.GetValidPitch();
        RollValid = ArdGyro.GetValidRoll();
    }

    ElapsedTime = ArdGyro.GetTime();

    /*if (ElapsedTime >= 1000 || ElapsedTime <= -1000)
        ElapsedTime = 0;*/

        //FilterVal(180.0f, -180.0f, Yaw);
    FilterVal(180.0f, -180.0f, Pitch);
    FilterVal(180.0f, -180.0f, Roll);

    sprintf_s(RollRead, "%f", Roll);
    sprintf_s(PtchRead, "%f", Pitch);
    //sprintf_s(YawRead, "%f", Yaw);
    VM1->Update2AttiudeValues(Pitch, Roll);

    TempTime = (float)ElapsedTime;
    TimeInSeconds = (TempTime / 100);


    TotalElapsedTime += TimeInSeconds;
}

void TelemetryUI::UpdateValuesRadar(bool& HBDetected, static bool& RadarArduinoCommsEstablished) {

    /*ArdRadar.ReadRadar2();

    this->RadarVal = RadarValue;
    this->RadarPos = RadarPosition;

    Radval = std::to_string(ArdRadar.GetRadarVal());
    Radpos = std::to_string(ArdRadar.GetRadarPos());*/

    CircularBuffer<unsigned char> RadarBufferIn{ 50 };
    std::vector<std::string> DataPackets;


    ArdRadar.ReadIntoBuffer(RadarBufferIn);
    ArdRadar.ParseBufferRad(RadarBufferIn, HBDetected, DataPackets);
    ArdRadar.RefineDataPackets(DataPackets);
    ArdRadar.ProcessDataPackets(DataPackets, RADAR_BOARD);

    int RadarDist = ArdRadar.GetRadarVal();
    int RadarPos = ArdRadar.GetRadarVal();

    //std::cout << "\n Radar Distance, position " << RadarDist << " " << RadarPos;

}

void TelemetryUI::UpdateValuesRadar(bool& HBDetected, bool& BoardReadiness, static bool& RadarArduinoCommsEstablished)
{
    /*ArdRadar.ReadRadar2();

     this->RadarVal = RadarValue;
     this->RadarPos = RadarPosition;

     Radval = std::to_string(ArdRadar.GetRadarVal());
     Radpos = std::to_string(ArdRadar.GetRadarPos());*/

    CircularBuffer<unsigned char> RadarBufferIn{ 50 };
    std::vector<std::string> DataPackets;


    ArdRadar.ReadIntoBuffer(RadarBufferIn);
    ArdRadar.ParseBufferRad(RadarBufferIn, HBDetected, BoardReadiness, DataPackets);
    ArdRadar.RefineDataPackets(DataPackets);
    ArdRadar.ProcessDataPackets(DataPackets, RADAR_BOARD);

    int RadarDist = ArdRadar.GetRadarVal();
    int RadarPos = ArdRadar.GetRadarVal();

    //std::cout << "\n Radar Distance, position " << RadarDist << " " << RadarPos;

}

void TelemetryUI::UpdateValuesMSB(bool& HBDetected, bool& BoardReadiness, static bool& MSBArduinoCommsEstablished)
{
    /*ArdRadar.ReadRadar2();

     this->RadarVal = RadarValue;
     this->RadarPos = RadarPosition;

     Radval = std::to_string(ArdRadar.GetRadarVal());
     Radpos = std::to_string(ArdRadar.GetRadarPos());*/

    CircularBuffer<unsigned char> RadarBufferIn{ 50 };
    std::vector<std::string> DataPackets;


    ArdMotorSteer.ReadIntoBuffer(RadarBufferIn);
    ArdMotorSteer.ParseBufferRad(RadarBufferIn, HBDetected, BoardReadiness, DataPackets);
    ArdMotorSteer.RefineDataPackets(DataPackets);
    ArdMotorSteer.ProcessDataPackets(DataPackets, MOTOR_STEER_BOARD);

    int SteerCommand = (int)SteerAngle;

    //ArdMotorSteer.SendMessageToArd(STEER_COMMAND, SteerCommand);

    //int RadarDist = ArdRadar.GetRadarVal();
    //int RadarPos = ArdRadar.GetRadarVal();

    //std::cout << "\nSTEER COMMAND " << SteerCommand;

}

void TelemetryUI::DirectCommandMSB(DataConcentrator& DC, static DWORD& lastSendTime)
{
    int SteerCommand = (int)SteerAngle;

    //int ThrottleFWD = (int)ThrottleAngle;
    //int ThrottleREV = (int)ReverseAngle;


    float RawThrottleCommand = ConvertValue(ThrottleAngle, 50.0f, 50.0f);
    int ThrottleFWD = (int)RawThrottleCommand + 1;   //Adding offset

    float RawReverseCommand = ConvertValue(ReverseAngle, 50.0f, 50.0f);
    int ThrottleREV = (int)RawReverseCommand + 1;

    if (ThrottleFWD > ThrottleREV)
    {
        //ArdMotorSteer.SendMessageToArdTotal(STEER_COMMAND, SteerCommand, MOTOR_FWD_LEFT, ThrottleFWD, MOTOR_FWD_RIGHT, ThrottleFWD, lastSendTime); //Dual motor command
        ArdMotorSteer.SendMessageToArdTotal(STEER_COMMAND, SteerCommand, MOTOR_FWD_LEFT, ThrottleFWD, lastSendTime); //Single motor command
        //std::cout << "\nFWD " <<ThrottleFWD;
    }
    else if (ThrottleFWD < ThrottleREV)
    {
        //ArdMotorSteer.SendMessageToArdTotal(STEER_COMMAND, SteerCommand, MOTOR_REV_LEFT, ThrottleREV, MOTOR_REV_RIGHT, ThrottleREV, lastSendTime); //Dual motor command
        ArdMotorSteer.SendMessageToArdTotal(STEER_COMMAND, SteerCommand, MOTOR_REV_LEFT, ThrottleREV, lastSendTime); //Single motor command
        //std::cout << "\nREV " << ThrottleREV;
    }
    else
    {
       //std::cout << "\nNeutral";
        //ArdMotorSteer.SendMessageToArdTotal(STEER_COMMAND, SteerCommand, MOTOR_FWD_LEFT, ThrottleFWD, MOTOR_FWD_RIGHT, ThrottleFWD, lastSendTime); //Dual motor command
        ArdMotorSteer.SendMessageToArdTotal(STEER_COMMAND, SteerCommand, MOTOR_FWD_LEFT, ThrottleFWD, lastSendTime); //Single motor command
    }

    //ArdMotorSteer.SendMessageToArdTotal(STEER_COMMAND, SteerCommand, MOTOR_FWD_LEFT, ThrottleFWD, MOTOR_FWD_RIGHT, ThrottleFWD, lastSendTime);
    //std::cout << "\nFWD " << ThrottleFWD <<" REV "<< ThrottleREV;
    //ArdMotorSteer.SendMessageToArdTotal(STEER_COMMAND, SteerCommand, lastSendTime);
    //ArdMotorSteer.SendHeartBeat(DC);

}

void TelemetryUI::DirectCommandMSBTimer(DataConcentrator& DC, static DWORD& lastSendTime)
{
    DWORD now = GetTickCount64();
    const DWORD SEND_INTERVAL_MS = 10;   // 20 == 50 Hz, 10 == 100 Hz. Both seem fine

    int SteerCommand = (int)SteerAngle;

    float RawThrottleCommand = ConvertValue(ThrottleAngle, 50.0f, 50.0f);
    int ThrottleFWD = (int)RawThrottleCommand + 1;   //Adding offset

    float RawReverseCommand = ConvertValue(ReverseAngle, 50.0f, 50.0f);
    int ThrottleREV = (int)RawReverseCommand + 1;

    if (now - lastSendTime >= SEND_INTERVAL_MS)
    {

        if (ThrottleFWD > ThrottleREV)
        {
            //ArdMotorSteer.SendMessageToArdTotal(STEER_COMMAND, SteerCommand, MOTOR_FWD_LEFT, ThrottleFWD, MOTOR_FWD_RIGHT, ThrottleFWD, lastSendTime); //Dual motor command
            ArdMotorSteer.SendMessageToArdTotal(STEER_COMMAND, SteerCommand, MOTOR_FWD_LEFT, ThrottleFWD, lastSendTime); //Single motor command
            //std::cout << "\nFWD " <<ThrottleFWD;
        }
        else if (ThrottleFWD < ThrottleREV)
        {
            //ArdMotorSteer.SendMessageToArdTotal(STEER_COMMAND, SteerCommand, MOTOR_REV_LEFT, ThrottleREV, MOTOR_REV_RIGHT, ThrottleREV, lastSendTime); //Dual motor command
            ArdMotorSteer.SendMessageToArdTotal(STEER_COMMAND, SteerCommand, MOTOR_REV_LEFT, ThrottleREV, lastSendTime); //Single motor command
            //std::cout << "\nREV " << ThrottleREV;
        }
        else
        {
            //std::cout << "\nNeutral";
             //ArdMotorSteer.SendMessageToArdTotal(STEER_COMMAND, SteerCommand, MOTOR_FWD_LEFT, ThrottleFWD, MOTOR_FWD_RIGHT, ThrottleFWD, lastSendTime); //Dual motor command
            ArdMotorSteer.SendMessageToArdTotal(STEER_COMMAND, SteerCommand, MOTOR_FWD_LEFT, ThrottleFWD, lastSendTime); //Single motor command
        }

    }

}

void TelemetryUI::RequestPitch()
{
    DWORD StartTimeSend = GetTickCount();

    while (GetTickCount() - StartTimeSend < 15)
    {
        ArdGyro.RequestPitch();
        //cout << "\nReq pitch";
    }
}

void TelemetryUI::SignalReadyToBoard()
{
    DWORD StartTimeSend = GetTickCount();

    while (GetTickCount() - StartTimeSend < 50)
    {
        ArdMotorSteer.SendPCReadiness();
        ArdGyro.SendPCReadiness();
        ArdRadar.SendPCReadiness();
    }
}
void TelemetryUI::UpdateGyroAccelT()
{
    CircularBuffer<unsigned char> GyroscopeBufferIn{ 50 };
    std::vector<std::string> DataPackets;
    
    ArdGyro.ParseBufferRadT(GyroscopeBufferIn, DataPackets);
}

void TelemetryUI::UpdateGyroAccel(bool& HBDetected, bool& BoardReadiness, static bool& GyroArduinoCommsEstablished, static bool& FirstReading)
{
    float TempTime,
        TimeInSeconds;

    CircularBuffer<unsigned char> GyroscopeBufferIn{ 50 };
    std::vector<std::string> DataPackets;

    ArdGyro.RequestPitch();
    ArdGyro.ReadIntoBufferT(GyroscopeBufferIn);
    ArdGyro.ParseBufferRadT(GyroscopeBufferIn, HBDetected, BoardReadiness, DataPackets);
    ArdGyro.RefineDataPackets(DataPackets);
    ArdGyro.ProcessDataPackets(DataPackets);

    Pitch = ArdGyro.GetPitch();
    Roll = ArdGyro.GetRoll();

    if (GyroArduinoCommsEstablished)
    {

        AccelX = ArdGyro.GetAccelX();
        AccelY = ArdGyro.GetAccelY();
        AccelZ = ArdGyro.GetAccelZ();

        PitchValid = ArdGyro.GetValidPitch();
        RollValid = ArdGyro.GetValidRoll();
    }

    ElapsedTime = ArdGyro.GetTime();

    /*if (ElapsedTime >= 1000 || ElapsedTime <= -1000)
        ElapsedTime = 0;*/

        //FilterVal(180.0f, -180.0f, Yaw);
    FilterVal(180.0f, -180.0f, Pitch);
    FilterVal(180.0f, -180.0f, Roll);

    sprintf_s(RollRead, "%f", Roll);
    sprintf_s(PtchRead, "%f", Pitch);
    //sprintf_s(YawRead, "%f", Yaw);
    VM1->Update2AttiudeValues(Pitch, Roll);

    TempTime = (float)ElapsedTime;
    TimeInSeconds = (TempTime / 100);


    TotalElapsedTime += TimeInSeconds;
}

void TelemetryUI::Update2Axis3AccelFromBuffer() {

    ArdGyro.ReadBufferArduino3Accel2Attitude();
    //Yaw = ArdGyro.GetYaw();
    Pitch = ArdGyro.GetPitch();
    Roll = ArdGyro.GetRoll();

    AccelX = ArdGyro.GetAccelX();
    AccelY = ArdGyro.GetAccelY();
    AccelZ = ArdGyro.GetAccelZ();

    PitchValid = ArdGyro.GetValidPitch();
    RollValid = ArdGyro.GetValidRoll();

    //FilterVal(180.0f, -180.0f, Yaw);
    FilterVal(180.0f, -180.0f, Pitch);
    FilterVal(180.0f, -180.0f, Roll);

    sprintf_s(RollRead, "%f", Roll);
    sprintf_s(PtchRead, "%f", Pitch);
    //sprintf_s(YawRead, "%f", Yaw);
    VM1->Update2AttiudeValues(Pitch, Roll);

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

            //SerialOrder OrderWait = 0;

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
    
    return ArdRadar.GetRadarPos(); 
}

int16_t TelemetryUI::GetRadarVal() {

    return ArdRadar.GetRadarVal(); 
}

void TelemetryUI::DelayForArduinoInit()
{
//Meant to be used after arduino init.
//to give time for the arduino to finish rebooting.

    Sleep(2000);
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

    ArdGyro.CloaseSerial();
    ArdMotorSteer.CloaseSerial();
    ArdRadar.CloaseSerial();

    std::cout << "\n Arduino Close ";
}

/*
void TelemetryUI::RecordData(std::ofstream DataFile) {

    if (DataFile.is_open()) {

        DataFile << Pitch;
        DataFile << ",";
        DataFile << TotalElapsedTime;
        DataFile << "\n";

    }

}

void TelemetryUI::OpenFile(std::ofstream DataFile) {

    DataFile.open("Pitch");
}

void TelemetryUI::CloseFile(std::ofstream DataFile) {

    DataFile.close();

}
*/

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
    LastReset = std::chrono::steady_clock::now();
    LastReset2 = std::chrono::steady_clock::now();
    LastReset3 = std::chrono::steady_clock::now();
    LastReset4 = std::chrono::steady_clock::now();
    LastReset5 = std::chrono::steady_clock::now();
    LastReset6 = std::chrono::steady_clock::now();
    LastReset7 = std::chrono::steady_clock::now();
    LastReset8 = std::chrono::steady_clock::now();
    LastReset9 = std::chrono::steady_clock::now();
    LastReset10 = std::chrono::steady_clock::now();
    LastReset11 = std::chrono::steady_clock::now();
}
void TelemetryUI::TimerFunction(int Duration)
{
    Now = std::chrono::steady_clock::now();
    std::chrono::seconds TimerDuration(Duration);


    if (Now - LastReset >= TimerDuration)
    {
        std::cout << "[Reset] 2 seconds have passed! TL window\n";

        LastReset += TimerDuration;
    }


}

void TelemetryUI::DetectGyroHeartBeat(int Duration, static bool& TimerElapsed, static int& HeartBeatCounter)
{
    ArdGyroHeartbeat = std::chrono::steady_clock::now();

    bool HeartBeatDetected = ArdGyro.ListenForHeartBeat('?');

    if (HeartBeatDetected)
        HeartBeatCounter++;

    std::chrono::seconds TimerDuration(Duration);

    if (ArdGyroHeartbeat - LastReset >= TimerDuration)
    {
        //std::cout << "\nTwo seconds passed" << "Hartbeat "<< HeartBeatDetected;
        LastReset += TimerDuration;
        TimerElapsed = 1;
    }
    else
    {
      // std::cout << "Arduino Gyro heartbeat detected\n";
        TimerElapsed = 0;
    }

    if (TimerElapsed && (HeartBeatCounter < 1))
        std::cout << "WARNING: Heartbeat not detected\n";
    if(TimerElapsed && (HeartBeatCounter > 0) )
        std::cout << "Heartbeat detected\n";

    if(TimerElapsed)
        HeartBeatCounter = 0;
}

void TelemetryUI::DetectGyroHeartBeat(int Duration, static bool& TimerElapsed, bool& HeartBeatDetected,  static bool& HBDetectedGlobal, static int& HeartBeatCounter, DataConcentrator& DC)
{
   /*
   * Heart beat detection. 
   * 3004 Gyro HB detected (cyan), 3005, 3006 HB not detected (red)
   */
    
    ArdGyroHeartbeat = std::chrono::steady_clock::now();

    if (HeartBeatDetected)
        HeartBeatCounter++;

    std::chrono::seconds TimerDuration(Duration);


    if (ArdGyroHeartbeat - LastReset3 >= TimerDuration)
    {
        //std::cout << "\nTwo seconds passed" << "Hartbeat "<< HeartBeatDetected;
        LastReset3 += TimerDuration;
        TimerElapsed = 1;
    }
    else
    {
        // std::cout << "Arduino Gyro heartbeat detected\n";
        TimerElapsed = 0;
    }

    /*
    if (TimerElapsed && (HeartBeatCounter < 1))
        std::cout << "WARNING: Heartbeat not detected\n";
    if (TimerElapsed && (HeartBeatCounter > 0))
        std::cout << "Heartbeat detected\n";
    */

    /*std::string MessageON = {"13:Arduino GYRO HB det "},
                MessageOFF ={ "11:Warning, HB not det, GYRO "},
                MessageOFF2 = { "11:Warning, Check ard connection, GYRO " };

    if (TimerElapsed && (HeartBeatCounter < 1))
    {
        HBDetectedGlobal = 0;
        DC.SetMessageStatus(MessageOFF, 3005, MESSAGE_ON);
        DC.SetMessageStatus(MessageOFF2, 3006, MESSAGE_ON);
        DC.SetMessageStatus(MessageON, 3004, MESSAGE_OFF);
        //std::cout << "\nWARNING: Heartbeat not detected Gyro ";
    }
    if(TimerElapsed && (HeartBeatCounter > 0))
    {
        HBDetectedGlobal = 1;
        DC.SetMessageStatus(MessageOFF, 3005, MESSAGE_OFF);
        DC.SetMessageStatus(MessageOFF2, 3006, MESSAGE_OFF);
        DC.SetMessageStatus(MessageON, 3004, MESSAGE_ON);
        //std::cout << "\nHeartbeat detected Gyro ";
    }*/


    std::string MessageON = { "13:Arduino GYRO HB det " },
                MessageOFF = { "11:Warning, HB not det, GYRO " };

    if (TimerElapsed && (HeartBeatCounter < 1))
    {
        HBDetectedGlobal = 0;
        if (DC.CheckMessageExistence(3004))
        {
            DC.SwapMessage(MessageOFF, 3005, 3004);
        }
        else
        {
            DC.SetMessageStatus(MessageOFF, 3005, MESSAGE_ON);
            DC.SetMessageStatus(MessageON, 3004, MESSAGE_OFF);
        }
        //std::cout << "\nWARNING: Heartbeat not detected Gyro ";
    }
    if (TimerElapsed && (HeartBeatCounter > 0))
    {
        HBDetectedGlobal = 1;


        if (DC.CheckMessageExistence(3005))
        {
            DC.SwapMessage(MessageON, 3004, 3005);
        }
        else
        {
            DC.SetMessageStatus(MessageOFF, 3005, MESSAGE_OFF);
            DC.SetMessageStatus(MessageON, 3004, MESSAGE_ON);
        }

        //std::cout << "\nHeartbeat detected Gyro ";
    }


    if (TimerElapsed)
        HeartBeatCounter = 0;
}

void TelemetryUI::DetectHeartBeat(int Duration, static bool& TimerElapsed, bool HeartBeatDetected, static int& HeartBeatCounter)
{

    
    ArdRadarHeartBeat = std::chrono::steady_clock::now();

    if (HeartBeatDetected)
        HeartBeatCounter++;

    std::chrono::seconds TimerDuration(Duration);

    if (ArdRadarHeartBeat - LastReset4 >= TimerDuration)
    {
        //std::cout << "\nTwo seconds passed" << "Hartbeat "<< HeartBeatDetected;
        LastReset4 += TimerDuration;
        TimerElapsed = 1;
    }
    else
    {
        // std::cout << "Arduino Gyro heartbeat detected\n";
        TimerElapsed = 0;
    }

    /*if (TimerElapsed && (HeartBeatCounter < 1))
        std::cout << "WARNING: Heartbeat not detected\n";
    if (TimerElapsed && (HeartBeatCounter > 0))
        std::cout << "Heartbeat detected\n";

    if (TimerElapsed)
        HeartBeatCounter = 0;
        */


}

void TelemetryUI::DetectHeartBeat(int Duration, static bool& TimerElapsed, bool HeartBeatDetected, static bool& HBStatus, static int& HeartBeatCounter, DataConcentrator& DC)
{
    /*
  * Heart beat detection.
  * 4004 Radar HB detected (cyan), 4005, 4008 HB not detected (red)
  */
    
    ArdRadarHeartBeat = std::chrono::steady_clock::now();

    if (HeartBeatDetected)
        HeartBeatCounter++;

    std::chrono::seconds TimerDuration(Duration);

    if (ArdRadarHeartBeat - LastReset4 >= TimerDuration)
    {
        //std::cout << "\nTwo seconds passed" << "Hartbeat "<< HeartBeatDetected;
        LastReset4 += TimerDuration;
        TimerElapsed = 1;
    }
    else
    {
        // std::cout << "Arduino Gyro heartbeat detected\n";
        TimerElapsed = 0;
    }

    /*if (TimerElapsed && (HeartBeatCounter < 1))
    {
        std::cout << "WARNING: Heartbeat not detected\n";
        HeartBeatDetected = 0;
    }
    if (TimerElapsed && (HeartBeatCounter > 0))
    {
        std::cout << "Heartbeat detected\n";
        HeartBeatDetected = 1;
    }*/

    std::string MessageON = { "13:Arduino RADAR HB det " },
        MessageOFF = { "11:Warning, HB not det, RADAR " };

    if (TimerElapsed && (HeartBeatCounter < 1))
    {
        
        
        if (DC.CheckMessageExistence(4004))
        {
            DC.SwapMessage(MessageOFF, 4005, 4004);
        }
        else
        {
            DC.SetMessageStatus(MessageOFF, 4005, MESSAGE_ON);
            DC.SetMessageStatus(MessageON, 4004, MESSAGE_OFF);
        }
        
        //DC.SetMessageStatus(MessageOFF, 4005, MESSAGE_ON);
        //DC.SetMessageStatus(MessageON, 4004, MESSAGE_OFF);
        //std::cout << "\nWARNING: Heartbeat not detected Radar ";
    }
    if (TimerElapsed && (HeartBeatCounter > 0))
    {
        
        if (DC.CheckMessageExistence(4005))
        {
            DC.SwapMessage(MessageON, 4004, 3005);
        }
        else
        {
            DC.SetMessageStatus(MessageOFF, 4005, MESSAGE_OFF);
            DC.SetMessageStatus(MessageON, 4004, MESSAGE_ON);
        }
        
        //DC.SetMessageStatus(MessageOFF, 4005, MESSAGE_OFF);
        //DC.SetMessageStatus(MessageON, 4004, MESSAGE_ON);
        //std::cout << "\nArduino Radar heartbeat detected Radar ";
    }

    if (TimerElapsed)
        HeartBeatCounter = 0;
}

void TelemetryUI::DetectBoardReadiness(int Duration, static bool& TimerElapsed, bool BoardReadinessDetected, static int& BoardReadinessCounter, DataConcentrator& DC)
{
  /*
  * Board readiness detection.
  * 4006 Radar ready detected (cyan), 4007 radar ready not detected (red)
  */

    ArdRadarBoardReadniess = std::chrono::steady_clock::now();

    if (BoardReadinessDetected)
        BoardReadinessCounter++;

    std::chrono::seconds TimerDuration(Duration);

    if (ArdRadarBoardReadniess - LastReset5 >= TimerDuration)
    {
        //std::cout << "\nTwo seconds passed" << "Hartbeat "<< HeartBeatDetected;
        LastReset5 += TimerDuration;
        TimerElapsed = 1;
    }
    else
    {
        // std::cout << "Arduino Gyro heartbeat detected\n";
        TimerElapsed = 0;
    }

    /*if (TimerElapsed && (BoardReadinessCounter < 1))
        std::cout << "WARNING: Board comms ready flag not detected\n";
    if (TimerElapsed && (BoardReadinessCounter > 0))
        std::cout << "Board comms valid\n";*/
    /*
    std::string MessageON = { "13:Arduino RADAR Readiness det " },
        MessageOFF = { "11:Warning, Readiness not det, RADAR " };

    if (TimerElapsed && (BoardReadinessCounter < 1))
    {
        DC.SetMessageStatus(MessageOFF, 4007, MESSAGE_ON);
        DC.SetMessageStatus(MessageON, 4006, MESSAGE_OFF);
    }
    if (TimerElapsed && (BoardReadinessCounter > 0))
    {
        DC.SetMessageStatus(MessageOFF, 4007, MESSAGE_OFF);
        DC.SetMessageStatus(MessageON, 4006, MESSAGE_ON);
    }
    */
    
    std::string MessageON = { "13:Arduino RADAR Readiness det " },
                MessageOFF = { "11:Warning, Readiness not det, RADAR " };

    if (TimerElapsed && (BoardReadinessCounter < 1))
    {

        if (DC.CheckMessageExistence(4006))
        {
            DC.SwapMessage(MessageOFF, 4007, 4006);
        }
        else
        {
            DC.SetMessageStatus(MessageOFF, 4007, MESSAGE_ON);
            DC.SetMessageStatus(MessageON, 4006, MESSAGE_OFF);
        }
        //std::cout << "\nWARNING: Heartbeat not detected Gyro ";
    }
    if (TimerElapsed && (BoardReadinessCounter > 0))
    {

        if (DC.CheckMessageExistence(4007))
        {
            DC.SwapMessage(MessageON, 4006, 4007);
        }
        else
        {
            DC.SetMessageStatus(MessageOFF, 4007, MESSAGE_OFF);
            DC.SetMessageStatus(MessageON, 4006, MESSAGE_ON);
        }

        //std::cout << "\nHeartbeat detected Gyro ";
    }

    if (TimerElapsed)
        BoardReadinessCounter = 0;
}

void TelemetryUI::DetectBoardReadinessGyro(int Duration, static bool& TimerElapsed, bool BoardReadinessDetected, static int& BoardReadinessCounter, DataConcentrator& DC)
{
    /*
    * Board readiness detection. Gyro
    * 4009 Gyro ready detected (cyan), 4010 Gyro ready not detected (red)
    */

    ArdGyroBoardReadniess = std::chrono::steady_clock::now();

    if (BoardReadinessDetected)
        BoardReadinessCounter++;

    std::chrono::seconds TimerDuration(Duration);

    if (ArdGyroBoardReadniess - LastReset8 >= TimerDuration)
    {
        //std::cout << "\nTwo seconds passed" << "Hartbeat "<< HeartBeatDetected;
        LastReset8 += TimerDuration;
        TimerElapsed = 1;
    }
    else
    {
        // std::cout << "Arduino Gyro heartbeat detected\n";
        TimerElapsed = 0;
    }

    /*if (TimerElapsed && (BoardReadinessCounter < 1))
        std::cout << "WARNING: Board comms ready flag not detected\n";
    if (TimerElapsed && (BoardReadinessCounter > 0))
        std::cout << "Board comms valid\n";*/
        /*
        std::string MessageON = { "13:Arduino RADAR Readiness det " },
            MessageOFF = { "11:Warning, Readiness not det, RADAR " };

        if (TimerElapsed && (BoardReadinessCounter < 1))
        {
            DC.SetMessageStatus(MessageOFF, 4007, MESSAGE_ON);
            DC.SetMessageStatus(MessageON, 4006, MESSAGE_OFF);
        }
        if (TimerElapsed && (BoardReadinessCounter > 0))
        {
            DC.SetMessageStatus(MessageOFF, 4007, MESSAGE_OFF);
            DC.SetMessageStatus(MessageON, 4006, MESSAGE_ON);
        }
        */

    std::string MessageON = { "13:Arduino GYRO Readiness det " },
        MessageOFF = { "11:Warning, Readiness not det, GYRO " };

    if (TimerElapsed && (BoardReadinessCounter < 1))
    {

        if (DC.CheckMessageExistence(4009))
        {
            DC.SwapMessage(MessageOFF, 4010, 4009);
        }
        else
        {
            DC.SetMessageStatus(MessageOFF, 4010, MESSAGE_ON);
            DC.SetMessageStatus(MessageON, 4009, MESSAGE_OFF);
        }
        //std::cout << "\nWARNING: Heartbeat not detected Gyro ";
    }
    if (TimerElapsed && (BoardReadinessCounter > 0))
    {

        if (DC.CheckMessageExistence(4010))
        {
            DC.SwapMessage(MessageON, 4009, 4010);
        }
        else
        {
            DC.SetMessageStatus(MessageOFF, 4010, MESSAGE_OFF);
            DC.SetMessageStatus(MessageON, 4009, MESSAGE_ON);
        }

        //std::cout << "\nHeartbeat detected Gyro ";
    }

    if (TimerElapsed)
        BoardReadinessCounter = 0;
}

void TelemetryUI::DetectBoardReadinessMSB(int Duration, static bool& TimerElapsed, bool BoardReadinessDetected, static int& BoardReadinessCounter, DataConcentrator& DC)
{
/*
* Board readiness detection. MSB
* 5004 Gyro ready detected (cyan), 5005 Gyro ready not detected (red)
*/
    ArdMSBBoardReadniess = std::chrono::steady_clock::now();

    if (BoardReadinessDetected)
        BoardReadinessCounter++;

    std::chrono::seconds TimerDuration(Duration);

    if (ArdMSBBoardReadniess - LastReset9 >= TimerDuration)
    {
        //std::cout << "\nTwo seconds passed" << "Readiness "<< BoardReadinessDetected;
        LastReset9 += TimerDuration;
        TimerElapsed = 1;
    }
    else
    {
        //std::cout << "Arduino Gyro heartbeat detected\n";
        TimerElapsed = 0;
    }


    std::string MessageON = { "13:Arduino MSB Readiness det " },
        MessageOFF = { "11:Warning, Readiness not det, MSB " };

    if (TimerElapsed && (BoardReadinessCounter < 1))
    {

        if (DC.CheckMessageExistence(5004))
        {
            DC.SwapMessage(MessageOFF, 5005, 5004);
        }
        else
        {
            DC.SetMessageStatus(MessageOFF, 5005, MESSAGE_ON);
            DC.SetMessageStatus(MessageON, 5004, MESSAGE_OFF);
        }
        //std::cout << "\nWARNING: Ready flag not detected MSB ";
    }
    if (TimerElapsed && (BoardReadinessCounter > 0))
    {

        if (DC.CheckMessageExistence(5005))
        {
            DC.SwapMessage(MessageON, 5004, 5005);
        }
        else
        {
            DC.SetMessageStatus(MessageOFF, 5005, MESSAGE_OFF);
            DC.SetMessageStatus(MessageON, 5004, MESSAGE_ON);
        }

        //std::cout << "\Ready flag detected MSB ";
    }

    if (TimerElapsed)
        BoardReadinessCounter = 0;
}

void TelemetryUI::DetectMSBHeartBeat(int Duration, static bool& TimerElapsed, bool HeartBeatDetected, static bool& HBStatus, static int& HeartBeatCounter, DataConcentrator& DC)
{   
  /*
  * Heart beat detection.
  * 5006 MSB HB detected (cyan), 5007 HB not detected (red)
  */

    ArdMSBHeartBeat = std::chrono::steady_clock::now();

    if (HeartBeatDetected)
        HeartBeatCounter++;

    std::chrono::seconds TimerDuration(Duration);

    if (ArdMSBHeartBeat - LastReset10 >= TimerDuration)
    {
        //std::cout << "\nTwo seconds passed" << "Hartbeat "<< HeartBeatDetected;
        LastReset10 += TimerDuration;
        TimerElapsed = 1;
    }
    else
    {
        // std::cout << "Arduino Gyro heartbeat detected\n";
        TimerElapsed = 0;
    }

    std::string MessageON = { "13:Arduino MSB HB det " },
                MessageOFF = { "11:Warning, HB not det, MSB " };

    if (TimerElapsed && (HeartBeatCounter < 1))
    {


        if (DC.CheckMessageExistence(5006))
        {
            DC.SwapMessage(MessageOFF, 5007, 5006);
        }
        else
        {
            DC.SetMessageStatus(MessageOFF, 5007, MESSAGE_ON);
            DC.SetMessageStatus(MessageON, 5006, MESSAGE_OFF);
        }

    }
    if (TimerElapsed && (HeartBeatCounter > 0))
    {

        if (DC.CheckMessageExistence(4007))
        {
            DC.SwapMessage(MessageON, 5006, 5007);
        }
        else
        {
            DC.SetMessageStatus(MessageOFF, 5007, MESSAGE_OFF);
            DC.SetMessageStatus(MessageON, 5006, MESSAGE_ON);
        }

    }

    if (TimerElapsed)
        HeartBeatCounter = 0;

}

void TelemetryUI::CheckArduinoConnections()
{
    if (ArdMotorSteer.IsArdConnected())
    {
        //std::cout << "\nMotor Steer board connected";
    }
    else
    {
        //std::cout << "\nMotor Steer board NOT connected!!!!!!!!!!!!!!";
    }

    if (ArdGyro.IsArdConnected())
    {
        //std::cout << "\nGyro board connected";
    }
    else
    {
       // std::cout << "\nGyro board NOT connected!!!!!!!!!!!!!!";
    }

}

void TelemetryUI::SendHeartBeat(int Duration)
{
    auto now = std::chrono::steady_clock::now();
    std::chrono::milliseconds TimerDuration(Duration);

    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - LastReset2); 

    if (elapsed >= TimerDuration)
    {
        //std::cout << "\n Milliseconds elapsed " << Duration;
        //ArdGyro.SendHeartBeat();
        LastReset2 += TimerDuration;
    }
    //ArdGyro.SendHeartBeat();
}

void TelemetryUI::SendHeartBeat(DataConcentrator& DC)
{
    ArdGyro.SendHeartBeat(DC);
    ArdRadar.SendHeartBeat(DC);
    //ArdMotorSteer.SendHeartBeat(DC);
}