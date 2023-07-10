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
  MEASURED_ROLL = 8,
  MEASURED_PITCH = 9,
  MEASURED_YAW = 10,
  MEASURED_ACCEL_X = 11,
  MEASURED_ACCEL_Y = 12,
  MEASURED_ACCEL_Z = 13,
  REQUEST_ROLL = 18,
  REQUEST_PITCH = 19,
  REQUEST_YAW = 20,
  PC_NOT_READY=15,
};

typedef enum SerialOrder SerialOrder;

#endif