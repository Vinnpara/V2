#ifndef SERIALORDER_H
#define SERIALORDER_H

// Define the orders that can be sent and received
enum SerialOrder {
  HELLO = 0,
  RADAR_DISTANCE = 1,
  RADAR_POSITION = 2,
  ALREADY_CONNECTED = 3,
  SERIAL_ERROR = 4,
  RECEIVED = 5,
  MOTOR_SPEED = 6,
  STOP=7,
  ARD_ITERATION_TIME=8,
  RADAR_DISTANCE_2 = 9, //Compound buffer, this means that distance and position for radar are 
  RADAR_POSITION_2 = 10,//sent in one buffer
};

typedef enum SerialOrder SerialOrder;

#endif
