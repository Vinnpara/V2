#include <fstream>
#include <cstdint>  // int8_t, int16_t, ...
#include<serial\SerialPort.h>
#include<serial\SerialOrder.h>
#include<serial\SerialComms.h>



using namespace std;
void write_i8(std::fstream& file, int8_t num)
{
	file.write((char*)&num, sizeof(int8_t));
}

void write_order(std::fstream& file, enum SerialOrder order)
{
	write_i8(file, (int8_t)order);
}

void write_i16(std::fstream& file, int16_t num)
{
	int8_t buffer[2] = { (int8_t)(num & 0xff), (int8_t)(num >> 8) };
	file.write((char*)buffer, 2 * sizeof(int8_t));
}

void write_i32(std::fstream& file, int32_t num)
{
	int8_t buffer[4] = { (int8_t)(num & 0xff), (int8_t)(num >> 8 & 0xff), (int8_t)(num >> 16 & 0xff), (int8_t)(num >> 24 & 0xff) };
	file.write((char*)buffer, 4 * sizeof(int8_t));
}

void write_u32(std::fstream& file, uint16_t num)
{
	uint8_t buffer[2] = { (uint8_t)(num & 0xff), (uint8_t)(num >> 8) };
	file.write((char*)buffer, 2 * sizeof(uint8_t));
}

SerialOrder read_order(SerialPort& serial_port)
{
	char buffer[MAX_DATA_LENGTH];
	serial_port.readSerialPort(buffer, sizeof(uint8_t));
	return (SerialOrder)buffer[0];
}

SerialOrder read_order_specific(char *buffer, const int Index)
{
	//char buffer[MAX_DATA_LENGTH];
	//serial_port.readSerialPort(buffer, 1);
	return (SerialOrder)buffer[Index];
}


int8_t read_i8(SerialPort& serial_port)
{  //int8_t a signed integer type with a width of exactly 8 bits.

	// We have to cast it to keep the sign
	char buffer[MAX_DATA_LENGTH];
	serial_port.readSerialPort(buffer, 1);
	return (int8_t) static_cast<signed char>(buffer[0]);
}

int16_t read_i16(SerialPort& serial_port)
{   //int16_t 16bit integer type

	char buffer[MAX_DATA_LENGTH];
	serial_port.readSerialPort(buffer, 2);
	return (((int16_t)buffer[0]) & 0xff) | (((int16_t)buffer[1]) << 8 & 0xff00);
}

void read_i16_compound(SerialPort& serial_port, SerialOrder OrderExpected, int16_t * ValuesReceived)
{   //int16_t 16bit integer type
	//Expects to read a compund buffer, will have 2 enums and 2 data types in the same buffer
	char buffer[MAX_DATA_LENGTH];

	SerialOrder OrderReceived, NextOrder;
	OrderReceived = read_order(serial_port);

	int16_t FirstValueRecieved, SecondValueReceived;

	if (OrderReceived == OrderExpected) {

		
		serial_port.readSerialPort(buffer, 6);
		FirstValueRecieved =(((int16_t)buffer[1]) & 0xff) | (((int16_t)buffer[2]) << 8 & 0xff00);
		
		ValuesReceived[0] = FirstValueRecieved;
		//Expecting the next Order to be PreviousOrder+1
		NextOrder = read_order_specific(buffer,3);

		if (NextOrder == (OrderExpected + 1)) {

			SecondValueReceived = (((int16_t)buffer[4]) & 0xff) | (((int16_t)buffer[5]) << 8 & 0xff00);
			ValuesReceived[1] = SecondValueReceived;
		}


	}
	//return *ValuesReceived;
}


uint16_t uread_i16(SerialPort& serial_port)
{   //int16_t 16bit integer type
	char buffer[MAX_DATA_LENGTH];
	serial_port.readSerialPort(buffer, 2);
	return (((uint16_t)buffer[0]) & 0xff) | (((uint16_t)buffer[1]) << 8 & 0xff00);
}

int32_t read_i32(SerialPort& serial_port)
{
	int32_t ValBad= 8000020;
	char buffer[MAX_DATA_LENGTH];
	serial_port.readSerialPort(buffer, 4);
	if (sizeof(buffer) >= 4) {
		return (((int32_t)buffer[0]) & 0xff) | (((int32_t)buffer[1]) << 8 & 0xff00) | (((int32_t)buffer[2]) << 16 & 0xff0000) | (((int32_t)buffer[3]) << 24 & 0xff000000);
	}
	else
		return ValBad;
	
}

