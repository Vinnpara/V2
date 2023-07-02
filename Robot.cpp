// Robot.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include<conio.h> 
#include <Windows.h>
#include <cstdio>
#include <mmsystem.h>
#include <cstdio>
#include <cstdlib>
#include <windows.h>
#include<chrono>
#include<string>
#include<stdlib.h>
#include <cmath>
#include <time.h>


//#include "timer.h"
#include "Robot.h"
#include"SerialPort.h"


#pragma comment(lib, "winmm.lib")
using namespace std;
using namespace chrono;


#define KEY(c) ( GetAsyncKeyState((int)(c)) & (SHORT)0x8000 )


char output[MAX_DATA_LENGTH];
char incomingData[MAX_DATA_LENGTH];
char* port = "\\\\.\\COM3";
char input[MAX_DATA_LENGTH];
SerialPort arduino(port);

Robot::Robot() {
	

	//if (arduino.isConnected()) {
	//	cout << "Connection made" << endl << endl;
	//}
	//else {
	//	cout << "Error in port name" << endl << endl;
	//}
	
	
	//v1 = 85;
	//v2 = 85;
	//v = 's';
}

Robot::~Robot() {
	cout << "\nDest ";
}

void Robot::serial_r_w() {
	
	//writing part
	const int NMAX = 64;
	short int* ve1, * ve2;
	//short int ve1, ve2;
	char buffin[NMAX];
	

	char* pi, * pb, * e, * st, * en;
	pb = buffin;
	pi = pb;

	ve1 = (short int*)pi;
	*ve1 = v1;
	pi += sizeof(short int);

	ve2 = (short int*)pi;
	*ve2 = v2;
	pi += sizeof(short int);

	/*e = (char*)pi;
	*e = v;*/
	
	arduino.writeSerialPort(buffin, 4);
	
	Sleep(10);
	arduino.readSerialPort(output, MAX_DATA_LENGTH);
	//reading part
	char* p_buffer, * p;
	short unsigned int* l, * di;

	short unsigned int light, dist;
	int lig, dis;

	p_buffer = output;
	p = p_buffer;

	l = (short unsigned int*)p;
	p += sizeof(short unsigned int);
	di = (short unsigned int*)p;

	light = l[0];
	dist = di[0];
	lig = (int)light;
	dis = (int)dist;
	d = dist;
	
	//cout << "DIR " << v << "\tData " << buffin[0] << endl;
	//cout << "\nlight " << light << "\tDist " << dist << endl;
	// "\nRIGHT_MOTOR " << v1 << "\tLEFT_MOTOR " << v2 <<' '<< buffin[0] << ' ' << buffin[1] << ' ' << buffin[2] << buffin[3] <<' ' << buffin[4] <<
	cout << "\nlight " << light << "\tDist " << dist <<' '<<"RIGHT_MOTOR " << v1 << "\tLEFT_MOTOR " << v2 <<' '<< buffin[0]<<' '<< buffin[1] << ' ' << buffin[2] << buffin[3] <<endl;
};

void Robot::send_info() {
	//writing part
	const int NMAX = 64;
	short int* ve1, * ve2;
	//short int ve1, ve2;
	char buffin[MAX_DATA_LENGTH];
	char end = 'e';

	char* pi, * pb, * e;
	//pb = buffin;
	int sz;
	sz = 1;
	//buffout = new char[sz];

	pb = buffin;
	pi = pb;

	
	ve1 = (short int*)pi;
	*ve1 = v1;
	pi += sizeof(short int);
	
	
	ve2 = (short int*)pi;
	*ve2 = v2;
	pi += sizeof(short int);
	
	
	/*e = (char*)pi;
	*e = v;*/
	
	
	//string OUT = string.valueOf(buffout);
	
	//Sleep(10);
	arduino.writeSerialPort(buffin, 4);

	//cout << "DIR " << v<<"\tData "<< buffin[0] << endl;
	//buffin[0]<<' '<< buffin[1] << ' ' << buffin[2] <<endl;
	cout <<"\nRIGHT_MOTOR " << v1 << "\tLEFT_MOTOR " << v2 <<buffin[0]<<' '<< buffin[1] << ' ' << buffin[2]<<' ' << buffin[3]<<endl;
	//<<"DIR "<<v

	//delete[] buffout;
}

