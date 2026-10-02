#ifndef AUTOMENU_GUI_FUNCTION_H
#define AUTOMENU_GUI_FUNCTION_H

#endif

#include <Windows.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <GUIFunction.h>
#include <FileEditor.h>

static struct StaticVariablesA
{
    int IDButtons[25],
        BoardSelected[5],
        IDSelected[25],
        BoardPort[5],
        ButtonSelection[10];

};

class AutoMenuGUIFunction : public GUIFunction
{
public:
    AutoMenuGUIFunction();

    void InitializeButtons();

    void CreateMenuBar();
    void WMCreateWindowFunction();
    void WMCommandWindowFunction();

private:
    static StaticVariablesA StaticMenuVar;

    std::vector <std::string> SetMenuTexts =
    {
        "Arduino boards",
        "Board 1",
        "Board 2",
        "Board 3",
        "Conf. slection"
    };

};