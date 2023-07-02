

#include <fstream>
#include <cstdint>  // int8_t, int16_t

#include<serial\SerialPort.h>
#include<serial\SerialOrder.h>

void write_order(std::fstream& file, enum SerialOrder order);

void write_i8(std::fstream& file, int8_t num);

void write_i16(std::fstream& file, int16_t num);

void write_i32(std::fstream& file, int32_t num);

void write_u32(std::fstream& file, uint16_t num);

SerialOrder read_order(SerialPort& serial_port);

int8_t read_i8(SerialPort& serial_port);

int16_t read_i16(SerialPort& serial_port);

void read_i16_compound(SerialPort& serial_port, SerialOrder OrderExpected, int16_t* ValuesReceived);

uint16_t uread_i16(SerialPort& serial_port);

int32_t read_i32(SerialPort& serial_port);


