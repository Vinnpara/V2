#include <serial\SerialPort.h>

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
            printf("\nERROR!!!");
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
            printf("\nERROR!!!");
        }
    }
    else {
        printf("\nHANDLE NOT INVALID");
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