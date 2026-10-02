#ifndef SETMENU_GUI_FUNCTION_H
#define SETMENU_GUI_FUNCTION_H

#define MAX_CHAR_SIZE 50



#include <Windows.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <GUIFunction.h>
#include <FileEditor.h>

/*
* This module handles the Board and port assingment section.
*/

static struct StaticVariables
{
     int IDButtons[25],
        BoardSelected[5],
        IDSelected[25],
        BoardPort[5],
        ButtonSelection[10];

};

class SetMenuGUIFunction : public GUIFunction
{
public:
    SetMenuGUIFunction();

    void InitializeButtons();
    void InitializeFile();
    void RecordFileData();
    void CloseFile();
    void SaveAssignedBoards();

    void CreateMenuBar();
    void WMCreateWindowFunction();
    void WMCommandWindowFunction();


    std::string GetSetMenuTexts(size_t i) { return SetMenuTexts[i];}
    std::string GetSetMenuCOMMOptions(size_t i) { return SetMenuCOMMOptions[i];}
    std::string GetSetMenuFileOptions(size_t i) { return SetMenuFileOptions[i];}
    std::vector <int> GetAssingedBoards() { return AssingedBoards;}

private:
    std::string FileName = "Boards.txt";
    static StaticVariables StaticMenuVar;
    std::vector <int> AssingedBoards;
    FileEditor BoardFiles;

    std::vector <std::string> SetMenuTexts =
    {
        "Arduino",
        "Port",
        "Conf. slection",
        "Motor Steering Board",
        "Radar Board",
        "Gyro Board",
    };


    std::vector <std::string> SetMenuCOMMOptions =
    {
        "NULL",
        "COM 1",
        "COM 2",
        "COM 3",
        "COM 4",
        "COM 5",
        "COM 6",
        "COM 7",
        "COM 8",
        "COM 9",
        "COM 10",
        "COM 11",
        "COM 12",
        "COM 13",
        "COM 14",
        "NULL",
        "NULL",
        "NULL"
    };

    std::vector <std::string> SetMenuFileOptions =
    {
        "File",
        "Ports",
        "Edit",
        "Exit"
    };

};

#endif