#include <Windows.h>
#include <string.h>
#include "TestWindow.h"
#include <iostream>
#include <sstream>

static int Board1,
           Board2,
           Board3
           ;

double Yaw,
	   Pitch,
	   Roll,
	   AccelX,
	   AccelY,
	   AccelZ
	   ;

bool  YawValid,
      PitchValid,
      RollValid,
	  AccelXValid,
	  AccelYValid,
	  AccelZValid
	  ;

LPWSTR PitchWritten,
       RollWritten,
       YawWritten,
	   AccelXWritten,
	   AccelYWritten,
	   AccelZWritten
	   ;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//Act according to the message recieved
	switch (uMsg)
	{
	case WM_CREATE: {

		////////////////////////////////////////Table titles

		CreateWindow(TEXT("STATIC"), TEXT("Arduino"), 
		WS_VISIBLE | WS_CHILD,
        10,10,
        100,25,
		hWnd,
		(HMENU) NULL,
		NULL,
		NULL
		); //to create buttons, textboxes ect takes 11 args.
		
		CreateWindow(TEXT("STATIC"), TEXT("Port"),
		WS_VISIBLE | WS_CHILD, //border is text box specific
		105, 10,
		100, 25,
		hWnd,
		(HMENU)NULL,
		NULL,
		NULL
		); 

		CreateWindow(TEXT("BUTTON"), TEXT("Confirm slection"),
		WS_VISIBLE | WS_CHILD | WS_BORDER, //border is text box specific
		200, 10,
	    115, 25,
		hWnd,
		(HMENU) ID_BUTTON,
		NULL,
		NULL
		); 

		////////////////////////////////////////Arduino listing

		CreateWindow(TEXT("STATIC"), TEXT("Motor Steering Board"),
			WS_VISIBLE | WS_CHILD,
			10, 40,
			100, 45,
			hWnd,
			(HMENU)NULL,
			NULL,
			NULL
		); 

		CreateWindow(TEXT("STATIC"), TEXT("Radar Board"),
			WS_VISIBLE | WS_CHILD,
			10, 85,
			100, 45,
			hWnd,
			(HMENU)NULL,
			NULL,
			NULL
		);

		CreateWindow(TEXT("STATIC"), TEXT("Gyro Board"),
			WS_VISIBLE | WS_CHILD,
			10, 125,
			100, 45,
			hWnd,
			(HMENU)NULL,
			NULL,
			NULL
		);

		HMENU hMenuBar = CreateMenu();
		HMENU hFile = CreateMenu();
		HMENU hPorts = CreateMenu();

		AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hFile, TEXT("File"));

		AppendMenu(hMenuBar, MF_POPUP, NULL, TEXT("Edit"));
		AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hPorts, TEXT("Ports"));

		AppendMenu(hFile, MF_STRING, NULL, TEXT("Exit"));
		AppendMenu(hPorts, MF_STRING, (UINT_PTR)ID_BUTTON_1, TEXT("COM 1"));
		AppendMenu(hPorts, MF_STRING, (UINT_PTR)ID_BUTTON_2, TEXT("COM 2"));
		AppendMenu(hPorts, MF_STRING, (UINT_PTR)ID_BUTTON_3, TEXT("COM 3"));
		AppendMenu(hPorts, MF_STRING, (UINT_PTR)ID_BUTTON_4, TEXT("COM 4"));
		AppendMenu(hPorts, MF_STRING, (UINT_PTR)ID_BUTTON_5, TEXT("COM 5"));
		AppendMenu(hPorts, MF_STRING, (UINT_PTR)ID_BUTTON_6, TEXT("COM 6"));
		AppendMenu(hPorts, MF_STRING, (UINT_PTR)ID_BUTTON_7, TEXT("COM 7"));
		AppendMenu(hPorts, MF_STRING, (UINT_PTR)ID_BUTTON_8, TEXT("COM 8"));
		AppendMenu(hPorts, MF_STRING, (UINT_PTR)ID_BUTTON_9, TEXT("COM 9"));
		AppendMenu(hPorts, MF_STRING, (UINT_PTR)ID_BUTTON_10, TEXT("COM 10"));
		AppendMenu(hPorts, MF_STRING, (UINT_PTR)ID_BUTTON_11, TEXT("COM 11"));
		AppendMenu(hPorts, MF_STRING, (UINT_PTR)ID_BUTTON_12, TEXT("COM 12"));
		AppendMenu(hPorts, MF_STRING, (UINT_PTR)ID_BUTTON_13, TEXT("COM 13"));
		AppendMenu(hPorts, MF_STRING, (UINT_PTR)ID_BUTTON_14, TEXT("COM 14"));
		SetMenu(hWnd, hMenuBar);


		//&& B1_Done && B2_Done && B3_Done


		break;

	}
	case WM_COMMAND: {

		/*board 1 cordinates
		110, 50,
		100, 25,
		board 2 cordinates
		110, 90,
		100, 25,
		board 3 cordinates 
		110, 130,
		100, 25,
		*/


		//There has to be a better way of doing this, making better use of API
		//Change, this is shit.
		
		if (!B1_Done && !B2_Done && !B3_Done) {
			//First board option

			if (LOWORD(wParam) == ID_BUTTON_1)
			{
				const char com[5] = "COM1";
				CreateWindow(TEXT("STATIC"), TEXT("COM1"),
					WS_VISIBLE | WS_CHILD,
					110, 50,
					100, 25,
					hWnd,
					(HMENU)NULL,
					NULL,
					NULL
				); //to create buttons, textboxes ect takes 11 args.

				B1_Done = 1;
				ID1_sel = 1;
				Board1 = COM1;


			}

			if (LOWORD(wParam) == ID_BUTTON_2)
			{
				CreateWindow(TEXT("STATIC"), TEXT("COM2"),
					WS_VISIBLE | WS_CHILD,
					110, 50,
					100, 25,
					hWnd,
					(HMENU)NULL,
					NULL,
					NULL
				); //to create buttons, textboxes ect takes 11 args.
				B1_Done = 1;
				ID2_sel = 1;
				Board1 = COM2;
			}

			if (LOWORD(wParam) == ID_BUTTON_3) //CHN
			{
				CreateWindow(TEXT("STATIC"), TEXT("COM3"), //CHN
					WS_VISIBLE | WS_CHILD,
					110, 50,
					100, 25,
					hWnd,
					(HMENU)NULL,
					NULL,
					NULL
				); //to create buttons, textboxes ect takes 11 args.
				B1_Done = 1;
				ID4_sel = 1; //CHN
				Board1 = COM3;
			}
			if (LOWORD(wParam) == ID_BUTTON_4)  //CHN
			{
				
				CreateWindow(TEXT("STATIC"), TEXT("COM4"),  //CHN
					WS_VISIBLE | WS_CHILD,
					110, 50,
					100, 25,
					hWnd,
					(HMENU)NULL,
					NULL,
					NULL
				); //to create buttons, textboxes ect takes 11 args.

				B1_Done = 1;
				ID4_sel = 1; //CHN
				Board1 = COM4;
			}

			if (LOWORD(wParam) == ID_BUTTON_5) //CHN
			{
				CreateWindow(TEXT("STATIC"), TEXT("COM5"), //CHN
					WS_VISIBLE | WS_CHILD,
					110, 50,
					100, 25,
					hWnd,
					(HMENU)NULL,
					NULL,
					NULL
				); //to create buttons, textboxes ect takes 11 args.
				B1_Done = 1;
				ID5_sel = 1; //CHN
				Board1 = COM5;
			}

			if (LOWORD(wParam) == ID_BUTTON_6) //CHN
			{
				CreateWindow(TEXT("STATIC"), TEXT("COM6"), //CHN
					WS_VISIBLE | WS_CHILD,
					110, 50,
					100, 25,
					hWnd,
					(HMENU)NULL,
					NULL,
					NULL
				); //to create buttons, textboxes ect takes 11 args.
				B1_Done = 1;
				ID6_sel = 1; //CHN
				Board1 = COM6;
			}

			if (LOWORD(wParam) == ID_BUTTON_7) //CHN
			{
				CreateWindow(TEXT("STATIC"), TEXT("COM7"), //CHN
					WS_VISIBLE | WS_CHILD,
					110, 50,
					100, 25,
					hWnd,
					(HMENU)NULL,
					NULL,
					NULL
				); //to create buttons, textboxes ect takes 11 args.
				B1_Done = 1;
				ID7_sel = 1; //CHN
				Board1 = COM7;
			}

			if (LOWORD(wParam) == ID_BUTTON_8) //CHN
			{
				CreateWindow(TEXT("STATIC"), TEXT("COM8"), //CHN
					WS_VISIBLE | WS_CHILD,
					110, 50,
					100, 25,
					hWnd,
					(HMENU)NULL,
					NULL,
					NULL
				); //to create buttons, textboxes ect takes 11 args.
				B1_Done = 1;
				ID8_sel = 1; //CHN
				Board1 = COM8;
			}

			if (LOWORD(wParam) == ID_BUTTON_9) //CHN
			{
				CreateWindow(TEXT("STATIC"), TEXT("COM9"), //CHN
					WS_VISIBLE | WS_CHILD,
					110, 50,
					100, 25,
					hWnd,
					(HMENU)NULL,
					NULL,
					NULL
				); //to create buttons, textboxes ect takes 11 args.
				B1_Done = 1;
				ID9_sel = 1; //CHN
				Board1 = COM9;
			}


			if (LOWORD(wParam) == ID_BUTTON_10) //CHN
			{
				CreateWindow(TEXT("STATIC"), TEXT("COM10"), //CHN
					WS_VISIBLE | WS_CHILD,
					110, 50,
					100, 25,
					hWnd,
					(HMENU)NULL,
					NULL,
					NULL
				); //to create buttons, textboxes ect takes 11 args.
				B1_Done = 1;
				ID10_sel = 1; //CHN
				Board1 = COM10;
			}


			if (LOWORD(wParam) == ID_BUTTON_11) //CHN
			{
				CreateWindow(TEXT("STATIC"), TEXT("COM11"), //CHN
					WS_VISIBLE | WS_CHILD,
					110, 50,
					100, 25,
					hWnd,
					(HMENU)NULL,
					NULL,
					NULL
				); //to create buttons, textboxes ect takes 11 args.
				B1_Done = 1;
				ID11_sel = 1; //CHN
				Board1 = COM11;
			}

			if (LOWORD(wParam) == ID_BUTTON_12) //CHN
			{
				CreateWindow(TEXT("STATIC"), TEXT("COM12"), //CHN
					WS_VISIBLE | WS_CHILD,
					110, 50,
					100, 25,
					hWnd,
					(HMENU)NULL,
					NULL,
					NULL
				); //to create buttons, textboxes ect takes 11 args.
				B1_Done = 1;
				ID12_sel = 1; //CHN
				Board1 = COM12;
			}


			if (LOWORD(wParam) == ID_BUTTON_13) //CHN
			{
				CreateWindow(TEXT("STATIC"), TEXT("COM13"), //CHN
					WS_VISIBLE | WS_CHILD,
					110, 50,
					100, 25,
					hWnd,
					(HMENU)NULL,
					NULL,
					NULL
				); //to create buttons, textboxes ect takes 11 args.
				B1_Done = 1;
				ID13_sel = 1; //CHN
				Board1 = COM13;
			}

			if (LOWORD(wParam) == ID_BUTTON_14) //CHN
			{
				CreateWindow(TEXT("STATIC"), TEXT("COM14"), //CHN
					WS_VISIBLE | WS_CHILD,
					110, 50,
					100, 25,
					hWnd,
					(HMENU)NULL,
					NULL,
					NULL
				); //to create buttons, textboxes ect takes 11 args.
				B1_Done = 1;
				ID14_sel = 1; //CHN
				Board1 = COM14;
			}


		}

		if (B1_Done && !B2_Done && !B3_Done) {
			//Second board option

			if (LOWORD(wParam) == ID_BUTTON_1 && !ID1_sel)
			{
				const char com[5] = "COM1";
				CreateWindow(TEXT("STATIC"), TEXT("COM1"),
					WS_VISIBLE | WS_CHILD,
					110, 90,
					100, 25,
					hWnd,
					(HMENU)NULL,
					NULL,
					NULL
				); //to create buttons, textboxes ect takes 11 args.

				B2_Done = 1;
				ID1_sel = 1;
				Board2 = COM1;
			}

			if (LOWORD(wParam) == ID_BUTTON_2 && !ID2_sel)
			{
				CreateWindow(TEXT("STATIC"), TEXT("COM2"),
					WS_VISIBLE | WS_CHILD,
					110, 90,
					100, 25,
					hWnd,
					(HMENU)NULL,
					NULL,
					NULL
				); //to create buttons, textboxes ect takes 11 args.
				B2_Done = 1;
				ID2_sel = 1;
				Board2 = COM2;
			}

			if (LOWORD(wParam) == ID_BUTTON_3 && !ID3_sel) //CHN
			{
				CreateWindow(TEXT("STATIC"), TEXT("COM3"), //CHN
					WS_VISIBLE | WS_CHILD,
					110, 90,
					100, 25,
					hWnd,
					(HMENU)NULL,
					NULL,
					NULL
				); //to create buttons, textboxes ect takes 11 args.
				B2_Done = 1;
				ID3_sel = 1; //CHN
				Board2 = COM3;
			}


			if (LOWORD(wParam) == ID_BUTTON_4 && !ID4_sel) //CHN
			{
				CreateWindow(TEXT("STATIC"), TEXT("COM4"), //CHN
					WS_VISIBLE | WS_CHILD,
					110, 90,
					100, 25,
					hWnd,
					(HMENU)NULL,
					NULL,
					NULL
				); //to create buttons, textboxes ect takes 11 args.
				B2_Done = 1;
				ID4_sel = 1; //CHN
				Board2 = COM4;
			}


			if (LOWORD(wParam) == ID_BUTTON_5 && !ID5_sel) //CHN
			{
				CreateWindow(TEXT("STATIC"), TEXT("COM5"), //CHN
					WS_VISIBLE | WS_CHILD,
					110, 90,
					100, 25,
					hWnd,
					(HMENU)NULL,
					NULL,
					NULL
				); //to create buttons, textboxes ect takes 11 args.
				B2_Done = 1;
				ID5_sel = 1; //CHN
				Board2 = COM5;
			}

			if (LOWORD(wParam) == ID_BUTTON_6 && !ID6_sel) //CHN
			{
				CreateWindow(TEXT("STATIC"), TEXT("COM6"), //CHN
					WS_VISIBLE | WS_CHILD,
					110, 90,
					100, 25,
					hWnd,
					(HMENU)NULL,
					NULL,
					NULL
				); //to create buttons, textboxes ect takes 11 args.
				B2_Done = 1;
				ID6_sel = 1; //CHN
				Board2 = COM6;
			}

			if (LOWORD(wParam) == ID_BUTTON_7 && !ID7_sel) //CHN
			{
				CreateWindow(TEXT("STATIC"), TEXT("COM7"), //CHN
					WS_VISIBLE | WS_CHILD,
					110, 90,
					100, 25,
					hWnd,
					(HMENU)NULL,
					NULL,
					NULL
				); //to create buttons, textboxes ect takes 11 args.
				B2_Done = 1;
				ID7_sel = 1; //CHN
				Board2 = COM7;
			}

			if (LOWORD(wParam) == ID_BUTTON_8 && !ID8_sel) //CHN
			{
				CreateWindow(TEXT("STATIC"), TEXT("COM8"), //CHN
					WS_VISIBLE | WS_CHILD,
					110, 90,
					100, 25,
					hWnd,
					(HMENU)NULL,
					NULL,
					NULL
				); //to create buttons, textboxes ect takes 11 args.
				B2_Done = 1;
				ID8_sel = 1; //CHN
				Board2 = COM8;
			}

			if (LOWORD(wParam) == ID_BUTTON_9 && !ID9_sel) //CHN
			{
				CreateWindow(TEXT("STATIC"), TEXT("COM9"), //CHN
					WS_VISIBLE | WS_CHILD,
					110, 90,
					100, 25,
					hWnd,
					(HMENU)NULL,
					NULL,
					NULL
				); //to create buttons, textboxes ect takes 11 args.
				B2_Done = 1;
				ID9_sel = 1; //CHN
				Board2 = COM9;
			}

			if (LOWORD(wParam) == ID_BUTTON_10 && !ID10_sel) //CHN
			{
				CreateWindow(TEXT("STATIC"), TEXT("COM10"), //CHN
					WS_VISIBLE | WS_CHILD,
					110, 90,
					100, 25,
					hWnd,
					(HMENU)NULL,
					NULL,
					NULL
				); //to create buttons, textboxes ect takes 11 args.
				B2_Done = 1;
				ID10_sel = 1; //CHN
				Board2 = COM10;
			}

			if (LOWORD(wParam) == ID_BUTTON_11 && !ID11_sel) //CHN
			{
				CreateWindow(TEXT("STATIC"), TEXT("COM11"), //CHN
					WS_VISIBLE | WS_CHILD,
					110, 90,
					100, 25,
					hWnd,
					(HMENU)NULL,
					NULL,
					NULL
				); //to create buttons, textboxes ect takes 11 args.
				B2_Done = 1;
				ID11_sel = 1; //CHN
				Board2 = COM11;
			}

			if (LOWORD(wParam) == ID_BUTTON_12 && !ID12_sel) //CHN
			{
				CreateWindow(TEXT("STATIC"), TEXT("COM12"), //CHN
					WS_VISIBLE | WS_CHILD,
					110, 90,
					100, 25,
					hWnd,
					(HMENU)NULL,
					NULL,
					NULL
				); //to create buttons, textboxes ect takes 11 args.
				B2_Done = 1;
				ID12_sel = 1; //CHN
				Board2 = COM12;
			}

			if (LOWORD(wParam) == ID_BUTTON_13 && !ID13_sel) //CHN
			{
				CreateWindow(TEXT("STATIC"), TEXT("COM13"), //CHN
					WS_VISIBLE | WS_CHILD,
					110, 90,
					100, 25,
					hWnd,
					(HMENU)NULL,
					NULL,
					NULL
				); //to create buttons, textboxes ect takes 11 args.
				B2_Done = 1;
				ID13_sel = 1; //CHN
				Board2 = COM13;
			}

			if (LOWORD(wParam) == ID_BUTTON_14 && !ID14_sel) //CHN
			{
				CreateWindow(TEXT("STATIC"), TEXT("COM14"), //CHN
					WS_VISIBLE | WS_CHILD,
					110, 90,
					100, 25,
					hWnd,
					(HMENU)NULL,
					NULL,
					NULL
				); //to create buttons, textboxes ect takes 11 args.
				B2_Done = 1;
				ID14_sel = 1; //CHN
				Board2 = COM14;
			}

		}

		if (B1_Done && B2_Done && !B3_Done) {
			//third board option

			if (LOWORD(wParam) == ID_BUTTON_1 && !ID1_sel)
			{
				const char com[5] = "COM1";
				CreateWindow(TEXT("STATIC"), TEXT("COM1"),
					WS_VISIBLE | WS_CHILD,
					110, 130,
					100, 25,
					hWnd,
					(HMENU)NULL,
					NULL,
					NULL
				); //to create buttons, textboxes ect takes 11 args.

				B3_Done = 1;
				ID1_sel = 1;
				Board3 = COM1;
			}

			if (LOWORD(wParam) == ID_BUTTON_2 && !ID2_sel)
			{
				CreateWindow(TEXT("STATIC"), TEXT("COM2"),
					WS_VISIBLE | WS_CHILD,
					110, 130,
					100, 25,
					hWnd,
					(HMENU)NULL,
					NULL,
					NULL
				); //to create buttons, textboxes ect takes 11 args.
				B3_Done = 1;
				ID2_sel = 1;
				Board3 = COM2;
			}

			if (LOWORD(wParam) == ID_BUTTON_3 && !ID3_sel) //CHN
			{
				CreateWindow(TEXT("STATIC"), TEXT("COM3"), //CHN
					WS_VISIBLE | WS_CHILD,
					110, 130,
					100, 25,
					hWnd,
					(HMENU)NULL,
					NULL,
					NULL
				); //to create buttons, textboxes ect takes 11 args.
				B3_Done = 1;
				ID3_sel = 1; //CNH
				Board3 = COM3;
			}

			if (LOWORD(wParam) == ID_BUTTON_4 && !ID4_sel) //CHN
			{
				CreateWindow(TEXT("STATIC"), TEXT("COM4"), //CHN
					WS_VISIBLE | WS_CHILD,
					110, 130,
					100, 25,
					hWnd,
					(HMENU)NULL,
					NULL,
					NULL
				); //to create buttons, textboxes ect takes 11 args.
				B3_Done = 1;
				ID4_sel = 1; //CNH
				Board3 = COM4;
			}

			if (LOWORD(wParam) == ID_BUTTON_5 && !ID5_sel) //CHN
			{
				CreateWindow(TEXT("STATIC"), TEXT("COM5"), //CHN
					WS_VISIBLE | WS_CHILD,
					110, 130,
					100, 25,
					hWnd,
					(HMENU)NULL,
					NULL,
					NULL
				); //to create buttons, textboxes ect takes 11 args.
				B3_Done = 1;
				ID5_sel = 1; //CNH
				Board3 = COM5;
			}

			if (LOWORD(wParam) == ID_BUTTON_6 && !ID6_sel) //CHN
			{
				CreateWindow(TEXT("STATIC"), TEXT("COM6"), //CHN
					WS_VISIBLE | WS_CHILD,
					110, 130,
					100, 25,
					hWnd,
					(HMENU)NULL,
					NULL,
					NULL
				); //to create buttons, textboxes ect takes 11 args.
				B3_Done = 1;
				ID6_sel = 1; //CNH
				Board3 = COM6;
			}

			if (LOWORD(wParam) == ID_BUTTON_7 && !ID7_sel) //CHN
			{
				CreateWindow(TEXT("STATIC"), TEXT("COM7"), //CHN
					WS_VISIBLE | WS_CHILD,
					110, 130,
					100, 25,
					hWnd,
					(HMENU)NULL,
					NULL,
					NULL
				); //to create buttons, textboxes ect takes 11 args.
				B3_Done = 1;
				ID7_sel = 1; //CNH
				Board3 = COM7;
			}

			if (LOWORD(wParam) == ID_BUTTON_8 && !ID8_sel) //CHN
			{
				CreateWindow(TEXT("STATIC"), TEXT("COM8"), //CHN
					WS_VISIBLE | WS_CHILD,
					110, 130,
					100, 25,
					hWnd,
					(HMENU)NULL,
					NULL,
					NULL
				); //to create buttons, textboxes ect takes 11 args.
				B3_Done = 1;
				ID8_sel = 1; //CNH
				Board3 = COM8;
			}

			if (LOWORD(wParam) == ID_BUTTON_9 && !ID9_sel) //CHN
			{
				CreateWindow(TEXT("STATIC"), TEXT("COM9"), //CHN
					WS_VISIBLE | WS_CHILD,
					110, 130,
					100, 25,
					hWnd,
					(HMENU)NULL,
					NULL,
					NULL
				); //to create buttons, textboxes ect takes 11 args.
				B3_Done = 1;
				ID9_sel = 1; //CNH
				Board3 = COM9;
			}

			if (LOWORD(wParam) == ID_BUTTON_10 && !ID10_sel) //CHN
			{
				CreateWindow(TEXT("STATIC"), TEXT("COM10"), //CHN
					WS_VISIBLE | WS_CHILD,
					110, 130,
					100, 25,
					hWnd,
					(HMENU)NULL,
					NULL,
					NULL
				); //to create buttons, textboxes ect takes 11 args.
				B3_Done = 1;
				ID10_sel = 1; //CNH
				Board3 = COM10;
			}

			if (LOWORD(wParam) == ID_BUTTON_11 && !ID11_sel) //CHN
			{
				CreateWindow(TEXT("STATIC"), TEXT("COM11"), //CHN
					WS_VISIBLE | WS_CHILD,
					110, 130,
					100, 25,
					hWnd,
					(HMENU)NULL,
					NULL,
					NULL
				); //to create buttons, textboxes ect takes 11 args.
				B3_Done = 1;
				ID11_sel = 1; //CNH
				Board3 = COM11;
			}

			if (LOWORD(wParam) == ID_BUTTON_12 && !ID12_sel) //CHN
			{
				CreateWindow(TEXT("STATIC"), TEXT("COM12"), //CHN
					WS_VISIBLE | WS_CHILD,
					110, 130,
					100, 25,
					hWnd,
					(HMENU)NULL,
					NULL,
					NULL
				); //to create buttons, textboxes ect takes 11 args.
				B3_Done = 1;
				ID12_sel = 1; //CNH
				Board3 = COM12;
			}

			if (LOWORD(wParam) == ID_BUTTON_13 && !ID13_sel) //CHN
			{
				CreateWindow(TEXT("STATIC"), TEXT("COM13"), //CHN
					WS_VISIBLE | WS_CHILD,
					110, 130,
					100, 25,
					hWnd,
					(HMENU)NULL,
					NULL,
					NULL
				); //to create buttons, textboxes ect takes 11 args.
				B3_Done = 1;
				ID13_sel = 1; //CNH
				Board3 = COM13;
			}

			if (LOWORD(wParam) == ID_BUTTON_14 && !ID14_sel) //CHN
			{
				CreateWindow(TEXT("STATIC"), TEXT("COM14"), //CHN
					WS_VISIBLE | WS_CHILD,
					110, 130,
					100, 25,
					hWnd,
					(HMENU)NULL,
					NULL,
					NULL
				); //to create buttons, textboxes ect takes 11 args.
				B3_Done = 1;
				ID14_sel = 1; //CNH
				Board3 = COM14;
			}



		}






		/*CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
		CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
		___________________END OF BOARD ASSINGMENT____________________
		CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
		CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
		*/



		if (LOWORD(wParam) == ID_BUTTON)
		{

			if (B1_Done && B2_Done && B3_Done) {

				CreateWindow(TEXT("STATIC"), TEXT("Ports Assigned"),
					WS_VISIBLE | WS_CHILD,
					200, 200,
					175, 45,
					hWnd,
					(HMENU)NULL,
					NULL,
					NULL
				);
			}
			else if ((!B1_Done || !B2_Done || !B3_Done) && !(B1_Done && B2_Done && B3_Done))
			{
				CreateWindow(TEXT("STATIC"), TEXT("WARN: Some Ports Not Assigned"),
					WS_VISIBLE | WS_CHILD,
					200, 200,
					175, 45,
					hWnd,
					(HMENU)NULL,
					NULL,
					NULL
				);


			}

		}


		break;
	}
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	
    case WM_DESTROY:
		PostQuitMessage(0); //quit application once destroyed
		return 0;

    }

	return DefWindowProc(hWnd, uMsg, wParam, lParam); //return undefiened params as well
};