void Robot::get_info() {

	Sleep(1);
	arduino.readSerialPort(output, MAX_DATA_LENGTH);

	char* p_buffer, * p;
	float* l, * di;
	short int* vi1, * vi2;
	float* servo_pos;

	float light, dist;
	short int v1, v2;
	int lig, dis;
	float servo_position, serv_pos;

	p_buffer = output;
	p = p_buffer;



	//l = (short unsigned int*)p;
	//p += sizeof(short unsigned int);
	di = (float*)p;
	
	p += sizeof(float);	
	servo_pos = (float*)p;

	
	/*p += sizeof(short unsigned int);
	vi1 = (short int*)p;
	p += sizeof(short int);
	vi2 = (short int*)p;*/


	//light = l[0];
	dist = di[0];
	
	servo_position = servo_pos[0];
	serv_pos = (float)servo_position;

	//v1 = vi1[0];
	//v2 = vi2[0];

	//lig = (int)light;
	dis = (int)dist;
	d = dist;

	//cout << "\nlight " << light << "\tDist " << dist << "\nVI1 " << v1 << "\tVI2 " << v2 << endl;
	cout <<"\nDist " << dist <<"\nServo pos "<< serv_pos <<endl;

	//cout << "\nDist " << dist <<endl;
}

/*void Robot:: manual() {
	if (KEY(VK_DOWN)) //FORWARD
	{
		//v1 = 0;
		//v2 = 180;
		v = 'b';
	}
	if (KEY(VK_UP))
	{
		//v1 = 180;
		//v2 = 0;
		v = 'f';
	}
	if (KEY(VK_LEFT))
	{
		//v1 = 0; //0
		//v2 = 0; //85
		v = 'l';
	}
	if (KEY(VK_RIGHT)) //V1 right 
	{
		//v1 = 180; //0
		//v2 = 180; //85
		v = 'r';
	}
	if (KEY(VK_SPACE)) //STOP
	{
		//v1 = 85;
		//v2 = 85;
		v = 's';
	}
	
};*/

void Robot::manual() {
	if (KEY(VK_DOWN)) //FORWARD
	{
		v1 = 0;
		v2 = 180;
	}
	if (KEY(VK_UP))
	{
		v1 = 180;
		v2 = 0;
	}
	if (KEY(VK_LEFT))
	{
		v1 = 0; //0
		v2 = 0; //85
	}
	if (KEY(VK_RIGHT)) //V1 right 
	{
		v1 = 180; //0
		v2 = 180; //85
	}
	if (KEY(VK_SPACE)) //STOP
	{
		v1 = 85;
		v2 = 85;
	}

	//send_info(v1, v2);

};


void Robot::em_stop() {

	if (d <= 7) {
		v1 = v2 = 85;
	}

};

void Robot::robot_go() {

	v1 = 180;
	v2 = 0;

};
void Robot::robot_stop() {

	v1 = 85;
	v2 = 85;

};
void Robot::robot_right() {
	v1 = 180;
	v2 = 85;
	//cout << "\nTURNING RIGHT";
	/*
	short int* v1, * v2;
	const int NMAX = 64;
	char buffin[NMAX];*/
};

void Robot::robot_left() {
	v1 = 85;
	v2 = 0;

	send_info();

};

void Robot::robot_turn_right(short int& v1, short int& v2) {
	/*bool turning = true;
	while (turning == true) {
		double ti;
		ti = high_resolution_time();
		if (ti >= 3.0) {
			v1 = 180;
			v2 = 0;
			cout << "\nGOING STRAIGHT";
			break;
		}
	}*/

	bool turning = true;
	while (turning == true) {
		v1 = 85;
		v2 = 0;
		double ti;
		//ti = high_resolution_time();
		/*if (ti >= 3.0) {
			v1 = 180;
			v2 = 0;
			turning = false;
			break;
		
		}*/
	}


}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
