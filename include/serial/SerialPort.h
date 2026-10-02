#ifndef SERIALPORT_H
#define SERIALPORT_H

#define ARDUINO_WAIT_TIME 2000
#define MAX_DATA_LENGTH 64

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include<SerialConnectionSpeed.h>

#include <string>
#include <vector>

#include <algorithm>

class SerialPort
{
private:
    HANDLE handler;
    bool connected;
    COMSTAT status;
    DWORD errors;
    SerialSpeed Speed;
    char * Port;
    char peekCache;
    bool isCacheFull;


public:
    SerialPort(char *portName);
    SerialPort(char* portName, int type);
    SerialPort(char* portName, SerialSpeed BaudRate);
    SerialPort() 
    {};
    ~SerialPort();

    void SetHandle(HANDLE& outHandle);

    int readSerialPort(char *buffer, unsigned int buf_size);
    int ReadSerialPortAfterPeek(char* buffer, unsigned int buf_size);
    bool writeSerialPort(char *buffer, unsigned int buf_size);
    void InitializeSerial(char* portName);
    bool InitializeSerialM(char* portName);

    bool InitializeSerialM(int portName);

    bool IndetifySerialPort(int portNumber, const std::string& expectedName, HANDLE& outHandle, int Baud);
    bool IndetifySerialPort(int portNumber, const std::string& expectedName, HANDLE& outHandle, int Baud, char SpecialChar);
    bool IndetifySerialPortHserial(int portNumber, const std::string& expectedName, HANDLE& outHandle, int Baud, char SpecialChar);

    void SetBaudRate(SerialSpeed BaudRate);
    void SetBaudRate(int BaudRate);

    void OpenConnection();
    bool isConnected();
    void SerialClose();
    unsigned char Peek(char* buffer);

};

#endif // SERIALPORT_H

