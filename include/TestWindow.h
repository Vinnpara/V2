#pragma once

// Window creation happens in two ways. Creation and events. 
// Events are handled with messages. Messages are obtained with GetMessage and PeekMessage. 
// so first is "Get a message and wait for the next if you cannot find any" and next is 
// get message but continue if cannot find any.
// When message is received. Must be sent to the window procedure. This is done by the 
// window procedure. Done using the DispatchMessage function. Window procedure is defined by the user
// Every thing like clicks, key presses is handled in this function:
// Event ---> PeekMessage ---> DispatchMessage ---> WindowProx

#include <Windows.h>
#include <SerialPortSelection.h>
#include <vector>
#include <map>
#include <string>

#include <ColorSelection.h>

#include <chrono>

#include <GUICallBackSelection.h>

#include <GUICallBackSelection.h>
#include <PlotterGUIFunction.h>
#include <SetMenuGUIFunction.h>
#include <AutoMenuGUIFunction.h>

#include <GUIFunction.h>


#include <algorithm>

#define ID_BUTTON 1
#define ID_BUTTON_1 2 //Identifier forCOM 1
#define ID_BUTTON_2 3
#define ID_BUTTON_3 4
#define ID_BUTTON_4 5
#define ID_BUTTON_5 6
#define ID_BUTTON_6 7
#define ID_BUTTON_7 8
#define ID_BUTTON_8 9
#define ID_BUTTON_9 10
#define ID_BUTTON_10 11
#define ID_BUTTON_11 12
#define ID_BUTTON_12 13
#define ID_BUTTON_13 14
#define ID_BUTTON_14 15
#define ID_BUTTON_START_RECORD 16
#define ID_BUTTON_STOP_RECORD 17


LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static bool B1_Done, 
            B2_Done, 
	        B3_Done, 
	        ID1_sel,
	        ID2_sel,
	        ID3_sel,
	        ID4_sel,
	        ID5_sel,
	        ID6_sel,
	        ID7_sel,
	        ID8_sel,
	        ID9_sel,
	        ID10_sel,
	        ID11_sel,
	        ID12_sel,
	        ID13_sel,
	        ID14_sel
	        ; // Supports 3 boards, force user to first choose the primary board, then the second
//This is the actual window procedure arguments are: 1st: what window is being acted on
// 2nd the message, 3rd and 4th gives info about the message;

LRESULT CALLBACK WindProcDiag(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

static bool Board1Selection[15],
            Board2Selection[15],
            Board3Selection[15]
	        ;

//string GetBoardCommand(int index);


class TestWindow
{
public:
	int board1, board2, board3;
	TestWindow();
	TestWindow(bool Diagnotics);
	TestWindow(const TestWindow&) = delete; //User does not need to copy window, so delete

	TestWindow(CallBackSelection Selection);


	TestWindow& operator = (const TestWindow&) = delete; //ALso delete the = operator
	~TestWindow();
	void Assignments();
	bool ProcessMessages();

	void TimeAnchorPoint();
	void UpdateElapsedTime();

	void UpdateDaignostcs(double Pitc_val, double Roll_val, double Yaw_val, unsigned long time);
	void UpdateDaignostcs(double Pitc_val, double Roll_val, double Yaw_val, bool ValidPitch, bool ValidRoll, unsigned long Time);
	void UpdateAccelDiag(double AccelX_val, double AccelY_val, double AccelZ_val);
	void UpdateVelDiag( float velx, float vely, float velcomp);
	void UpdateElapsedTime(float ElapsedTime);

	void UpdateRadarDaignostcs(int Val, int Pos);
	void UpdateMotorSteering(int Steer, int Throttle);
	void DisplayDiagnostics();

	void GetBoardNames();
	void SetBoardParamaters();

	void ShowBoardParams();

	size_t GetNumberOfBoards() { return BoardSelections.size(); };

	std::vector <string> ReturnBoardNames() { return BoardSelections; };
	std::map<std::string, int> GetBoardsAndRates(){ return BoardSelectionAndBuadRates; };

void ShowDiagnosisData(static bool& MessagePrinted);
   void GetMessageToprint(std::vector <std::string> Messages) { MessagesToPrint = Messages; };


	LPWSTR GetPitchWritten()
	{
		return PitchWritten;
	};
	LPWSTR GetRollWritten()
	{
		return RollWritten;
	};
	LPWSTR GetYawWritten()
	{
		return YawWritten;
	};
	//For drawing

	HWND ReturnWindowHandle() {
		return m_hWnd;
	};

	void DrawDiagnostic(HWND hWnd);

	void RecordCommandButtons();

	bool RecStartStatus();
	bool RecStopStatus();

private:
	HINSTANCE m_hInstance; //connected to application
	HWND m_hWnd; //The actual window

	WNDCLASS wndClassDiag = {};  //Window class For diagnostics
	const wchar_t* DIAG_CLASS_NAME = L"Diagnosis Window Class"; //Wide character
	DWORD DiagStyle = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU; //Display close button ect.
	RECT DiagRect;   //Dimensions of the window

	WNDCLASS DiagBox = {};  // Dynamic textbox

	LPWSTR 
	PitchWritten,
	RollWritten,
	YawWritten,
	RadarPos,
	RadarVal,
    SteerPos,
	ThrottlePos,
    AccelXWritten,
	AccelYWritten,
    AccelZWritten,
	ElapsedTimeWritten,
	VelxWritten,
	VelyWritten,
	VelcWritten
	;
	bool PitchValid,
		 RollValid;

	LPWSTR ConvertToLPWSTR(std::string Data);

	std::vector<int> ColorSelector(ColorSelection Color);
	int GetColorRequested(std::string Message);
	std::string ProcessMessage(std::string Message);
	std::vector <std::string> SortMessages(std::vector <std::string> Messages);
	std::vector <std::string> MessagesToPrint;

	std::chrono::steady_clock::time_point StartPoint;
	std::vector <string> BoardSelections;

	std::map<std::string, int> BoardSelectionAndBuadRates;
};