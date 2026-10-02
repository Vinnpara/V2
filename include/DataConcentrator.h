#ifndef DATA_CONCENTRATOR_H
#define DATA_CONCENTRATOR_H

#include <Windows.h>
#include <string>
#include <algorithm>

#include <string>
#include <map>
#include <vector>

#include <iostream>

enum DataOptions
{
	MESSAGE_ON = 1,
	MESSAGE_OFF = 2,
};
typedef enum DataOptions DataOptions;

class DataConcentrator
{
public:
	DataConcentrator();
	void AddMessage(std::string Message, int MessageCode);
	void SetMessageStatus(std::string Message, int MessageCode, DataOptions Option);
	void RemoveMessage(int MessageCode);
	void SwapMessage(std::string MessageToAdd, int MessageCode, int MessageCodeToswap);
	bool CheckMessageExistence(int MessageCode);


	void PreparePrintMessage();
	std::vector <std::string> GetPrintMessage();


	int GetSizeOfMap() { return MapSize; }
	void SetMapSize();
	void CheckMapIncrement(static int &Previous);
	void ShowIncrement();
	void ReprintMessage(static bool & Print, static bool &MessageAdded);

private:
	std::map <int, std::string> MessagesToPrint;   // map<keytype, valuetype> mapName
	std::vector <std::string> MessageFeed;
	int MapSize,
		PreviousSize;
	bool MessageChanged,
		 MessageAddorRemove;
};

#endif