LRESULT CALLBACK WindowProcBlank(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	switch (uMsg)
	{
	case WM_CREATE: {

		////////////////////////////////////////Table titles

		CreateWindow(TEXT("STATIC"), TEXT("Diag"),
			WS_VISIBLE | WS_CHILD,
			150, 10,
			100, 25,
			hWnd,
			(HMENU)NULL,
			NULL,
			NULL
		); //to create buttons, textboxes ect takes 11 args

		break;

	};

	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam); //return undefiened params as well

}

LRESULT CALLBACK WindProcDiag(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	switch (uMsg)
	{
	case WM_CREATE: {

		////////////////////////////////////////Table titles

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

		RECT RadarValText;
		RadarValText.left = 30,
			RadarValText.top = 100,
			RadarValText.right = 100,
			RadarValText.bottom = 25
			;

		RECT RadarPosText;
		RadarPosText.left = 30,
			RadarPosText.top = 140,
			RadarPosText.right = 100,
			RadarPosText.bottom = 25
			;


		RECT SteerText;
		SteerText.left = 30,
			SteerText.top = 180,
			SteerText.right = 100,
			SteerText.bottom = 25
			;

		RECT ThrottleText;
		ThrottleText.left = 30,
			ThrottleText.top = 220,
			ThrottleText.right = 100,
			ThrottleText.bottom = 25
			;

		RECT AccelXText;
		AccelXText.left = 265,
			AccelXText.top = 10,
			AccelXText.right = 100,
			AccelXText.bottom = 25
			;

		RECT AccelYText;
		AccelYText.left = 265,
			AccelYText.top = 40,
			AccelYText.right = 100,
			AccelYText.bottom = 25
			;

		RECT AccelZText;
		AccelZText.left = 265,
			AccelZText.top = 70,
			AccelZText.right = 100,
			AccelZText.bottom = 25
			;

		CreateWindow(TEXT("STATIC"), TEXT("Pitch"),
			WS_VISIBLE | WS_CHILD,
			PitchText.left, PitchText.top,
			PitchText.right, PitchText.bottom,
			hWnd,
			(HMENU)NULL,
			NULL,
			NULL
		); //to create buttons, textboxes ect takes 11 args

		CreateWindow(TEXT("STATIC"), TEXT("Roll"),
			WS_VISIBLE | WS_CHILD,
			RollText.left, RollText.top,
			RollText.right, RollText.bottom,
			hWnd,
			(HMENU)NULL,
			NULL,
			NULL
		); //to create buttons, textboxes ect takes 11 args

		CreateWindow(TEXT("STATIC"), TEXT("Yaw"),
			WS_VISIBLE | WS_CHILD,
			YawText.left, YawText.top,
			YawText.right, YawText.bottom,
			hWnd,
			(HMENU)NULL,
			NULL,
			NULL
		); //to create buttons, textboxes ect takes 11 args

		CreateWindow(TEXT("STATIC"), TEXT("Radar val"),
			WS_VISIBLE | WS_CHILD,
			RadarValText.left, RadarValText.top,
			RadarValText.right, RadarValText.bottom,
			hWnd,
			(HMENU)NULL,
			NULL,
			NULL
		); //to create buttons, textboxes ect takes 11 args

		CreateWindow(TEXT("STATIC"), TEXT("Radar Pos"),
			WS_VISIBLE | WS_CHILD,
			RadarPosText.left, RadarPosText.top,
			RadarPosText.right, RadarPosText.bottom,
			hWnd,
			(HMENU)NULL,
			NULL,
			NULL
		); //to create buttons, textboxes ect takes 11 args

		CreateWindow(TEXT("STATIC"), TEXT("Steer"),
			WS_VISIBLE | WS_CHILD,
			SteerText.left, SteerText.top,
			SteerText.right, SteerText.bottom,
			hWnd,
			(HMENU)NULL,
			NULL,
			NULL
		); //to create buttons, textboxes ect takes 11 args

		CreateWindow(TEXT("STATIC"), TEXT("Throttle"),
			WS_VISIBLE | WS_CHILD,
			ThrottleText.left, ThrottleText.top,
			ThrottleText.right, ThrottleText.bottom,
			hWnd,
			(HMENU)NULL,
			NULL,
			NULL
		); //to create buttons, textboxes ect takes 11 args

		CreateWindow(TEXT("STATIC"), TEXT("Accel X"),
			WS_VISIBLE | WS_CHILD,
			AccelXText.left, AccelXText.top,
			AccelXText.right, AccelXText.bottom,
			hWnd,
			(HMENU)NULL,
			NULL,
			NULL
		); //to create buttons, textboxes ect takes 11 args

		CreateWindow(TEXT("STATIC"), TEXT("Accel Y"),
			WS_VISIBLE | WS_CHILD,
			AccelYText.left, AccelYText.top,
			AccelYText.right, AccelYText.bottom,
			hWnd,
			(HMENU)NULL,
			NULL,
			NULL
		); //to create buttons, textboxes ect takes 11 args

		CreateWindow(TEXT("STATIC"), TEXT("Accel Z"),
			WS_VISIBLE | WS_CHILD,
			AccelZText.left, AccelZText.top,
			AccelZText.right, AccelZText.bottom,
			hWnd,
			(HMENU)NULL,
			NULL,
			NULL
		); //to create buttons, textboxes ect takes 11 args

		/*CreateWindow(TEXT("BUTTON"), TEXT("Update selection"),
			WS_VISIBLE | WS_CHILD | WS_BORDER, //border is text box specific
			350, 10,
			115, 25,
			hWnd,
			(HMENU)ID_BUTTON,
			NULL,
			NULL
		);*/

		break;

	};

	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(0); //quit application once destroyed


	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam); //return undefiened params as well
}

