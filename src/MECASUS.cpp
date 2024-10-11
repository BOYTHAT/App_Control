#include "MECASUS.h"

MECASUS::MECASUS() {
  // Initialize variables
  x = 128;
  y = 128;
  w = 128;
  b = 0;
  v = 0;
  VW1 = 0;
  VW2 = 0;
  VW3 = 0;
  VW4 = 0;
}

void MECASUS::begin() {
  // Set up motor and pin modes
  myMotor.begin();
  pinMode(4, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  digitalWrite(4, LOW);
}

void MECASUS::controlMecanumWheels() {
  // Calculate joystick and rotation values
  int Vy = adjustJoystickInput(y, 128);
  int Vx = adjustJoystickInput(x, 128);
  int W = adjustJoystickInput(w, 128);

  // Calculate the speeds for each wheel based on Vw_Right and Vw_Left
  VW1 = Vy - Vx - W;
  VW2 = Vy + Vx + W;
  VW3 = Vy - Vx + W;
  VW4 = Vy + Vx - W;

  // Constrain wheel speeds to valid PWM range
  VW1 = constrain(VW1, -255, 255);
  VW2 = constrain(VW2, -255, 255);
  VW3 = constrain(VW3, -255, 255);
  VW4 = constrain(VW4, -255, 255);

  // Control motors based on calculated speeds
  if (VW1 > 0) myMotor.motor1(1, VW1);
  else if (VW1 < 0) myMotor.motor1(0, -VW1);
  else myMotor.motorStop(1);

  if (VW2 > 0) myMotor.motor2(1, VW2);
  else if (VW2 < 0) myMotor.motor2(0, -VW2);
  else myMotor.motorStop(2);

  if (VW3 > 0) myMotor.motor3(1, VW3);
  else if (VW3 < 0) myMotor.motor3(0, -VW3);
  else myMotor.motorStop(3);

  if (VW4 > 0) myMotor.motor4(1, VW4);
  else if (VW4 < 0) myMotor.motor4(0, -VW4);
  else myMotor.motorStop(4);
}

int MECASUS::adjustJoystickInput(int value, int center) {
  if (value == center) {
    return 0;
  } else if (value > center) {
    return map(value, center, 255, 0, 255);
  } else {
    return map(value, center, 0, 0, -255);
  }
}

void MECASUS::handleSerialInput() {
  static String received = "";
  static bool receiving = false;

  while (Serial.available()) {
    char c = Serial.read();
    if (c == 'x' || c == 'y' || c == 'w' || c == 'b' || c == 'v' || c == '\t') {
      if (receiving) {
        int value = atoi(received.c_str());

        switch (c) {
          case 'x': x = value; break;
          case 'y': y = value; break;
          case 'w': w = value; break;
          case 'b':
            b = value;
            if (b == 1) digitalWrite(6, HIGH);
            else if (b == 2) digitalWrite(7, HIGH);
            else if (b == 3) digitalWrite(6, LOW);
            else if (b == 4) digitalWrite(7, LOW);
            break;
          case 'v': v = value; break;
          case '\t': received = ""; receiving = false; continue;
        }

        received = "";
      }
    } else {
      received += c;
      receiving = true;
    }
  }

  // Update motor speeds based on joystick input
  controlMecanumWheels();
}
