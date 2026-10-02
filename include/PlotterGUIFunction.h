#ifndef PLOTTER_GUI_FUNCTION_H
#define PLOTTER_GUI_FUNCTION_H

#include <GUIFunction.h>
#include <GraphPlotter.h>

#include <Windows.h>
#include <string>
#include <vector>
#include <codecvt>
#include <locale>

class PlotterGUIFunction : public GUIFunction 
{
public:
	PlotterGUIFunction();

	void WMCreateFunction();
	void WMCommandFunction();
private:

	void InitializeLocalButtons();
	void CountSourceResultFiles();

	void CreatePlotButtons();
	void CreatePlotWindows();

	void GetPlotAndResultFiles();
	void LoadPlots();
	void CreatePlots();

	void ResetPlot() { Buttons.GenericButtonState[1] = 0; };

	std::vector<std::string> GetSourceFiles() { return PlotSourceFilesNames; };
	std::vector<std::string> GetResultFiles() { return PlotResultFilesNames; };

	std::string StringFromWchar(wchar_t *text);

	static GraphPlotter PlotBulk;

	std::vector <std::string> PlotButtons =
	{
		"Load data",
		"Verify data",
		"Plot",
		"Reset Plot"
	};

	std::vector <std::string> PlotTexts =
	{
		"Plot 1",
		"Plot 2",
		"Plot 3",
		"Plot 4",
		"Plot 5",
		"Plot 6",
		"Plot 7"
	};

	static std::vector<HWND> PlotSourceNamesIn;
	static std::vector<HWND> PlotResultNamesIn;

	std::vector<std::vector <wchar_t>>PlotSourceNamesOut[50];
	std::vector<std::vector <wchar_t>>PlotResultNamesOut[50];

	static std::vector<std::string> PlotSourceFilesNames;
	static std::vector<std::string> PlotResultFilesNames;

	/* For Ref:
	*  (x) left = 30,
	*  (y) top = 60,
	*  (w) right = 100,
	*  (h) bottom = 25
	* 
	*/
};

#endif