TestWindow::TestWindow()
	:m_hInstance(GetModuleHandle(nullptr))
{
	const wchar_t* CLASS_NAME = L"Test Window Class"; //Wide character
	WNDCLASS wndClass = {};  //Window class
	wndClass.lpszClassName = CLASS_NAME; 
	wndClass.hInstance = m_hInstance; //our instance
	wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW); //mouse will be arrow
	wndClass.lpfnWndProc = WindowProc; //actual use of window procedure, its a function pointer

	RegisterClass(&wndClass);

	DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU; //Display close button ect.

	int width = 640;
	int height = 480;

	RECT rect;   //Dimensions of the window
	rect.left = 250; //where is appears
	rect.top = 250;
	rect.right = rect.left + width;
	rect.bottom = rect.top + height;

	AdjustWindowRect(&rect, style, false); //canvas needs to be the right size, done here rect defines those dimensions

	m_hWnd = CreateWindowEx(  //Initialize m_hwnd object
		0,
		CLASS_NAME,  //name
		L"SETUP_WINDOW", //window tytle
		style, //style must match all occurences
		rect.left,
		rect.top,
		rect.right - rect.left,
		rect.bottom - rect.top, //dimensions
		NULL,
		NULL, //For menus
		m_hInstance,
		NULL
	);

	ShowWindow(m_hWnd, SW_SHOW); //shows the window

	//Initialize the undone buttons
	B1_Done = 0, B2_Done = 0, B3_Done = 0;
}

