#ifndef SERIALSPEED_H
#define SERIALSPEED_H

// Define the Serial Connection speeds
enum SerialSpeed {
	BAUD_RATE_9600 = 1,
	BAUD_RATE_57600 = 2,
	BAUD_RATE_115200 = 3,	
};

typedef enum SerialSpeed SerialSpeed;

#endif
