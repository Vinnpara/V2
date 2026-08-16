#ifndef SERIALPORT_H
#define SERIALPORT_H

#define ARDUINO_WAIT_TIME 2000
#define MAX_DATA_LENGTH 64

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include<SerialConnectionSpeed.h>

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

    int readSerialPort(char *buffer, unsigned int buf_size);
    int ReadSerialPortAfterPeek(char* buffer, unsigned int buf_size);
    bool writeSerialPort(char *buffer, unsigned int buf_size);
    void InitializeSerial(char* portName);
    void SetBaudRate(SerialSpeed BaudRate);
    void OpenConnection();
    bool isConnected();
    void SerialClose();
    unsigned char Peek(char* buffer);

};

#endif // SERIALPORT_H