/*TestWindow::TestWindow(bool Diagnotics)
	:m_hInstance(GetModuleHandle(nullptr))
{
	const wchar_t* CLASS_NAME = L"Test Window Class"; //Wide character
	WNDCLASS wndClass = {};  //Window class
	wndClass.lpszClassName = CLASS_NAME;
	wndClass.hInstance = m_hInstance; //our instance
	wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW); //mouse will be arrow
    
	if(!Diagnotics)
	wndClass.lpfnWndProc = WindowProc; //actual use of window procedure, its a function pointer
	else
    wndClass.lpfnWndProc = WindProcDiag; //actual use of window procedure, its a function pointer


	RegisterClass(&wndClass);

	DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU; //Display close button ect.

	int width = 640;
	int height = 480;

	RECT rect;   //Dimensions of the window
	rect.left = 250; //where is appears
	rect.top = 250;
	rect.right = rect.left + width;
	rect.bottom = rect.top + height;

	AdjustWindowRect(&rect, style, false); //canvas needs to be the right size, done here rect defines those dimensions

	m_hWnd = CreateWindowEx(  //Initialize m_hwnd object
		0,
		CLASS_NAME,  //name
		L"DIAGNOSTIC_WINDOW", //window tytle
		style, //style must match all occurences
		rect.left,
		rect.top,
		rect.right - rect.left,
		rect.bottom - rect.top, //dimensions
		NULL,
		NULL, //For menus
		m_hInstance,
		NULL
	);

	ShowWindow(m_hWnd, SW_SHOW); //shows the window

	//Initialize the undone buttons
	//B1_Done = 0, B2_Done = 0, B3_Done = 0;
}*/

