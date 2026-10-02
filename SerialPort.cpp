#include <serial\SerialPort.h>
#include <SerialConnectionSpeed.h>
#include <iostream>
#include <windows.h>

#include <string>
#include <vector>
#include <string.h>

#include <stdexcept>       
#include <cstddef>   

const char START_MARKER = '[',
END_MARKER = ']';

SerialPort::SerialPort(char *portName)
{
    this->connected = false;

    this->handler = CreateFileA(static_cast<LPCSTR>(portName),
                                GENERIC_READ | GENERIC_WRITE,
                                0,
                                NULL,
                                OPEN_EXISTING,
                                FILE_ATTRIBUTE_NORMAL,
                                NULL);
    if (this->handler == INVALID_HANDLE_VALUE){
        if (GetLastError() == ERROR_FILE_NOT_FOUND){
            printf("\nERROR: Handle was not attached. Reason: %s not available\n", portName);
        }
    else
        {
            printf("\nERROR!!!_1");
        }
    }
    else {
        DCB dcbSerialParameters = {0};

        if (!GetCommState(this->handler, &dcbSerialParameters)) {
            printf("\nfailed to get current serial parameters");
        }
        else {
            dcbSerialParameters.BaudRate = CBR_9600;
            dcbSerialParameters.ByteSize = 8;
            dcbSerialParameters.StopBits = ONESTOPBIT;
            dcbSerialParameters.Parity = NOPARITY;
            dcbSerialParameters.fDtrControl = DTR_CONTROL_ENABLE;

            if (!SetCommState(handler, &dcbSerialParameters))
            {
                printf("\nALERT: could not set Serial port parameters\n");
            }
            else {
                this->connected = true;
                PurgeComm(this->handler, PURGE_RXCLEAR | PURGE_TXCLEAR);
                Sleep(ARDUINO_WAIT_TIME);
                printf("\nARD PORT CONNECTED\n");
                printf("\n \n");
                printf(portName);
            }
        }
    }
}

