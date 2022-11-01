#include "MotorController.h"


MotorController::MotorController(int motorEnable, int motorPositive, int motorNegative){
  motorEnablePin = motorEnable;
  motorPositivePin = motorPositive;
  motorNegativePin = motorNegative;

  pinMode(motorEnablePin, OUTPUT);
  pinMode(motorPositivePin, OUTPUT);
  pinMode(motorNegativePin, OUTPUT);

}


void MotorController::moveMotorsDownFor(int32_t time){
  moveMotorDirectionFor(true, time);
}

void MotorController::moveMotorsUpFor(int32_t time){
  moveMotorDirectionFor(false, time);
}

void MotorController::moveMotorDirectionFor(bool rollDown, int32_t time){
  digitalWrite(motorEnablePin, HIGH);
  digitalWrite(motorPositivePin, !rollDown);
  digitalWrite(motorNegativePin, rollDown);

  delay(time);
  digitalWrite(motorEnablePin, LOW);
  digitalWrite(motorPositivePin, LOW);
  digitalWrite(motorNegativePin, LOW);
}