TestWindow::TestWindow(bool Diagnotics)
	:m_hInstance(GetModuleHandle(nullptr))
{
	

	wndClassDiag.lpszClassName = DIAG_CLASS_NAME;
	wndClassDiag.hInstance = m_hInstance; //our instance
	wndClassDiag.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wndClassDiag.hCursor = LoadCursor(NULL, IDC_ARROW); //mouse will be arrow

	/*if (!Diagnotics)
		wndClass.lpfnWndProc = WindowProc; //actual use of window procedure, its a function pointer
	else
		wndClass.lpfnWndProc = WindProcDiag; //actual use of window procedure, its a function pointer*/

	wndClassDiag.lpfnWndProc = WindProcDiag;

	RegisterClass(&wndClassDiag);



	int width = 640;
	int height = 480;


	DiagRect.left = 250; //where is appears
	DiagRect.top = 250;
	DiagRect.right = DiagRect.left + width;
	DiagRect.bottom = DiagRect.top + height;

	AdjustWindowRect(&DiagRect, DiagStyle, false); //canvas needs to be the right size, done here rect defines those dimensions

	m_hWnd = CreateWindowEx(  //Initialize m_hwnd object
		0,
		DIAG_CLASS_NAME,  //name
		L"DIAGNOSTIC_WINDOW", //window tytle
		DiagStyle, //style must match all occurences
		DiagRect.left,
		DiagRect.top,
		DiagRect.right - DiagRect.left,
		DiagRect.bottom - DiagRect.top, //dimensions
		NULL,
		NULL, //For menus
		m_hInstance,
		NULL
	);

	//Initialize the undone buttons
	//B1_Done = 0, B2_Done = 0, B3_Done = 0;

	ShowWindow(m_hWnd, SW_SHOW); //shows the window

}

