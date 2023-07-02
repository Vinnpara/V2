


#include"SerialPort.h"
#define KEY(c) ( GetAsyncKeyState((int)(c)) & (SHORT)0x8000 )


#pragma comment(lib, "winmm.lib")


class Robot {
	short int v1, v2;
	short unsigned int d;
	char v;

public:	
	Robot();
	void manual();
	void em_stop();
	void robot_go();
	void robot_stop();
	void robot_right();
	void robot_left();
	void robot_turn_right(short int& v1, short int& v2);
	void serial_r_w();
	void send_info();
	void get_info();
	~Robot();
};