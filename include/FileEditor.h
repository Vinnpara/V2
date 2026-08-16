#ifndef FILE_EDITOR_H
#define FILE_EDITOR_H

#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

class FileEditor
{
public:
	FileEditor();
	FileEditor(char* filename);
	FileEditor(string filename);
	void OpenFile();
	void OpenFileStr();
	void SetName(string Name);
	void RecordDataFloat(float Data1, float Data2);
	void CloseFile();

private:
	std::ofstream DataFile;
	char* NameOfFile;
	string name;
};


#endif
