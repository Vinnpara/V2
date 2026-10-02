#ifndef GRAPH_PLOTTER_H
#define GRAPH_PLOTTER_H
#include<FileEditor.h>


#include <vector>
#include <typeinfo>


class GraphPlotter
{
public:
	GraphPlotter();
	GraphPlotter(string Name);

	FileEditor File,
               BulkFile[10];

	void InitilalizePlot();
	void PlotGraph(string plotname);
	void SetFileName(string name);
	void setNumberOfPlots(std::size_t Num) { NumberOfPlots = Num; };

	void BulkSetFileName(std::vector<std::string> Names);
	void BulkInitialize();
	void BulkPlotGraph(std::vector<std::string> Names);


	void CloseFeedFile() { File.CloseFile(); }

private:

	char *FileName;
	std::size_t NumberOfPlots;

	//char SourceFilesName[10];
	//char ResultFilesName[10];

};

#endif
