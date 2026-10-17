#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include <Windows.h>
#include <SerialPortSelection.h>
#include <vector>
#include <map>
#include <string>

#include <ColorSelection.h>
#include <DataConcentrator.h>

#include <chrono>

#include <algorithm>
#include <richedit.h>
#include <codecvt>
#include <locale>

#include<SerialConnectionSpeed.h>

std::vector <std::string> SortMessages(std::vector <std::string> Messages);
int GetColorRequested(std::string Message);
std::string ProcessMessage(std::string Message);
std::vector<int> ColorSelector(ColorSelection Color);
LPWSTR ConvertToLPWSTR(std::string Data);


void ShowData(DataConcentrator &Dc, HWND &Hndl, static bool& MessagePrinted);
void TestMessage(std::string Mssg, HWND Hndl);

#endif