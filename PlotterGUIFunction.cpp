#include<PlotterGUIFunction.h>

GraphPlotter PlotterGUIFunction::PlotBulk;

std::vector<std::string> PlotterGUIFunction::PlotSourceFilesNames;
std::vector<std::string> PlotterGUIFunction::PlotResultFilesNames;
std::vector<HWND> PlotterGUIFunction::PlotSourceNamesIn;
std::vector<HWND> PlotterGUIFunction::PlotResultNamesIn;

void PlotterGUIFunction::InitializeLocalButtons() {

	for (size_t i = 0; i < 20; i++) 
	{
		Buttons.GenericButton[i] = i + 1;
	}
}

PlotterGUIFunction::PlotterGUIFunction() 
{
	InitializeLocalButtons();
}

std::string PlotterGUIFunction::StringFromWchar(wchar_t* text)
{
	std::string st1;

	std::wstring ws(text);
	std::string str(ws.begin(), ws.end());

	return str;
}

void PlotterGUIFunction::CreatePlotButtons()
{
	for (size_t i = 0; i < PlotButtons.size(); i++)
	{
		CreateWindowFunctionButton(BUTTON_STATE_BUTTON, PlotButtons[i], 350, i * 35, 0, 0, Buttons.GenericButton[i]);
	}
}

void PlotterGUIFunction::CreatePlotWindows()
{

	/*
	* Across RECT plot memebers, the .top (y) increments by 30
	*/

	for (size_t i = 0; i < PlotTexts.size(); i++)
	{
		if (i < 1) {
			HWND PlotSourceIN, PlotResultIN;
			CreateWindowFunction(BUTTON_STATE_STATIC, PlotTexts[i], 0, 0, 0, 0);
			PlotSourceIN = CreateWindowFunctionHWND(BUTTON_STATE_EDIT, Dot, 110, 0, 0, 0);
			PlotResultIN = CreateWindowFunctionHWND(BUTTON_STATE_EDIT, Dot, 210, 0, 0, 0);

			PlotSourceNamesIn.push_back(PlotSourceIN);
			PlotResultNamesIn.push_back(PlotResultIN);

			PlotSourceNamesIn[i] = PlotSourceIN;
			PlotResultNamesIn[i] = PlotResultIN;
			std::cout << "\n" << "First ";
		}
		else
		{
			HWND PlotSourceIN, PlotResultIN;
			CreateWindowFunction(BUTTON_STATE_STATIC, PlotTexts[i], 0, 30 * i, 0, 0);
			PlotSourceIN = CreateWindowFunctionHWND(BUTTON_STATE_EDIT, Dot, 110, 30 * i, 0, 0);
			PlotResultIN = CreateWindowFunctionHWND(BUTTON_STATE_EDIT, Dot, 210, 30 * i, 0, 0);
			
			PlotSourceNamesIn.push_back(PlotSourceIN);
			PlotResultNamesIn.push_back(PlotResultIN);

			PlotSourceNamesIn[i] = PlotSourceIN;
			PlotResultNamesIn[i] = PlotResultIN;
			std::cout << "\n" << "successive "<< i <<" " << PlotResultNamesIn.size();

		}
	}
}

void PlotterGUIFunction::GetPlotAndResultFiles() 
{
	std::cout<<"\n"<<PlotSourceNamesIn.size();
	std::cout << "\n" << PlotResultNamesIn.size();
	std::cout << "\n" << PlotTexts.size();

	for (size_t i = 0; i < PlotSourceNamesIn.size(); i++)
	{
		
	    wchar_t PerfPlotResult[100];
		GetText(PlotSourceNamesIn[i], PerfPlotResult);
		std::string str;
		str = StringFromWchar(PerfPlotResult);
		PlotSourceFilesNames.push_back(str);

		//std::cout << "\nString cont.  " << PlotSourceFilesNames[i];
	}

	std::cout << "\n Source string number " << PlotSourceFilesNames.size();

	for (size_t i = 0; i < PlotResultNamesIn.size(); i++)
	{

		wchar_t PerfResultResult[100];
		GetText(PlotResultNamesIn[i], PerfResultResult);
		std::string str;
		str = StringFromWchar(PerfResultResult);
		PlotResultFilesNames.push_back(str);

		//std::cout << "\nString cont.  " << PlotResultFilesNames[i];
	}

	std::cout << "\n Result string number " << PlotResultFilesNames.size();
};

void PlotterGUIFunction::CountSourceResultFiles()
{
	for (size_t i = 0; i < PlotSourceFilesNames.size(); i++)
	{
		if(PlotSourceFilesNames[i] != Dot)
		  Buttons.GenericCounter[0] ++;
	}

	for (size_t i = 0; i < PlotResultFilesNames.size(); i++)
	{
		if (PlotResultFilesNames[i] != Dot)
			Buttons.GenericCounter[1]++;

	}
	std::cout << "\nVALID SOURCE FILE COUNT " << Buttons.GenericCounter[0];
	std::cout << "\nVALID Result FILE COUNT " << Buttons.GenericCounter[1];

}

void PlotterGUIFunction::LoadPlots()
{
	if (Buttons.GenericCounter[0] == Buttons.GenericCounter[1])
	{
		PlotBulk.setNumberOfPlots(Buttons.GenericCounter[0]);
		PlotBulk.BulkSetFileName(GetSourceFiles());
		std::cout << "\nPLOTS LOADED...size "<< Buttons.GenericCounter[0];
		std::cout << "\nPLOTS LOADED...size of sourcefile " << PlotSourceNamesIn.size();
	}

};

void PlotterGUIFunction::CreatePlots()
{
	if (Buttons.GenericCounter[0] == Buttons.GenericCounter[1])
	{
		PlotBulk.BulkInitialize();
		PlotBulk.BulkPlotGraph(GetResultFiles());
		std::cout << "\nPLOTS COMMAND ";

	}
};

void PlotterGUIFunction::WMCreateFunction() 
{
	CreatePlotButtons();
	CreatePlotWindows();
}

void PlotterGUIFunction::WMCommandFunction() 
{
	if (LOWORD(wParam) == Buttons.GenericButton[0])
	{
		//Load button
		Buttons.GenericButtonState[0] = 1;
		std::cout << "\nLOAD_SEL";
		GetPlotAndResultFiles();
		CountSourceResultFiles();
	}

	if (LOWORD(wParam) == Buttons.GenericButton[2])
	{
		Buttons.GenericButtonState[1] = 1;
		//Plot button
		std::cout << "\nPLOT_SEL";
	}

	if (Buttons.GenericButtonState[1] == 1)
	{
		LoadPlots();
		CreatePlots();
		ResetPlot();
	}

}