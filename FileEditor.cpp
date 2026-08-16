#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include<FileEditor.h>

FileEditor::FileEditor()
{

}

FileEditor::FileEditor(char* filename)
{
	this->NameOfFile = filename;



}

FileEditor::FileEditor(string filename)
{
    name = filename;



}

void FileEditor::SetName(string Name) {

    name = Name;

}

void FileEditor::OpenFile() {

	DataFile.open(NameOfFile);

}

void FileEditor::OpenFileStr() {

    DataFile.open(name);
    if (DataFile.is_open()) {
        std::cout << "\n OPEN  ";
    }
    else {
        std::cout << "\n CLOSED  ";
    }
}

void FileEditor::RecordDataFloat(float Data1, float Data2) {
    
    if (DataFile.is_open()) {

        //DataFile << "\nText";
        
        DataFile << Data1;
        DataFile << ",";
        DataFile << Data2;
        DataFile << "\n";

        //std::cout << "\n START  " << Data1;
        //std::cout << "\n STOP  " << Data2;
    }
    //std::cout << "\n START  ";
    //std::cout << "\n STOP  ";
}

void FileEditor::CloseFile() {

    DataFile.close();

}