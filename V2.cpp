// V2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define _USE_MATH_DEFINES
#include <iostream>

#include <windows.h>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<GL/GL.h>
//#include "Robot.h"
//#include"SerialPort.h"
#include <fstream>
#include <streambuf>
#include <chrono>
#include <map>

#include<serial/SerialPort.h>
#include<serial/SerialOrder.h>
#include<SerialPortSelection.h>

#include <graph\Shader.h>
#include<vector>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <math.h>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>
#include <opencv2/objdetect.hpp>

//#include<serial/SerialStream.h>
//#include<serial/SerialStreamBuf.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SerialComms.h"
#include "TextRender.h"
#include "Radar.h"
#include "GraphicRender.h"
#include <RadarUI.h>
#include <RadarVirtual.h>
#include <ResourceHandle.h>
#include <TelemetryUI.h>
#include <camera.h>

#include <ArduinoReceiver.h>
#include <Vision.h>

#include <TestWindow.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800; //800
const unsigned int SCR_HEIGHT = 600; //600

char output[MAX_DATA_LENGTH];
char incomingData[MAX_DATA_LENGTH];
char* port = "\\\\.\\COM3";
char input[MAX_DATA_LENGTH];



#define KEY(c) ( GetAsyncKeyState((int)(c)) & (SHORT)0x8000 )
using namespace std;

void LimitAngle(float max, float min, float &value);


std::vector<float> GenerateVertices( int VerticesNumber);
std::vector<float> GenerateVertices2(int VerticesNUmber);

using timer = std::chrono::system_clock;
using namespace std::chrono_literals;

timer::time_point clock_wait;
timer::time_point clock_check;
timer::duration elapsed_time;


float 
ConvertedRoll=0.77, 
ConvertedPitch=-5.3,
ConvertedYaw=2.3;

Radar* R12;
TextRender * T12;
GraphicRender* GR12;
GraphicRender* GR13;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));


const int max_value_H = 360 / 2;
const int max_value = 255;
const String window_capture_name = "Video Capture";
const String window_detection_name = "Object Detection";
int low_H = 0, low_S = 0, low_V = 0;
int high_H = max_value_H, high_S = max_value, high_V = max_value;
static void on_low_H_thresh_trackbar(int, void*)
{
    low_H = min(high_H - 1, low_H);
    setTrackbarPos("Low H", window_detection_name, low_H);
}
static void on_high_H_thresh_trackbar(int, void*)
{
    high_H = max(high_H, low_H + 1);
    setTrackbarPos("High H", window_detection_name, high_H);
}
static void on_low_S_thresh_trackbar(int, void*)
{
    low_S = min(high_S - 1, low_S);
    setTrackbarPos("Low S", window_detection_name, low_S);
}
static void on_high_S_thresh_trackbar(int, void*)
{
    high_S = max(high_S, low_S + 1);
    setTrackbarPos("High S", window_detection_name, high_S);
}
static void on_low_V_thresh_trackbar(int, void*)
{
    low_V = min(high_V - 1, low_V);
    setTrackbarPos("Low V", window_detection_name, low_V);
}
static void on_high_V_thresh_trackbar(int, void*)
{
    high_V = max(high_V, low_V + 1);
    setTrackbarPos("High V", window_detection_name, high_V);
}

void HSVSliders()
{
    // Trackbars to set thresholds for HSV values
    createTrackbar("Low H", window_detection_name, &low_H, max_value_H, on_low_H_thresh_trackbar);
    createTrackbar("High H", window_detection_name, &high_H, max_value_H, on_high_H_thresh_trackbar);
    createTrackbar("Low S", window_detection_name, &low_S, max_value, on_low_S_thresh_trackbar);
    createTrackbar("High S", window_detection_name, &high_S, max_value, on_high_S_thresh_trackbar);
    createTrackbar("Low V", window_detection_name, &low_V, max_value, on_low_V_thresh_trackbar);
    createTrackbar("High V", window_detection_name, &high_V, max_value, on_high_V_thresh_trackbar);

}

