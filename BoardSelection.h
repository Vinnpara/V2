#ifndef SERIALORDER_H
#define SERIALORDER_H

enum BoardSelection {
	RADAR_BOARD = 1,
	GYROSCOPE_BOARD = 2,
	MOTOR_STEER_BOARD = 3
};

typedef enum BoardSelection BoardSelection;

#endif