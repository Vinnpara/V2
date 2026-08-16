#ifndef BOARD_SELECTION_H
#define BOARD_SELECTION_H

enum BoardSelection {
	RADAR_BOARD = 1,
	GYROSCOPE_BOARD = 2,
	MOTOR_STEER_BOARD = 3
};

typedef enum BoardSelection BoardSelection;

#endif