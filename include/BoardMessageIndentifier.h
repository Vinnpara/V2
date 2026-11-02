#ifndef BOARD_IDENT_H
#define BOARD_IDENT_H

enum BoardMessageIndentifier {
	RADAR_BOARD_MESSAGE = 4000,
	GYROSCOPE_BOARD_MESSAGE = 3000,
	MOTOR_STEER_BOARD_MESSAGE = 5000
};

typedef enum BoardMessageIndentifier BoardMessageIndentifier;

#endif