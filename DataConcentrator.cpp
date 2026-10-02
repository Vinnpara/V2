#include<DataConcentrator.h>

#include <Windows.h>
#include <string>
#include <algorithm>
#include <stdlib.h>
#include <math.h>

#include <string>
#include <map>

#include <iostream>

DataConcentrator::DataConcentrator(){}

void DataConcentrator::AddMessage(std::string Message, int MessageCode)
{
	
	/*if (!MessagesToPrint.count(MessageCode));
	{
		MessagesToPrint.insert({ MessageCode,Message });
		MessageAddorRemove = 1;
		std::cout << "\nAdding message "<<" "<< Message<< " "<< MessageCode;
	}*/

	auto it = MessagesToPrint.find(MessageCode);

	if (it == MessagesToPrint.end())
	{
		MessagesToPrint.insert({ MessageCode,Message });
		MessageAddorRemove = 1;
		//std::cout << "\nAdding message " << " " << Message << " " << MessageCode;
	}

}

void DataConcentrator::RemoveMessage(int MessageCode)
{
	/*if (MessagesToPrint.count(MessageCode));
	{
		MessagesToPrint.erase(MessageCode);
		MessageAddorRemove = 1;
		std::cout << "\nRemoving message "<<" "<< MessageCode;
	}*/

	auto it = MessagesToPrint.find(MessageCode);

	if (it != MessagesToPrint.end())
	{
		MessagesToPrint.erase(MessageCode);
		MessageAddorRemove = 1;
		//std::cout << "\nRemoving message " << " " << MessageCode;
	}
}

bool DataConcentrator::CheckMessageExistence(int MessageCode)
{
	bool MessageExists = 0;

	/*auto it = MessagesToPrint.find(MessageCode);

	if (it != MessagesToPrint.end())
	{
		MessageExists = 1;
	}
	else
		MessageExists = 0;*/

	MessageExists = MessagesToPrint.count(MessageCode);

	return MessageExists;
}

void DataConcentrator::SwapMessage(std::string MessageToAdd, int MessageCode, int MessageCodeToswap)
{
	if (CheckMessageExistence(MessageCodeToswap))
	{
		RemoveMessage(MessageCodeToswap);
		AddMessage(MessageToAdd, MessageCode);
		MessageAddorRemove = 1;
	}
}

void DataConcentrator::SetMessageStatus(std::string Message, int MessageCode, DataOptions Option)
{
	if (Option == MESSAGE_ON)
	{
		AddMessage(Message, MessageCode);

	}
	if (Option == MESSAGE_OFF)
	{
		RemoveMessage(MessageCode);

	}

}

void DataConcentrator::PreparePrintMessage()
{
	if (MessageFeed.size() == 0 || MessageChanged || MessageAddorRemove)
	{
		if(!MessageFeed.size() == 0)
			MessageFeed.clear();
		
		for (const auto& X : MessagesToPrint)
		{
			/*std::string message = X.first;
			int code = X.second;*/

			int code = X.first;
			std::string message = X.second;

			std::string Code = std::to_string(code);
			std::string MessageTotal = message + ", " + Code;

			MessageFeed.push_back(MessageTotal);
		}
	}

	
	
}

void DataConcentrator::CheckMapIncrement(static int &Previous)
{
	if (std::abs(int(MessagesToPrint.size() - Previous)) > 0)
	{
		//std::cout << "\nIncrement of map detected ";
		//std::cout << "\nMap size " << MessagesToPrint.size() <<" "<< Previous;
		MessageChanged = 1;
		Previous = MessagesToPrint.size();
		//std::cout << "\nMap size 2 " << MessagesToPrint.size() << " " << Previous;
	}
	else
		MessageChanged = 0;
}

void DataConcentrator::ReprintMessage(static bool& Print, static bool& MessageAdded)
{
	MessageAdded = MessageAddorRemove;
	
	if (MessageChanged)
	{
		Print = 0;
		//std::cout << "\nMESSAGE RE PRINT ";
	}

	if (MessageAdded)
	{
		Print = 0;
		MessageAdded = 0;
		MessageAddorRemove = 0;
		//std::cout << "\nMESSAGE Swapped ";
	}
}

void DataConcentrator::ShowIncrement()
{
	//if(MessageChanged)
		//std::cout << "\nIncrement of map detected "<< MessagesToPrint.size();
	//else

}

void DataConcentrator::SetMapSize()
{
	PreviousSize = MessagesToPrint.size();


}

std::vector <std::string> DataConcentrator::GetPrintMessage()
{
	return MessageFeed;
}