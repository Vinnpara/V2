#include <MessageHandler.h>

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

std::vector <std::string> SortMessages(std::vector <std::string> Messages)
{
	std::vector <std::string> RedMessages,
		AmberMessages,
		CyanMessages;

	std::vector <std::string> OutputMessages;

	if (!Messages.empty())
	{
		for (int i = 0; i < Messages.size(); i++)
		{
			ColorSelection ColorReq = (ColorSelection)GetColorRequested(Messages[i]);

			if (ColorReq == RED)
			{
				RedMessages.push_back(Messages[i]);
			}
			if (ColorReq == AMBER)
			{
				AmberMessages.push_back(Messages[i]);
			}
			if (ColorReq == CYAN)
			{
				CyanMessages.push_back(Messages[i]);
			}

		}
	}

	for (int i = 0; i < RedMessages.size(); i++)
	{
		OutputMessages.push_back(RedMessages[i]);
	}

	for (int i = 0; i < AmberMessages.size(); i++)
	{
		OutputMessages.push_back(AmberMessages[i]);
	}

	for (int i = 0; i < CyanMessages.size(); i++)
	{
		OutputMessages.push_back(CyanMessages[i]);
	}

	return OutputMessages;
}

int GetColorRequested(std::string Message)
{
	std::string ColorReq;

	for (int j = 0; j < Message.size(); j++)
	{
		if (Message.at(j) == ':')
		{
			//std::cout << "\nMessage at j " << Messages[i].at(j)<<" "<<j;
			break;
		}

		ColorReq.push_back(Message.at(j));
		//std::cout << "\nThe requested color " << ColorRequested[j];
	}

	int ColorRequestInt = std::stoi(ColorReq);

	return ColorRequestInt;
}

std::string ProcessMessage(std::string Message)
{
	std::string str = Message;

	int End = 0;
	for (int j = 0; j < str.size(); j++)
	{

		if (str.at(j) == ':')
		{
			End = j;
			break;
		}


	}

	str.erase(0, (End + 1));

	return str;
}

std::vector<int> ColorSelector(ColorSelection Color)
{
	switch (Color)
	{
	case RED:
	{
		std::vector<int> Colors{ 255, 0, 0 };
		return Colors;
		break;
	};

	case AMBER:
	{
		std::vector<int> Colors{ 255, 150, 75 };
		return Colors;
		break;
	};

	case CYAN:
	{
		std::vector<int> Colors{ 0, 160, 255 };
		return Colors;
		break;
	};


	}
}

LPWSTR ConvertToLPWSTR(std::string Data)
{
	size_t size = Data.size() + 1;
	const char* const_p = Data.c_str();
	wchar_t* portName = new wchar_t[size];
	size_t outSize;
	mbstowcs_s(&outSize, portName, size, const_p, size - 1);

	return portName;
}

void ShowData(DataConcentrator& Dc, HWND &Hndl, static bool& MessagePrinted)
{
	if (!Hndl) return;

	std::vector <std::string>Messages = SortMessages(Dc.GetPrintMessage());

	if (!MessagePrinted)
	{
		std::cout << "\nVector size " << Messages.size();

		SendMessage(Hndl, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(L""));

		for (int i = 0; i < Messages.size(); i++)
		{
			int TextLength = GetWindowTextLength(Hndl);
			SendMessage(Hndl, EM_SETSEL, TextLength, TextLength);

			ColorSelection ColorReq = (ColorSelection)GetColorRequested(Messages[i]);
			std::string Message = ProcessMessage(Messages[i]);

			std::vector<int> ColorToUse = ColorSelector(ColorReq);

			//SetTextColor(dc, RGB(255, 0, 0));

			COLORREF textColor = RGB(ColorToUse.at(0), ColorToUse.at(1), ColorToUse.at(2));

			//COLORREF textColor = RGB(255, 0, 0);

			CHARFORMAT2 cf = {};
			cf.cbSize = sizeof(CHARFORMAT2);
			cf.dwMask = CFM_COLOR;              // We only want to modify the color attribute
			cf.crTextColor = textColor;

			SendMessage(Hndl, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);

			wchar_t* message = ConvertToLPWSTR(Message);

			SendMessage(Hndl, EM_REPLACESEL, FALSE, (LPARAM)message);
			SendMessage(Hndl, EM_REPLACESEL, FALSE, (LPARAM)L"\r\n");
			SendMessage(Hndl, EM_SCROLLCARET, 0, 0);

			UpdateWindow(Hndl);

			//std::cout << "\nPrint func " << Messages.size() <<" Message "<< Messages[i];

			if (i == (Messages.size() - 1))
				MessagePrinted = 1;
		}

	}
}

void TestMessage(std::string Mssg, HWND Hndl)
{
	int TextLength = GetWindowTextLength(Hndl);
	SendMessage(Hndl, EM_SETSEL, TextLength, TextLength);

	//ColorSelection ColorReq = (ColorSelection)GetColorRequested(Mssg);
	//std::string Message = ProcessMessage(Mssg);

	//std::vector<int> ColorToUse = ColorSelector(ColorReq);

	//SetTextColor(dc, RGB(255, 0, 0));

	COLORREF textColor = RGB(102, 55, 65);

	//COLORREF textColor = RGB(255, 0, 0);

	CHARFORMAT2 cf = {};
	cf.cbSize = sizeof(CHARFORMAT2);
	cf.dwMask = CFM_COLOR;              // We only want to modify the color attribute
	cf.crTextColor = textColor;

	SendMessage(Hndl, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);

	wchar_t* message = ConvertToLPWSTR(Mssg);

	SendMessage(Hndl, EM_REPLACESEL, FALSE, (LPARAM)message);
	SendMessage(Hndl, EM_REPLACESEL, FALSE, (LPARAM)L"\r\n");
	SendMessage(Hndl, EM_SCROLLCARET, 0, 0);
}