int main()
{
    // glfw: initialize and configure
// ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Vision_V2", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
   
    Shader S1("D:/V2/V2/V2/include/RadarBackgroundVertex.vs", "D:/V2/V2/V2/include/RadarBackgroundFragment.ffs");

    ShaderVision VSM;

    RadarUI Rad1;

    TelemetryUI TL1;

    std::vector<int16_t>RadarValues{0};

    for (int i = 0; i < 172; i++) {
        RadarValues.push_back(0.0);
    }
    static bool FirstPass;
    FirstPass = true;

    static bool RollReceived=false,
                PitchReceived = false,
                GyroArduinoReady = false,
                GyroArduinoCommsEstablished = false,
                GyroFirstReading = true,
                GyroHeartBeatTimerElapsed =false,
                GyroArduinoCommunicationsEstablished = false,
                GyroArduinoCommsLost =0;

    static bool RadarArduinoReady = false,
                RadarArduinoCommsEstablished = false,
                RadarFirstReading = true,
                RadarHeartBeatTimerElapsed = false,
                RadarBoardReady = false,
                RadarBoardReadinessTimerElapsed = false,
                RadarArduinoCommsLost = 0;

    static int GyroHeartBeatCounter =0,
               RadarHeartBeatCounter = 0,
               RadarBoardReadyCounter = 0;


    unsigned int CharVAO, CharVBO=0, CommonVBO=0;

    Shader S1Char("D:/V2/V2/V2/include/TextVert.vs", "D:/V2/V2/V2/include/TextFrag.ffs");

    Rad1.RadarInitalize();

    //COM5 MOTOR
    //COM8 RADAR
    //COM9 GYRO

    TL1.AssignBoards();

    TL1.InitializeTelemetry();

    cv::VideoCapture WebCAm(0);

    Vision V1(WebCAm);

    cv::Mat Frame;
    TL1.OpenSerial();

    //TestWindow* tWindow = new TestWindow(true);
    //TestWindow* tWindow = new TestWindow(true);

    TestWindow* DiagWindow = new TestWindow(true);

    TL1.InitializeDataFile();

    //TL1.OpenFile(Pitch("Data_pitch"));
    int TimerDuration = 1;
 
    //const std::chrono::seconds interval(TimerDuration);
    //std::chrono::steady_clock::time_point last_reset = std::chrono::steady_clock::now();
    TL1.TimerAnchorPoint();

    while (!glfwWindowShouldClose(window) ) {

        
        LimitAngle(180.0f, -180.0f, ConvertedYaw);
        LimitAngle(180.0f, -180.0f, ConvertedPitch);
        LimitAngle(180.0f, -180.0f, ConvertedRoll);

        

        //std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
        //TL1.TimerFunction(TimerDuration);
        
        if (GyroFirstReading || !GyroArduinoCommsEstablished)
        {
            TL1.EstablishedCommunicationsArdGyro(GyroArduinoCommsEstablished);

        }

        if (RadarFirstReading || !RadarArduinoCommsEstablished || !RadarBoardReady)
        {
            TL1.EstablishedCommunicationsArdRadar(RadarArduinoCommsEstablished);

        }

        if (!RadarBoardReady)
        {
            TL1.ListenForArduinoReadiness(RadarBoardReady);
        }

        //TL1.EstablishedCommunicationsArdRadar(RadarArduinoCommsEstablished);
        TL1.SendHeartBeat();

        //if(!GyroFirstReading && GyroArduinoCommsEstablished)
         //TL1.DetectGyroHeartBeat(TimerDuration, GyroHeartBeatTimerElapsed, GyroHeartBeatCounter);
         
        //TL1.SendHeartBeat(20);

        //if (now - last_reset >= interval) {
            // 5. Trigger the reset event
            //std::cout << "[Reset] 2 seconds have passed!\n";

            // 6. Advance the time_point anchor by adding the duration
           // last_reset += interval;
        //}

        int ControllerPresent = glfwJoystickPresent(GLFW_JOYSTICK_1);
        //cout << "\n COntroller status " << ControllerPresent;
        const float* axes = 0;
        if (ControllerPresent == 1) {

            int AxesCount;
            axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &AxesCount);
            //cout << "\n COntroller Axes " << AxesCount;

        }

        //TL1.UpdateValues3Attitude(ConvertedYaw, ConvertedPitch, ConvertedRoll);
        //TL1.Update2Axis3Accel();
        if (!GyroFirstReading && GyroArduinoCommsEstablished)
        {
            bool HBdetected = 0;
            TL1.Update2Axis3Accel(RollReceived, PitchReceived, HBdetected, GyroArduinoCommsEstablished, GyroFirstReading);
            TL1.DetectGyroHeartBeat(TimerDuration, GyroHeartBeatTimerElapsed, HBdetected, GyroHeartBeatCounter);
        }


        if (!RadarFirstReading && RadarArduinoCommsEstablished && RadarBoardReady)
        {
            bool HBdetected = 0,
                 BoardReadyDetect =0;

            TL1.UpdateValuesRadar(HBdetected, BoardReadyDetect, RadarArduinoCommsEstablished);
            TL1.DetectHeartBeat(TimerDuration, RadarHeartBeatTimerElapsed, HBdetected,  RadarHeartBeatCounter);
            TL1.DetectBoardReadiness(TimerDuration, RadarBoardReadinessTimerElapsed, BoardReadyDetect, RadarBoardReadyCounter);
        }

        TL1.CalcVelocity();
        //TL1.Update2Axis3AccelFromBuffer();
       

        //TL1.UpdateDiagnosticsWindow(DiagWindow);
        //TL1.DrawDiagnosticsData(DiagWindow);
        
        DiagWindow->UpdateDaignostcs(TL1.ReturnPitch(),TL1.ReturnRoll(),TL1.ReturnYaw(), TL1.GetPitchValid(), TL1.GetRollValid(), TL1.ReturnTime());
        DiagWindow->UpdateAccelDiag(TL1.ReturnAccelX(), TL1.ReturnAccelY(), TL1.ReturnAccelZ());
        DiagWindow->UpdateRadarDaignostcs(TL1.GetRadarVal(), TL1.GetRadarPos());
        DiagWindow->UpdateMotorSteering(TL1.ReturnSteerAngle(), TL1.ReturnThrottleAngle());
        DiagWindow->UpdateVelDiag(TL1.ReturnVelx(), TL1.ReturnVely(), TL1.ReturnVelocityCombined());
        DiagWindow->UpdateElapsedTime(TL1.ReturnTotalTime());
        DiagWindow->DrawDiagnostic(DiagWindow->ReturnWindowHandle());

        DiagWindow->RecordCommandButtons();

        //std::cout << "\nWINDOW PITCH VALUE.......  " << TL1.ReturnTime();
        //TL1.ViewDiagnostics();

        glfwPollEvents();
        HSVSliders();
        processInput(window);

       // V1.capt();
       // V1.disp();
       // V1.HSVScale(low_H, low_S, low_V, high_H, high_S, high_V);



        static bool ValidCommandRoll, ValidCommandPitch, ValidCommandYaw, ValidRadarVal, ValidRadarPos;
     
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        // S1.use();
        //TL1.UpdateValues3Attitude(ConvertedYaw, ConvertedPitch, ConvertedRoll);
        //TL1.UpdateValuues(arduino);
        //TL1.UpdateValuesRadar(RadarValue, RadarPosition);
        //Rad1.UpdateValues(arduino);

        LimitAngle(180.0f, -180.0f, ConvertedYaw);
        LimitAngle(180.0f, -180.0f, ConvertedPitch);
        LimitAngle(180.0f, -180.0f, ConvertedRoll);

        //TL1.UpdateValues3Attitude(ConvertedYaw, ConvertedPitch, ConvertedRoll);

        TL1.RenderPitch();
        TL1.RenderRoll();
        TL1.RenderYaw();
        //TL1.UpdateValuesRadar();
        TL1.RenderRadar();
        TL1.RenderModel();
        TL1.RenderControllerState(ControllerPresent);

        TL1.GetRecStartStopCommand(DiagWindow->RecStartStatus(), DiagWindow->RecStopStatus());

        int16_t RadarValue = 0, RadarPosition = 0;

        RadarValue = TL1.GetRadarVal();
        RadarPosition = TL1.GetRadarPos();

        Rad1.UpdateValues(RadarValue, RadarPosition);
        Rad1.RadarDraw();

        if (ControllerPresent == 1) {

            TL1.RenderAxis(axes);
            TL1.RenderRawSteerAngle(axes);
        }

        TL1.RecordData();

        //TL1.UpdateDiagnosticsWindow();
        //TL1.ViewDiagnostics();*/

        glfwSwapBuffers(window);
        
        //cout << "\nITERATION DONE";



        GyroFirstReading = 0;
        RadarFirstReading = 0;

    }

    //ARD1.CloaseSerial();
    TL1.CloseSerial();
    TL1.CloseDataFile();
    //delete tWindow;
    delete DiagWindow;
    glfwTerminate();
    return 0;
}


void LimitAngle(float max, float min, float& value) {

    if (value > max) {
        value = max;
    }
    if (value < min){
        value = min;
    }

}



// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}



// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