void TestWindow::DisplayDiagnostics() {


}

TestWindow::~TestWindow() 
{
	const wchar_t* CLASS_NAME = L"Test Window Class";

	UnregisterClass(CLASS_NAME, m_hInstance); //after using, unregister. The class name can be stored in the class, as a variable
	std::cout << "\nWINDOW CLOSED  ";
}

bool TestWindow::ProcessMessages() 
{
	MSG msg = {};

	while (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE)) 
	{
		if (msg.message == WM_QUIT)
		{
			return false;
		}


		TranslateMessage(&msg); //Does things like translate key presses to character.
		DispatchMessage(&msg); //calls window procedure

	}

	return true;
}

void TestWindow::UpdateDaignostcs(double Pitc_val, double Roll_val, double Yaw_val)
{

	Pitch = Pitc_val;
	std::string str1 = std::to_string(Pitch);
	size_t size = str1.size() + 1;
	const char* const_p = str1.c_str();
	wchar_t* portName = new wchar_t[size];
	size_t outSize;
	mbstowcs_s(&outSize, portName, size, const_p, size - 1);
	PitchWritten = portName;
	//std::cout << "\nWINDOW PITCH VALUE.......  " << PitchWritten;

	Roll = Roll_val;
	std::string str2 = std::to_string(Roll);
	size_t size2 = str2.size() + 1;
	const char* const_p2 = str2.c_str();
	wchar_t* portName1 = new wchar_t[size2];
	size_t outSize2;
	mbstowcs_s(&outSize2, portName1, size2, const_p2, size2 - 1);
	RollWritten = portName1;


	Yaw = Yaw_val;
	std::string str3 = std::to_string(Yaw);
	size_t size3 = str3.size() + 1;
	const char* const_p3 = str3.c_str();
	wchar_t* portName2 = new wchar_t[size2];
	size_t outSize3;
	mbstowcs_s(&outSize3, portName2, size3, const_p3, size3 - 1);
	YawWritten = portName2;


	//std::cout << "\nWINDOW PITCH VALUE.......  " << PitchWritten;
}

