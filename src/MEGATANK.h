#ifndef MEGATANK_h
#define MEGATANK_h

#include "Arduino.h"
#include "DriverMotor.h"

class MEGATANK {
  public:
    MEGATANK();             // Constructor
    void begin();           // Initializes the motor and pins
    void controlMecanumWheels();  // Controls the Mecanum wheels based on joystick input
    void handleSerialInput();     // Reads and processes serial input

  private:
    int x, y, w, b, v;      // Variables for joystick and control values
    int VW1, VW2, Vy, Vx, V; // Wheel speed and control variables
    motor myMotor;          // Motor object

    // Helper function to map joystick values
    int mapJoystickInput(int value, int center, int outputMin, int outputMax);
};

#endif
