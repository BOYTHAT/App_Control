#include "MEGATANK.h"

MEGATANK myRobot;

void setup() {
  Serial.begin(9600);
  myRobot.begin();
}

void loop() {
  myRobot.handleSerialInput();
}