void TestWindow::UpdateDaignostcs(double Pitc_val, double Roll_val, double Yaw_val, bool ValidPitch, bool ValidRoll) {

	Pitch = Pitc_val;
	std::string str1 = std::to_string(Pitch);
	size_t size = str1.size() + 1;
	const char* const_p = str1.c_str();
	wchar_t* portName = new wchar_t[size];
	size_t outSize;
	mbstowcs_s(&outSize, portName, size, const_p, size - 1);
	PitchWritten = portName;
	//std::cout << "\nWINDOW PITCH VALUE.......  " << PitchWritten;

	Roll = Roll_val;
	std::string str2 = std::to_string(Roll);
	size_t size2 = str2.size() + 1;
	const char* const_p2 = str2.c_str();
	wchar_t* portName1 = new wchar_t[size2];
	size_t outSize2;
	mbstowcs_s(&outSize2, portName1, size2, const_p2, size2 - 1);
	RollWritten = portName1;


	Yaw = Yaw_val;
	std::string str3 = std::to_string(Yaw);
	size_t size3 = str3.size() + 1;
	const char* const_p3 = str3.c_str();
	wchar_t* portName2 = new wchar_t[size2];
	size_t outSize3;
	mbstowcs_s(&outSize3, portName2, size3, const_p3, size3 - 1);
	YawWritten = portName2;

	PitchValid = ValidPitch;
	RollValid = ValidRoll;

}

void  TestWindow::UpdateAccelDiag(double AccelX_val, double AccelY_val, double AccelZ_val) {



	AccelX = AccelX_val;
	std::string str1 = std::to_string(AccelX);
	size_t size = str1.size() + 1;
	const char* const_p = str1.c_str();
	wchar_t* portName = new wchar_t[size];
	size_t outSize;
	mbstowcs_s(&outSize, portName, size, const_p, size - 1);
	AccelXWritten = portName;
	//std::cout << "\nWINDOW PITCH VALUE.......  " << PitchWritten;

	AccelY = AccelY_val;
	std::string str2 = std::to_string(AccelY);
	size_t size2 = str2.size() + 1;
	const char* const_p2 = str2.c_str();
	wchar_t* portName1 = new wchar_t[size2];
	size_t outSize2;
	mbstowcs_s(&outSize2, portName1, size2, const_p2, size2 - 1);
	AccelYWritten = portName1;


	AccelZ = AccelZ_val;
	std::string str3 = std::to_string(AccelZ);
	size_t size3 = str3.size() + 1;
	const char* const_p3 = str3.c_str();
	wchar_t* portName2 = new wchar_t[size2];
	size_t outSize3;
	mbstowcs_s(&outSize3, portName2, size3, const_p3, size3 - 1);
	AccelZWritten = portName2;


}

