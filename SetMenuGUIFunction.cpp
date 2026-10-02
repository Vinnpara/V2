#include <Windows.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <GUIFunction.h>
#include <SetMenuGUIFunction.h>


/*
* See Header file for the deets
*/

StaticVariables SetMenuGUIFunction::StaticMenuVar;


void SetMenuGUIFunction::InitializeButtons()
{
	for (size_t i = 1; i < 15; i++)
	{
		
	    StaticMenuVar.IDButtons[i] = i;
		
	}

	for (size_t i = 1; i < 10; i++)
	{

		StaticMenuVar.ButtonSelection[i] = i;

	}

}

void SetMenuGUIFunction::InitializeFile()
{
	BoardFiles.SetName(FileName);
	BoardFiles.OpenFile();
}

void SetMenuGUIFunction::RecordFileData()
{
	//AssingedBoards[0] != NULL;

	for (size_t i = 0; i < AssingedBoards.size(); i++)
	{
		//std::cout << "\n" << AssingedBoards[i];
		//BoardFiles.RecordDataFloat((float)AssingedBoards[i]);
		BoardFiles.RecordDataFloat(2.0f,2.0f);
	}
	
}

void SetMenuGUIFunction::CloseFile()
{
	BoardFiles.CloseFile();
}

SetMenuGUIFunction::SetMenuGUIFunction()
{
	InitializeButtons();
}

void SetMenuGUIFunction::CreateMenuBar() {

	HMENU hMenuBar = CreateMenu();
	HMENU hFile = CreateMenu();
	HMENU hPorts = CreateMenu();

	AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hFile, TEXT("File"));
	AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hPorts, TEXT("Ports"));
	AppendMenu(hMenuBar, MF_POPUP, NULL, TEXT("Edit"));
	AppendMenu(hMenuBar, MF_STRING, NULL, TEXT("Exit"));

	for (size_t i = 1; i < 10; i++) 
	{
		AppMenu(hPorts, SetMenuCOMMOptions[i], M_STRING, StaticMenuVar.ButtonSelection[i]);
	}

	SetMenu(hWnd, hMenuBar);
}

void SetMenuGUIFunction::WMCreateWindowFunction() {

	CreateWindowFunction(BUTTON_STATE_STATIC, SetMenuTexts[0]);

	//CreateWindowFunctionButton(BUTTON_STATE_STATIC, SetMenuTexts[0], StaticMenuVar.ButtonSelection[1]);

	for (size_t i = 1; i < 3; i++)
	{
		if (i < 2)
			CreateWindowFunction(BUTTON_STATE_STATIC, SetMenuTexts[i], 90 * i, 0, 0, 0);
		else
			CreateWindowFunctionButton(BUTTON_STATE_BUTTON, SetMenuTexts[i], 90 * i, 0, 0, 0, StaticMenuVar.ButtonSelection[1]);

	}

	//Menu.CreateWindowFunction(BUTTON_STATE_STATIC, SetMenuTexts[3], BoardCords);

	for (size_t i = 0; i < 3; i++)
	{
		if (i == 0)
			CreateWindowFunction(BUTTON_STATE_STATIC, SetMenuTexts[i + 3], 0, 30, 0, 20);
		else
			CreateWindowFunction(BUTTON_STATE_STATIC, SetMenuTexts[i + 3], 0, 60 * i, 0, 20);

	}

	CreateMenuBar();
}



void SetMenuGUIFunction::SaveAssignedBoards()
{
	BoardFiles.SetName(FileName);
	BoardFiles.OpenFile();

	for (size_t i = 0; i < 3; i++)
	{
		//BoardFiles.RecordDataInt(StaticMenuVar.BoardPort[i]);
		std::cout << "\nBoards" << StaticMenuVar.BoardPort[i];
	}

	BoardFiles.CloseFile();
}

void SetMenuGUIFunction::WMCommandWindowFunction()
{
	if (!StaticMenuVar.BoardSelected[0] && !StaticMenuVar.BoardSelected[1] && !StaticMenuVar.BoardSelected[2])
	{
		for (size_t i = 1; i < 15; i++)
		{
			if (LOWORD(wParam) == StaticMenuVar.IDButtons[i] && i != 0)
			{
				int CordsUsed[6] = { 110,50,100,25 };
				std::string st1 = "STATIC";
				CreateWindowFunction(st1, SetMenuCOMMOptions[i], CordsUsed);

				StaticMenuVar.BoardSelected[0] = 1;
				StaticMenuVar.IDSelected[i] = 1;
				StaticMenuVar.BoardPort[0] = i;
				std::cout << "\nHere 1" << StaticMenuVar.BoardPort[0];
				AssingedBoards.push_back(StaticMenuVar.BoardPort[0]);
			}


		}
	}

	if (StaticMenuVar.BoardSelected[0] && !StaticMenuVar.BoardSelected[1] && !StaticMenuVar.BoardSelected[2])
	{
		for (size_t i = 1; i < 15; i++)
		{
			if (LOWORD(wParam) == StaticMenuVar.IDButtons[i] && i != 0 && !StaticMenuVar.IDSelected[i])
			{
				int CordsUsed[6] = { 110,90,100,25 };
				std::string st1 = "STATIC";
				CreateWindowFunction(st1, SetMenuCOMMOptions[i], CordsUsed);

				StaticMenuVar.BoardSelected[1] = 1;
				StaticMenuVar.IDSelected[i] = 1;
				StaticMenuVar.BoardPort[1] = i;

				std::cout << "\nHere 2" << StaticMenuVar.BoardPort[1];
				AssingedBoards.push_back(StaticMenuVar.BoardPort[1]);

			}


		}
	}

	if (StaticMenuVar.BoardSelected[0] && StaticMenuVar.BoardSelected[1] && !StaticMenuVar.BoardSelected[2])
	{
		for (size_t i = 1; i < 15; i++)
		{
			if (LOWORD(wParam) == StaticMenuVar.IDButtons[i] && i != 0 && !StaticMenuVar.IDSelected[i])
			{
				int CordsUsed[6] = { 110,130,100,25 };
				std::string st1 = "STATIC";
				CreateWindowFunction(st1, SetMenuCOMMOptions[i], CordsUsed);

				StaticMenuVar.BoardSelected[2] = 1;
				StaticMenuVar.IDSelected[i] = 1;
				StaticMenuVar.BoardPort[2] = i;

				std::cout << "\nHere 3" << StaticMenuVar.BoardPort[2];
				AssingedBoards.push_back(StaticMenuVar.BoardPort[2]);

			}


		}
	}

	if (StaticMenuVar.BoardSelected[0] && StaticMenuVar.BoardSelected[1] && StaticMenuVar.BoardSelected[2])
	{
		//std::cout << "\nBOARDS ASSINGED";

		if (LOWORD(wParam) == StaticMenuVar.ButtonSelection[1])
		{
			SaveAssignedBoards();
		}

	}

}