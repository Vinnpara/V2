#include <Windows.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <GUIFunction.h>
#include <AutoMenuGUIFunction.h>


/*
* See Header file for the deets
*/

StaticVariablesA AutoMenuGUIFunction::StaticMenuVar;

void AutoMenuGUIFunction::InitializeButtons()
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
AutoMenuGUIFunction :: AutoMenuGUIFunction()
{
	InitializeButtons();
};


void AutoMenuGUIFunction::WMCreateWindowFunction() {

	CreateWindowFunction(BUTTON_STATE_STATIC, SetMenuTexts[0]);

	for (size_t i = 1; i < 5; i++)
	{
		if (i < 4)
			CreateWindowFunction(BUTTON_STATE_STATIC, SetMenuTexts[i], 0, 30 * i, 0, 0);
		else
			CreateWindowFunctionButton(BUTTON_STATE_BUTTON, SetMenuTexts[i], 0, 30 * i, 0, 0, StaticMenuVar.ButtonSelection[1]); //confirm sel LOWORD(wParam) == 1

	}

}