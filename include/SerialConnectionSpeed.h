#ifndef SERIALSPEED_H
#define SERIALSPEED_H

// Define the Serial Connection speeds
enum SerialSpeed {
	BAUD_RATE_9600 = 1,
	BAUD_RATE_57600 = 2,
	BAUD_RATE_115200 = 3,
	BAUD_RATE_14400 = 4,
	BAUD_RATE_31250 = 5,
};

typedef enum SerialSpeed SerialSpeed;

#endif
