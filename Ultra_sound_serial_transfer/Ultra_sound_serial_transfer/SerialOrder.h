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
  MEASURED_ROLL=8,
  MEASURED_PITCH = 9,  
  MEASURED_YAW = 10,
};

typedef enum SerialOrder SerialOrder;

#endif
