#ifndef GUI_FUNCTION_H
#define GUI_FUNCTION_H

#define BUTTON_STATE_STATIC "STATIC"
#define BUTTON_STATE_EDIT "EDIT"
#define BUTTON_STATE_BUTTON "BUTTON"

#define MAX_CHAR_SIZE 64

#include <Windows.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

/*
* Generic functions common to the plot, setup and diagnostic windows go here. Base class, with its functions accessible 
* to all.
*/

enum MenuOptions
{
	M_POPUP = 1,
	M_STRING = 2,
};

typedef enum MenuOptions MenuOptions;

static struct ButtonOptions
{
	int GenericButton[20];
	bool GenericButtonState[10];
	int GenericCounter[10];

};

class GUIFunction 
{
public:
	/*Constructors, inits */
	GUIFunction();
	GUIFunction(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void SetParams(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void SetMenuCords(int x, int y, int h, int w);

	/*Create Window funcs*/
	void CreateWindowFunction(std::string ButtonType, std::string ButtonBody);
	void CreateWindowFunction(std::string ButtonType, std::string ButtonBody, int Coords[6]);
	void CreateWindowFunction(std::string ButtonType, std::string ButtonBody, int xAug, int yAug, int HAug, int WAug);

	HWND CreateWindowFunctionHWND(std::string ButtonType, std::string ButtonBody, int xAug, int yAug, int HAug, int WAug);

	void CreateWindowFunctionButton(std::string ButtonType, std::string ButtonBody, static int ButtonPNT);
	void CreateWindowFunctionButton(std::string ButtonType, std::string ButtonBody, int xAug, int yAug, int HAug, int WAug, static int ButtonPNT);

	/*Create Menus*/
	void AppMenu(HMENU hPort, std::string Text, MenuOptions MenuType, int ButtonState);

	/*Get text from windows*/

	void GetText(HWND hWND, wchar_t * txt);

	int GetCordX() { return MenuCord[0]; }
	int GetCordY() { return MenuCord[1]; }
	int GetCordH() { return MenuCord[2]; }
	int GetCordW() { return MenuCord[3]; }

protected:

	static ButtonOptions Buttons;

	std::string Dot = "...";

	int MenuCord[10];
	HWND hWnd; 
	UINT uMsg; 
	WPARAM wParam; 
	LPARAM lParam;
};

#endif