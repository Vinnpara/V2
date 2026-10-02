#include <Windows.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <GUIFunction.h>

/*
* See Header file for the deets
*/

ButtonOptions GUIFunction::Buttons;

GUIFunction::GUIFunction() 
{

}

GUIFunction::GUIFunction(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
	this->hWnd = hWnd;
	this->uMsg = uMsg;
	this->wParam = wParam;
	this->lParam = lParam;

}

void GUIFunction::SetParams(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	
	this->hWnd = hWnd;
	this->uMsg = uMsg;
	this->wParam = wParam;
	this->lParam = lParam;
}

void GUIFunction::SetMenuCords(int x, int y, int h, int w)
{
	MenuCord[0] = x;
	MenuCord[1] = y;
	MenuCord[2] = h;
	MenuCord[3] = w;
}

void GUIFunction::CreateWindowFunction(std::string ButtonType, std::string ButtonBody)
{
	LPCWSTR strtemp1, strtemp2;

	std::wstring stemp = std::wstring(ButtonType.begin(), ButtonType.end());
	strtemp1 = stemp.c_str();
	std::wstring stemp2 = std::wstring(ButtonBody.begin(), ButtonBody.end());
	strtemp2 = stemp2.c_str();

	CreateWindow(strtemp1, strtemp2,
		WS_VISIBLE | WS_CHILD,
		MenuCord[0], MenuCord[1],
		MenuCord[2], MenuCord[3],
		hWnd,
		(HMENU)NULL,
		NULL,
		NULL
	);
}

void GUIFunction::CreateWindowFunction(std::string ButtonType, std::string ButtonBody, int Coords[6])
{
	LPCWSTR strtemp1, strtemp2;

	std::wstring stemp = std::wstring(ButtonType.begin(), ButtonType.end());
	strtemp1 = stemp.c_str();
	std::wstring stemp2 = std::wstring(ButtonBody.begin(), ButtonBody.end());
	strtemp2 = stemp2.c_str();

	CreateWindow(strtemp1, strtemp2,
		WS_VISIBLE | WS_CHILD,
		Coords[0], Coords[1],
		Coords[2], Coords[3],
		hWnd,
		(HMENU)NULL,
		NULL,
		NULL
	);
}

void GUIFunction::CreateWindowFunction(std::string ButtonType, std::string ButtonBody, int xAug, int yAug, int WAug, int HAug)
{
	LPCWSTR strtemp1, strtemp2;

	std::wstring stemp = std::wstring(ButtonType.begin(), ButtonType.end());
	strtemp1 = stemp.c_str();
	std::wstring stemp2 = std::wstring(ButtonBody.begin(), ButtonBody.end());
	strtemp2 = stemp2.c_str();

	CreateWindow(strtemp1, strtemp2,
		WS_VISIBLE | WS_CHILD,
		MenuCord[0] + xAug, MenuCord[1] + yAug,
		MenuCord[2] + WAug, MenuCord[3] + HAug,
		hWnd,
		(HMENU)NULL,
		NULL,
		NULL
	);
}

HWND GUIFunction::CreateWindowFunctionHWND(std::string ButtonType, std::string ButtonBody, int xAug, int yAug, int HAug, int WAug)
{
	HWND Hwnd;

	LPCWSTR strtemp1, strtemp2;

	std::wstring stemp = std::wstring(ButtonType.begin(), ButtonType.end());
	strtemp1 = stemp.c_str();
	std::wstring stemp2 = std::wstring(ButtonBody.begin(), ButtonBody.end());
	strtemp2 = stemp2.c_str();

	Hwnd = CreateWindow(strtemp1, strtemp2,
		WS_VISIBLE | WS_CHILD | WS_BORDER,
		MenuCord[0] + xAug, MenuCord[1] + yAug,
		MenuCord[2] + WAug, MenuCord[3] + HAug,
		hWnd,
		(HMENU)NULL,
		NULL,
		NULL
	);

	return Hwnd;
}

void GUIFunction::CreateWindowFunctionButton(std::string ButtonType, std::string ButtonBody, static int ButtonPNT)
{
	LPCWSTR strtemp1, strtemp2;

	std::wstring stemp = std::wstring(ButtonType.begin(), ButtonType.end());
	strtemp1 = stemp.c_str();
	std::wstring stemp2 = std::wstring(ButtonBody.begin(), ButtonBody.end());
	strtemp2 = stemp2.c_str();

	CreateWindow(strtemp1, strtemp2,
		WS_VISIBLE | WS_CHILD,
		MenuCord[0], MenuCord[1],
		MenuCord[2], MenuCord[3],
		hWnd,
		(HMENU)ButtonPNT,
		NULL,
		NULL
	);
}

void GUIFunction::CreateWindowFunctionButton(std::string ButtonType, std::string ButtonBody, int xAug, int yAug, int HAug, int WAug, static int ButtonPNT)
{
	LPCWSTR strtemp1, strtemp2;

	std::wstring stemp = std::wstring(ButtonType.begin(), ButtonType.end());
	strtemp1 = stemp.c_str();
	std::wstring stemp2 = std::wstring(ButtonBody.begin(), ButtonBody.end());
	strtemp2 = stemp2.c_str();

	CreateWindow(strtemp1, strtemp2,
		WS_VISIBLE | WS_CHILD | WS_BORDER,
		MenuCord[0] + xAug, MenuCord[1] + yAug,
		MenuCord[2] + WAug, MenuCord[3] + HAug,
		hWnd,
		(HMENU)ButtonPNT,
		NULL,
		NULL
	);
}

void GUIFunction::GetText(HWND hWND, wchar_t* txt) 
{
	GetWindowTextW(hWND, txt, MAX_CHAR_SIZE);
}

void GUIFunction::AppMenu(HMENU hPort, std::string Text, MenuOptions MenuType, int ButtonState)
{

	LPCWSTR strtemp1;

	std::wstring stemp = std::wstring(Text.begin(), Text.end());
	strtemp1 = stemp.c_str();

	if (MenuType == M_POPUP)
		AppendMenu(hPort, MF_POPUP, ButtonState, strtemp1);
	else if (MenuType == M_STRING)
		AppendMenu(hPort, MF_STRING, ButtonState, strtemp1);


}