void TestWindow::UpdateRadarDaignostcs(int Val, int Pos)
{
	int rVal;
	rVal = Val;
	std::string str1 = std::to_string(rVal);
	size_t size = str1.size() + 1;
	const char* const_p = str1.c_str();
	wchar_t* portName = new wchar_t[size];
	size_t outSize;
	mbstowcs_s(&outSize, portName, size, const_p, size - 1);
	RadarPos = portName;
	//std::cout << "\nWINDOW PITCH VALUE.......  " << PitchWritten;

	int rPos;
	rPos = Pos;
	std::string str2 = std::to_string(rPos);
	size_t size2 = str2.size() + 1;
	const char* const_p2 = str2.c_str();
	wchar_t* portName1 = new wchar_t[size2];
	size_t outSize2;
	mbstowcs_s(&outSize2, portName1, size2, const_p2, size2 - 1);
	RadarVal = portName1;




}

void TestWindow::UpdateMotorSteering(int Steer, int Throttle)
{
	int rSteer;
	rSteer = Steer;
	std::string str1 = std::to_string(rSteer);
	size_t size = str1.size() + 1;
	const char* const_p = str1.c_str();
	wchar_t* portName = new wchar_t[size];
	size_t outSize;
	mbstowcs_s(&outSize, portName, size, const_p, size - 1);
	SteerPos = portName;
	//std::cout << "\nWINDOW PITCH VALUE.......  " << PitchWritten;

	int rThrottle;
	rThrottle = Throttle;
	std::string str2 = std::to_string(rThrottle);
	size_t size2 = str2.size() + 1;
	const char* const_p2 = str2.c_str();
	wchar_t* portName1 = new wchar_t[size2];
	size_t outSize2;
	mbstowcs_s(&outSize2, portName1, size2, const_p2, size2 - 1);
	ThrottlePos = portName1;




}

void TestWindow::DrawDiagnostic(HWND hWnd) {

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

	RECT RadarValText;
	RadarValText.left = 30,
		RadarValText.top = 100,
		RadarValText.right = 100,
		RadarValText.bottom = 25
		;

	RECT RadarPosText;
	RadarPosText.left = 30,
		RadarPosText.top = 140,
		RadarPosText.right = 100,
		RadarPosText.bottom = 25
		;

	RECT SteerText;
	SteerText.left = 30,
		SteerText.top = 180,
		SteerText.right = 100,
		SteerText.bottom = 25
		;

	RECT ThrottleText;
	ThrottleText.left = 30,
		ThrottleText.top = 220,
		ThrottleText.right = 100,
		ThrottleText.bottom = 25
		;

	RECT AccelXText;
	AccelXText.left = 265,
		AccelXText.top = 10,
		AccelXText.right = 100,
		AccelXText.bottom = 25
		;

	RECT AccelYText;
	AccelYText.left = 265,
		AccelYText.top = 40,
		AccelYText.right = 100,
		AccelYText.bottom = 25
		;

	RECT AccelZText;
	AccelZText.left = 265,
		AccelZText.top = 70,
		AccelZText.right = 100,
		AccelZText.bottom = 25
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

	RECT rect3;
	rect3.left = RadarValText.left + 210; //where is appears
	rect3.top = RadarValText.top + 100;
	rect3.right = RadarValText.right;
	rect3.bottom = RadarValText.bottom;

	RECT rect4;
	rect4.left = RadarPosText.left + 210; //where is appears
	rect4.top = RadarPosText.top + 130;
	rect4.right = RadarPosText.right;
	rect4.bottom = RadarPosText.bottom;

	RECT rect5;
	rect5.left = SteerText.left + 210; //where is appears
	rect5.top = SteerText.top + 155;
	rect5.right = SteerText.right;
	rect5.bottom = SteerText.bottom;

	RECT rect6;
	rect6.left = ThrottleText.left + 210; //where is appears
	rect6.top = ThrottleText.top + 195;
	rect6.right = ThrottleText.right ;
	rect6.bottom = ThrottleText.bottom;

	RECT rect7;
	rect7.left = AccelXText.left + 430; //where is appears
	rect7.top = AccelXText.top;
	rect7.right = AccelXText.right;
	rect7.bottom = AccelXText.bottom;

	RECT rect8;
	rect8.left = AccelYText.left + 430; //where is appears
	rect8.top = AccelYText.top + 35;
	rect8.right = AccelYText.right;
	rect8.bottom = AccelYText.bottom;

	RECT rect9;
	rect9.left = AccelZText.left + 430; //where is appears
	rect9.top = AccelZText.top + 60;
	rect9.right = AccelZText.right;
	rect9.bottom = AccelZText.bottom;

	RECT rect10;
	rect10.left = PitchText.left + 285; //where is appears
	rect10.top = PitchText.top;
	rect10.right = PitchText.right;
	rect10.bottom = PitchText.bottom;

	RECT rect11;
	rect11.left = RollText.left + 285; //where is appears
	rect11.top = RollText.top + 35;
	rect11.right = RollText.right;
	rect11.bottom = RollText.bottom;

	HDC dc = GetDC(hWnd);
	RECT rc;
	GetClientRect(hWnd, &rc);

	DrawText(dc, PitchWritten, -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	DrawText(dc, RollWritten, -1, &rect1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	DrawText(dc, YawWritten, -1, &rect2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	DrawText(dc, RadarPos, -1, &rect3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	DrawText(dc, RadarVal, -1, &rect4, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	DrawText(dc, SteerPos, -1, &rect5, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	DrawText(dc, ThrottlePos, -1, &rect6, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	DrawText(dc, AccelXWritten, -1, &rect7, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	DrawText(dc, AccelYWritten, -1, &rect8, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	DrawText(dc, AccelZWritten, -1, &rect9, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	if(PitchValid)
		DrawText(dc, L"P-Y", -1, &rect10, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	else
		DrawText(dc, L"P-N", -1, &rect10, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	if (RollValid)
		DrawText(dc, L"R-Y", -1, &rect11, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	else
		DrawText(dc, L"R-N", -1, &rect11, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	ReleaseDC(hWnd, dc);

};

void TestWindow::Assignments() 
{
	board1 = Board1;
	board2 = Board2;
	board3 = Board3;

}