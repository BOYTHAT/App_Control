#ifndef MECASUS_h
#define MECASUS_h

#include "Arduino.h"
#include "DriverMotor.h"

class MECASUS {
  public:
    MECASUS();
    
    // Initializes motors and pins
    void begin();
    
    // Processes joystick input and controls the Mecanum wheels
    void controlMecanumWheels();
    
    // Reads input from serial communication and updates control values
    void handleSerialInput();
    
  private:
    int x, y, w, b, v, VW1, VW2, VW3, VW4;
    motor myMotor;

    // Helper functions to calculate wheel speeds
    int adjustJoystickInput(int value, int center);
};

#endif