SerialPort::SerialPort(char* portName, int type) {

    this->connected = false;

    this->handler = CreateFileA(static_cast<LPCSTR>(portName),
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (this->handler == INVALID_HANDLE_VALUE) {
        if (GetLastError() == ERROR_FILE_NOT_FOUND) {
            printf("\nERROR: Handle was not attached. Reason: %s not available\n", portName);
        }
        else
        {
            printf("\nERROR!!!_2");
        }
    }
    else {
        printf("\nHANDLE NOT INVALID");
    }

}

SerialPort::SerialPort(char* portName, SerialSpeed BaudRate) {

    this->connected = false;

    this->handler = CreateFileA(static_cast<LPCSTR>(portName),
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    if (this->handler == INVALID_HANDLE_VALUE) {
        if (GetLastError() == ERROR_FILE_NOT_FOUND) {
            printf("\nERROR: Handle was not attached. Reason: %s not available\n", portName);
        }
        else
        {
            printf("\nERROR!!!_1");
        }
    }
    else {
        DCB dcbSerialParameters = { 0 };

        if (!GetCommState(this->handler, &dcbSerialParameters)) {
            printf("\nfailed to get current serial parameters");
        }
        else {
            switch (BaudRate) {
               case BAUD_RATE_9600:
               {
                   dcbSerialParameters.BaudRate = CBR_9600;
                   break;
               }
               case BAUD_RATE_57600:
               {
                   dcbSerialParameters.BaudRate = CBR_57600;
                   break;
               }
               case BAUD_RATE_115200:
               {
                   dcbSerialParameters.BaudRate = CBR_115200;
                   break;
               }
               default:
               {
                   dcbSerialParameters.BaudRate = CBR_9600;
                   break;
               }
            }
            dcbSerialParameters.ByteSize = 8;
            dcbSerialParameters.StopBits = ONESTOPBIT;
            dcbSerialParameters.Parity = NOPARITY;
            dcbSerialParameters.fDtrControl = DTR_CONTROL_ENABLE;

            if (!SetCommState(handler, &dcbSerialParameters))
            {
                printf("\nALERT: could not set Serial port parameters\n");
            }
            else {
                this->connected = true;
                PurgeComm(this->handler, PURGE_RXCLEAR | PURGE_TXCLEAR);
                Sleep(ARDUINO_WAIT_TIME);
                printf("\nARD PORT CONNECTED\n");
                printf("\n \n");
                printf(portName);
            }
        }
    }


}

void SerialPort::InitializeSerial(char* portName) {

    this->connected = false;


    this->handler = CreateFileA(static_cast<LPCSTR>(portName),
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (this->handler == INVALID_HANDLE_VALUE) {
        if (GetLastError() == ERROR_FILE_NOT_FOUND) {
            printf("\nERROR: Handle was not attached. Reason: %s not available\n", portName);
        }
        else
        {
            printf("\nERROR!!!_1");
        }
    }

    this->Port = portName;
}

bool SerialPort::InitializeSerialM(char* portName)
{
    this->connected = false;


    this->handler = CreateFileA(static_cast<LPCSTR>(portName),
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    bool SerialInitialized = 1;

    if (this->handler == INVALID_HANDLE_VALUE) {
        if (GetLastError() == ERROR_FILE_NOT_FOUND) {
            //printf("\nERROR: Handle was not attached. Reason: %s not available\n", portName);
            SerialInitialized = 0;
        }
        else
        {
            //printf("\nERROR!!!_1");
            SerialInitialized = 0;
        }

        
    }


    this->Port = portName;

    return SerialInitialized;
}

bool SerialPort::InitializeSerialM(int portName)
{
    this->connected = false;
    bool SerialInitialized = 1;

    std::wstring portNameSTR = L"\\\\.\\COM" + std::to_wstring(portName);

    char portCH = (char)portName;

    this->Port = &portCH;

    this->handler = CreateFileW(
        portNameSTR.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        0, nullptr,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        nullptr);


    if (this->handler == INVALID_HANDLE_VALUE) {
        if (GetLastError() == ERROR_FILE_NOT_FOUND) {
            printf("\nERROR: Handle was not attached. Reason: %s not available\n", (char)portName);
            SerialInitialized = 0;
        }
        else
        {
            printf("\nERROR!!!_1");
            SerialInitialized = 0;
        }

    }


    //this->Port = portName;

    return SerialInitialized;
}

void SerialPort::SetHandle(HANDLE& outHandle)
{
    this->handler = outHandle;
}

void SerialPort::SetBaudRate(SerialSpeed BaudRate) {

    if (this->handler != INVALID_HANDLE_VALUE) {

        DCB dcbSerialParameters = { 0 };
        dcbSerialParameters.DCBlength = sizeof(dcbSerialParameters);

        COMMTIMEOUTS timeouts = { 0 }; //Adding to help with MSB buffer overflows

        if (!GetCommState(this->handler, &dcbSerialParameters)) {
            printf("\nfailed to get current serial parameters");
        }
        else {
            switch (BaudRate) {
            case BAUD_RATE_9600:
            {
                dcbSerialParameters.BaudRate = CBR_9600;
                break;
            }
            case BAUD_RATE_57600:
            {
                dcbSerialParameters.BaudRate = CBR_57600;
                break;
            }
            case BAUD_RATE_115200:
            {
                dcbSerialParameters.BaudRate = CBR_115200;
                break;
            }
            default:
            {
                dcbSerialParameters.BaudRate = CBR_9600;
                break;
            }
            }
            dcbSerialParameters.ByteSize = 8;
            dcbSerialParameters.StopBits = ONESTOPBIT;
            dcbSerialParameters.Parity = NOPARITY;
            dcbSerialParameters.fDtrControl = DTR_CONTROL_ENABLE;

            // CRITICAL: Disable all flow control that flags down or corrupts streams
            dcbSerialParameters.fOutxCtsFlow = FALSE;  //Adding to help with MSB buffer overflows
            dcbSerialParameters.fOutxDsrFlow = FALSE;
            dcbSerialParameters.fDtrControl = DTR_CONTROL_ENABLE; // Keeps Arduino stable
            dcbSerialParameters.fOutX = FALSE;
            dcbSerialParameters.fInX = FALSE;
            dcbSerialParameters.fRtsControl = RTS_CONTROL_ENABLE;

            if (!SetCommState(handler, &dcbSerialParameters))
            {
                printf("\nALERT: could not set Serial port parameters\n");
            }
            else {
                this->connected = true;
                PurgeComm(this->handler, PURGE_RXCLEAR | PURGE_TXCLEAR);
                Sleep(ARDUINO_WAIT_TIME);
                printf("\nARD PORT CONNECTED\n");
                printf("\n \n");
                if (Port != nullptr)
                {
                    printf(Port);
                }
                else
                    printf("UNDEFINED PORT ");
            }

            timeouts.ReadIntervalTimeout = 0; //Adding to help with MSB buffer overflows
            timeouts.ReadTotalTimeoutConstant = 0;
            timeouts.ReadTotalTimeoutMultiplier = 0;
            timeouts.WriteTotalTimeoutConstant = 50;  // Maximum ms wait for writing
            timeouts.WriteTotalTimeoutMultiplier = 10;

            SetCommTimeouts(this->handler, &timeouts);


        }
    }
}

void SerialPort::SetBaudRate(int BaudRate)
{

    if (this->handler != INVALID_HANDLE_VALUE) {

        DCB dcbSerialParameters = { 0 };
        dcbSerialParameters.DCBlength = sizeof(dcbSerialParameters);

        COMMTIMEOUTS timeouts = { 0 }; //Adding to help with MSB buffer overflows

        if (!GetCommState(this->handler, &dcbSerialParameters)) {
            printf("\nfailed to get current serial parameters");
        }
        else {
            
            dcbSerialParameters.BaudRate = BaudRate;

            dcbSerialParameters.ByteSize = 8;
            dcbSerialParameters.StopBits = ONESTOPBIT;
            dcbSerialParameters.Parity = NOPARITY;
            dcbSerialParameters.fDtrControl = DTR_CONTROL_ENABLE;

            // CRITICAL: Disable all flow control that flags down or corrupts streams
            dcbSerialParameters.fOutxCtsFlow = FALSE;  //Adding to help with MSB buffer overflows
            dcbSerialParameters.fOutxDsrFlow = FALSE;
            dcbSerialParameters.fDtrControl = DTR_CONTROL_ENABLE; // Keeps Arduino stable
            dcbSerialParameters.fOutX = FALSE;
            dcbSerialParameters.fInX = FALSE;
            dcbSerialParameters.fRtsControl = RTS_CONTROL_ENABLE;

            if (!SetCommState(handler, &dcbSerialParameters))
            {
                printf("\nALERT: could not set Serial port parameters\n");
            }
            else {
                this->connected = true;
                PurgeComm(this->handler, PURGE_RXCLEAR | PURGE_TXCLEAR);
                Sleep(ARDUINO_WAIT_TIME);
                printf("\nARD PORT CONNECTED\n");
                printf("\n \n");
                if (Port != nullptr)
                {
                    printf(Port);
                }
                else
                    printf("UNDEFINED PORT ");
            }

            timeouts.ReadIntervalTimeout = 0; //Adding to help with MSB buffer overflows
            timeouts.ReadTotalTimeoutConstant = 0;
            timeouts.ReadTotalTimeoutMultiplier = 0;
            timeouts.WriteTotalTimeoutConstant = 50;  // Maximum ms wait for writing
            timeouts.WriteTotalTimeoutMultiplier = 10;

            SetCommTimeouts(this->handler, &timeouts);


        }
    }
}

SerialPort::~SerialPort()
{
    /*if (this->connected){
        this->connected = false;
        CloseHandle(this->handler);
        printf("\nSERIAL CLOSED");
    }*/
}

void SerialPort::OpenConnection() {

    DCB dcbSerialParameters = { 0 };

    if (!GetCommState(this->handler, &dcbSerialParameters)) {
        printf("\nfailed to get current serial parameters");
    }
    else {
        dcbSerialParameters.BaudRate = CBR_9600;
        dcbSerialParameters.ByteSize = 8;
        dcbSerialParameters.StopBits = ONESTOPBIT;
        dcbSerialParameters.Parity = NOPARITY;
        dcbSerialParameters.fDtrControl = DTR_CONTROL_ENABLE;

        if (!SetCommState(handler, &dcbSerialParameters))
        {
            printf("\nALERT: could not set Serial port parameters\n");
        }
        else {
            this->connected = true;
            PurgeComm(this->handler, PURGE_RXCLEAR | PURGE_TXCLEAR);
            Sleep(ARDUINO_WAIT_TIME);
        }
    }
    if (this->connected = true) {
        printf("\nCONNECTED TRUE");

    }
    else {
        printf("\nNOT CONNECTED TRUE");
    }
}

int SerialPort::readSerialPort(char *buffer, unsigned int buf_size)
{
    DWORD bytesRead;
    unsigned int toRead = 0;

    ClearCommError(this->handler, &this->errors, &this->status);

    if (this->status.cbInQue > 0){
        if (this->status.cbInQue > buf_size){
            toRead = buf_size;
        }
        else toRead = this->status.cbInQue;
    }

    if (ReadFile(this->handler, buffer, toRead, &bytesRead, NULL)) return bytesRead;

    return 0;
}

bool SerialPort::writeSerialPort(char *buffer, unsigned int buf_size)
{
    DWORD bytesSend;

    if (!WriteFile(this->handler, (void*) buffer, buf_size, &bytesSend, 0)){
        ClearCommError(this->handler, &this->errors, &this->status);
        return false;
    }
    else return true;
}

bool SerialPort::isConnected()
{
    return this->connected;
}

void SerialPort::SerialClose() {

    if (this->connected) {
        this->connected = false;
        CloseHandle(this->handler);
        printf("\nSERIAL CLOSED");
    }

}

int SerialPort::ReadSerialPortAfterPeek(char* buffer, unsigned int buf_size)
{
    DWORD bytesRead;
    unsigned int toRead = 0;

    if (!connected) return 0;

    // 1. If a peek() happened before this, consume and clear the cache
    if (isCacheFull) {
        isCacheFull = false;
        return (unsigned char)peekCache;
    }
    // 2. Otherwise, do a normal standard hardware read
    
    /*char incomingByte;
    if (ReadFile(this->handler, &incomingByte, 1, &bytesRead, NULL) && bytesRead > 0) {
        return (unsigned char)incomingByte;
    }*/

    if (this->status.cbInQue > 0) {
        if (this->status.cbInQue > buf_size) {
            toRead = buf_size;
        }
        else toRead = this->status.cbInQue;
    }

    if (ReadFile(this->handler, buffer, toRead, &bytesRead, NULL)) return bytesRead;

    return 0;
}

unsigned char SerialPort::Peek(char* buffer)
{
    if (!this->connected) return 0;

    if (isCacheFull) {
        return (unsigned char)peekCache;
    }

    DWORD errors,
          bytesRead;
    COMSTAT status;

    ClearCommError(this->handler, &this->errors, &this->status);

    if (this->status.cbInQue > 0) 
    {
        // Destructively pull 1 byte out of the Windows hardware queue
        if (ReadFile(this->handler, &peekCache, 1, &bytesRead, NULL) && bytesRead > 0) {
            //buffer = &peekCache;
            isCacheFull = true; // Lock it into our software layer
            return (unsigned char)peekCache;
        }
    }

    return 0; // Buffer was totally empty

}

bool SerialPort::IndetifySerialPort(int portNumber, const std::string& expectedName, HANDLE& outHandle, int Baud, char SpecialChar)
{
    std::wstring portName = L"\\\\.\\COM" + std::to_wstring(portNumber);


    this->connected = false;


    HANDLE hSerial = CreateFileW(portName.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    bool SerialInitialized = 1;

    if (hSerial == INVALID_HANDLE_VALUE) {


        if (GetLastError() == ERROR_FILE_NOT_FOUND) {
            //printf("\nERROR: Handle was not attached. Reason: %s not available\n", portName);
            SerialInitialized = 0;
        }
        else
        {
            //printf("\nERROR!!!_1");
            SerialInitialized = 0;
        }

        return false; //Port does not exist or in use
    }

    DCB dcbSerialParameters = { 0 };
    dcbSerialParameters.DCBlength = sizeof(dcbSerialParameters);

    COMMTIMEOUTS timeouts = { 0 }; //Adding to help with MSB buffer overflows

    if (!GetCommState(hSerial, &dcbSerialParameters))
    {
        printf("\nfailed to get current serial parameters");
        CloseHandle(hSerial);
        return false; //Port does not exist or in use
    }

    dcbSerialParameters.BaudRate = Baud;
    dcbSerialParameters.ByteSize = 8;
    dcbSerialParameters.StopBits = ONESTOPBIT;
    dcbSerialParameters.Parity = NOPARITY;
    dcbSerialParameters.fDtrControl = DTR_CONTROL_ENABLE;

    // CRITICAL: Disable all flow control that flags down or corrupts streams
    dcbSerialParameters.fOutxCtsFlow = FALSE;  //Adding to help with MSB buffer overflows
    dcbSerialParameters.fOutxDsrFlow = FALSE;
    dcbSerialParameters.fDtrControl = DTR_CONTROL_ENABLE; // Keeps Arduino stable
    dcbSerialParameters.fOutX = FALSE;
    dcbSerialParameters.fInX = FALSE;
    dcbSerialParameters.fRtsControl = RTS_CONTROL_ENABLE;



    if (!SetCommState(hSerial, &dcbSerialParameters))
    {
        printf("\nALERT: could not set Serial port parameters\n");
        return false;
    }
    else {
        this->connected = true;

        printf("\nCONNECTION FOUND\n");
        printf("\n \n");

    }

    timeouts.ReadIntervalTimeout = 50; //Adding to help with MSB buffer overflows
    timeouts.ReadTotalTimeoutConstant = 500;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;  // Maximum ms wait for writing
    timeouts.WriteTotalTimeoutMultiplier = 10;

    SetCommTimeouts(hSerial, &timeouts);

    Sleep(ARDUINO_WAIT_TIME);
    PurgeComm(hSerial, PURGE_RXCLEAR | PURGE_TXCLEAR);

    // Send identification query: START_MARKER + '?' + END_MARKER

    DWORD StartTimeSend = GetTickCount();

    while (GetTickCount() - StartTimeSend < 100)
    {

        char query[] = { START_MARKER, SpecialChar, END_MARKER };
        DWORD bytesWritten = 0;
        WriteFile(hSerial, query, sizeof(query), &bytesWritten, nullptr);

    }

    /*
    // Read response
    char response[32] = { 0 };

    DWORD bytesRead = 0;
    COMSTAT HandleStatus;
    DWORD Errors;

    unsigned int toRead = 0;

    //ReadFile(this->handler, response, 128, &bytesRead, NULL);

    bytesRead = ReadFile(hSerial, response, sizeof(response) - 1, &bytesRead, nullptr);

    //response[bytesRead] = '\0';

    std::string resp(response);

    printf("\nRESPONSE REC  ");
    std::cout << response <<" Bytes read "<< bytesRead;

    */


    DWORD bytesRead = 0;
    std::string result;
    DWORD startTime = GetTickCount();
    bool started = false;

    while (GetTickCount() - startTime < 2500)
    {
        char c;
        DWORD bytesRead = 0;
        if (!ReadFile(hSerial, &c, 1, &bytesRead, nullptr) || bytesRead == 0)
        {
            std::cout << " \nTimeout ";
            continue;   // ReadIntervalTimeout elapsed with no byte; keep waiting
        }
        std::cout << " \nReceived " << c;

        if (c == START_MARKER)
        {
            result.clear();
            started = true;
            continue;

        }
        if (started)
        {
            if (c == END_MARKER)
                break;           // complete response

            result += c;
        }
    }

    printf("\nRESPONSE REC  ");
    std::cout << result << " Bytes read " << bytesRead;

    //printf(resp.c_str());

    if (result.find(expectedName) != std::string::npos)
    {
        outHandle = hSerial;   // caller now owns this handle
        printf("\nARD PORT FOUND FOR \n");
        printf(expectedName.c_str());

        CloseHandle(hSerial);

        return true;
    }


    CloseHandle(hSerial);
    return false;
}

bool SerialPort:: IndetifySerialPort(int portNumber, const std::string& expectedName, HANDLE& outHandle, int Baud)
{
    std::wstring portName = L"\\\\.\\COM" + std::to_wstring(portNumber);


    this->connected = false;


    HANDLE hSerial = CreateFileW(portName.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    bool SerialInitialized = 1;

    if (hSerial == INVALID_HANDLE_VALUE) {


        if (GetLastError() == ERROR_FILE_NOT_FOUND) {
            //printf("\nERROR: Handle was not attached. Reason: %s not available\n", portName);
            SerialInitialized = 0;
        }
        else
        {
            //printf("\nERROR!!!_1");
            SerialInitialized = 0;
        }

        return false; //Port does not exist or in use
    }

    DCB dcbSerialParameters = { 0 };
    dcbSerialParameters.DCBlength = sizeof(dcbSerialParameters);

    COMMTIMEOUTS timeouts = { 0 }; //Adding to help with MSB buffer overflows

    if (!GetCommState(hSerial, &dcbSerialParameters))
    {
        printf("\nfailed to get current serial parameters");
        CloseHandle(hSerial);
        return false; //Port does not exist or in use
    }

    dcbSerialParameters.BaudRate = Baud;
    dcbSerialParameters.ByteSize = 8;
    dcbSerialParameters.StopBits = ONESTOPBIT;
    dcbSerialParameters.Parity = NOPARITY;
    dcbSerialParameters.fDtrControl = DTR_CONTROL_ENABLE;

    // CRITICAL: Disable all flow control that flags down or corrupts streams
    dcbSerialParameters.fOutxCtsFlow = FALSE;  //Adding to help with MSB buffer overflows
    dcbSerialParameters.fOutxDsrFlow = FALSE;
    dcbSerialParameters.fDtrControl = DTR_CONTROL_ENABLE; // Keeps Arduino stable
    dcbSerialParameters.fOutX = FALSE;
    dcbSerialParameters.fInX = FALSE;
    dcbSerialParameters.fRtsControl = RTS_CONTROL_ENABLE;



    if (!SetCommState(hSerial, &dcbSerialParameters))
    {
        printf("\nALERT: could not set Serial port parameters\n");
        return false;
    }
    else {
        this->connected = true;

        printf("\nCONNECTION FOUND\n");
        printf("\n \n");

    }

    timeouts.ReadIntervalTimeout = 50; //Adding to help with MSB buffer overflows
    timeouts.ReadTotalTimeoutConstant = 500;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;  // Maximum ms wait for writing
    timeouts.WriteTotalTimeoutMultiplier = 10;

    SetCommTimeouts(hSerial, &timeouts);

    Sleep(ARDUINO_WAIT_TIME);
    PurgeComm(hSerial, PURGE_RXCLEAR | PURGE_TXCLEAR);

    // Send identification query: START_MARKER + '?' + END_MARKER

    DWORD StartTimeSend = GetTickCount();

    while (GetTickCount() - StartTimeSend < 100)
    {

        char query[] = { START_MARKER, '?', END_MARKER };
        DWORD bytesWritten = 0;
        WriteFile(hSerial, query, sizeof(query), &bytesWritten, nullptr);

    }

    /*
    // Read response
    char response[32] = { 0 };

    DWORD bytesRead = 0;
    COMSTAT HandleStatus;
    DWORD Errors;

    unsigned int toRead = 0;

    //ReadFile(this->handler, response, 128, &bytesRead, NULL);

    bytesRead = ReadFile(hSerial, response, sizeof(response) - 1, &bytesRead, nullptr);

    //response[bytesRead] = '\0';

    std::string resp(response);

    printf("\nRESPONSE REC  ");
    std::cout << response <<" Bytes read "<< bytesRead;

    */
    
    
    DWORD bytesRead = 0;
    std::string result;
    DWORD startTime = GetTickCount();
    bool started = false;

    while (GetTickCount() - startTime < 2500)
    {
        char c;
        DWORD bytesRead = 0;
        if (!ReadFile(hSerial, &c, 1, &bytesRead, nullptr) || bytesRead == 0)
        {
            std::cout << " \nTimeout ";
            continue;   // ReadIntervalTimeout elapsed with no byte; keep waiting
        }
        std::cout << " \nReceived "<<c;

        if (c == START_MARKER)
        {
            result.clear();
            started = true;
            continue;
            
        }
        if (started)
        {
            if (c == END_MARKER)
                break;           // complete response
                                
            result += c;
        }
    }
    
    printf("\nRESPONSE REC  ");
    std::cout << result <<" Bytes read "<< bytesRead;
    
 



    //printf(resp.c_str());

    if (result.find(expectedName) != std::string::npos)
    {
        outHandle = hSerial;   // caller now owns this handle
        printf("\nARD PORT FOUND FOR \n");
        printf(expectedName.c_str());

        CloseHandle(hSerial);

        return true;
    }


    CloseHandle(hSerial);
    return false;
}

bool SerialPort::IndetifySerialPortHserial(int portNumber, const std::string& expectedName, HANDLE& outHandle, int Baud, char SpecialChar)
{
    //Meant to transfer the serial handle

    std::wstring portName = L"\\\\.\\COM" + std::to_wstring(portNumber);


    this->connected = false;


    this->handler = CreateFileW(portName.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    bool SerialInitialized = 1;

    if (this->handler == INVALID_HANDLE_VALUE) {


        if (GetLastError() == ERROR_FILE_NOT_FOUND) {
            //printf("\nERROR: Handle was not attached. Reason: %s not available\n", portName);
            SerialInitialized = 0;
        }
        else
        {
            //printf("\nERROR!!!_1");
            SerialInitialized = 0;
        }

        return false; //Port does not exist or in use
    }

    DCB dcbSerialParameters = { 0 };
    dcbSerialParameters.DCBlength = sizeof(dcbSerialParameters);

    COMMTIMEOUTS timeouts = { 0 }; //Adding to help with MSB buffer overflows

    if (!GetCommState(this->handler, &dcbSerialParameters))
    {
        printf("\nfailed to get current serial parameters");
        CloseHandle(this->handler);
        return false; //Port does not exist or in use
    }

    dcbSerialParameters.BaudRate = Baud;
    dcbSerialParameters.ByteSize = 8;
    dcbSerialParameters.StopBits = ONESTOPBIT;
    dcbSerialParameters.Parity = NOPARITY;
    dcbSerialParameters.fDtrControl = DTR_CONTROL_ENABLE;

    // CRITICAL: Disable all flow control that flags down or corrupts streams
    dcbSerialParameters.fOutxCtsFlow = FALSE;  //Adding to help with MSB buffer overflows
    dcbSerialParameters.fOutxDsrFlow = FALSE;
    dcbSerialParameters.fDtrControl = DTR_CONTROL_ENABLE; // Keeps Arduino stable
    dcbSerialParameters.fOutX = FALSE;
    dcbSerialParameters.fInX = FALSE;
    dcbSerialParameters.fRtsControl = RTS_CONTROL_ENABLE;



    if (!SetCommState(this->handler, &dcbSerialParameters))
    {
        printf("\nALERT: could not set Serial port parameters\n");
        return false;
    }
    else {
        this->connected = true;
        //PurgeComm(this->handler, PURGE_RXCLEAR | PURGE_TXCLEAR);
        printf("\nCONNECTION FOUND\n");
        printf("\n \n");

    }

    timeouts.ReadIntervalTimeout = 50; //Adding to help with MSB buffer overflows
    timeouts.ReadTotalTimeoutConstant = 500;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;  // Maximum ms wait for writing
    timeouts.WriteTotalTimeoutMultiplier = 10;

    SetCommTimeouts(this->handler, &timeouts);

    Sleep(ARDUINO_WAIT_TIME);
    PurgeComm(this->handler, PURGE_RXCLEAR | PURGE_TXCLEAR);

    // Send identification query: START_MARKER + '?' + END_MARKER

    DWORD StartTimeSend = GetTickCount();

    while (GetTickCount() - StartTimeSend < 100)
    {

        char query[] = { START_MARKER, SpecialChar, END_MARKER };
        DWORD bytesWritten = 0;
        WriteFile(this->handler, query, sizeof(query), &bytesWritten, nullptr);

    }

    DWORD bytesRead = 0;
    std::string result;
    DWORD startTime = GetTickCount();
    bool started = false;

    while (GetTickCount() - startTime < 2500)
    {
        char c;
        DWORD bytesRead = 0;
        if (!ReadFile(this->handler, &c, 1, &bytesRead, nullptr) || bytesRead == 0)
        {
            std::cout << " \nTimeout ";
            continue;   // ReadIntervalTimeout elapsed with no byte; keep waiting
        }
        std::cout << " \nReceived " << c;

        if (c == START_MARKER)
        {
            result.clear();
            started = true;
            continue;

        }
        if (started)
        {
            if (c == END_MARKER)
                break;           // complete response

            result += c;
        }
    }

    printf("\nRESPONSE REC  ");
    std::cout << result << " Bytes read " << bytesRead;

    //printf(resp.c_str());

    if (result.find(expectedName) != std::string::npos)
    {
        outHandle = this->handler;   // caller now owns this handle
        printf("\nARD PORT FOUND FOR \n");
        printf(expectedName.c_str());

        //CloseHandle(hSerial);

        return true;
    }


    //CloseHandle(hSerial);
    return false;

}