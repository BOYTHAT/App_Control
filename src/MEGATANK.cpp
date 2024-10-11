#include "MEGATANK.h"

MEGATANK::MEGATANK() {
  // Initialize variables
  x = 128;
  y = 128;
  w = 128;
  b = 0;
  v = 0;
  VW1 = 0;
  VW2 = 0;
  Vy = 0;
  Vx = 0;
  V = 0;
}

void MEGATANK::begin() {
  // Set up motor and pin modes
  myMotor.begin();
  pinMode(4, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  digitalWrite(4, LOW);
}

void MEGATANK::controlMecanumWheels() {
  // Map joystick and control variables
  if (x == 128) {
    Vx = 0;
  } else if (x > 128) {
    Vx = map(x, 128, 255, 0, V);
  } else if (x < 128) {
    Vx = map(x, 128, 0, 0, -V);
  }

  if (y == 128) {
    Vy = 0;
  } else if (y > 128) {
    Vy = map(y, 128, 255, 0, V);
  } else if (y < 128) {
    Vy = map(y, 128, 0, 0, -V);
  }

  if (v > 50) {
    V = map(v, 50, 100, 128, 255);
  } else if (v <= 50) {
    V = map(v, 0, 50, 0, 128);
  }

  if (v == 0) {
    V = 0;
  }

  // Calculate wheel speeds
  VW1 = Vy - Vx;
  VW2 = Vy + Vx;

  VW1 = constrain(VW1, -V, V);
  VW2 = constrain(VW2, -V, V);

  Serial.print("VW1: ");
  Serial.print(VW1);
  Serial.print("  ||  ");
  Serial.print("VW2: ");
  Serial.print(VW2);
  Serial.print("  ||  ");
  Serial.print("V: ");
  Serial.println(V);

  if (V > 0) {
    // Control motors based on calculated speeds
    if (VW1 > 0) myMotor.motor1(1, VW1);
    else if (VW1 == 0) myMotor.motorStop(1);
    else if (VW1 < 0) myMotor.motor1(0, -VW1);

    if (VW2 > 0) myMotor.motor2(1, VW2);
    else if (VW2 == 0) myMotor.motorStop(2);
    else if (VW2 < 0) myMotor.motor2(0, -VW2);
  }
}

void MEGATANK::handleSerialInput() {
  static String received = "";    // Buffer to store incoming data
  static bool receiving = false;  // Flag to indicate if we are receiving a message

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
            if (b == 1) {
              Serial.println("A");
              digitalWrite(6, HIGH);
            } else if (b == 2) {
              Serial.println("B");
              digitalWrite(7, HIGH);
            } else if (b == 3) {
              Serial.println("C");
              digitalWrite(6, LOW);
            } else if (b == 4) {
              Serial.println("D");
              digitalWrite(7, LOW);
            }
            break;
          case 'v':
            v = value;
            Serial.print("v: ");
            Serial.println(v);
            break;
          case '\t':
            received = "";
            receiving = false;
            continue;
        }
        received = "";
      }
    } else {
      received += c;
      receiving = true;
    }
  }
  controlMecanumWheels();
}
