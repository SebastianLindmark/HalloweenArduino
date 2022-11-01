#include <movingAvg.h> 
#include <IRremote.h>
#include "MotorController.h"
#include "ObjectDetector.h"

#define echoPin 2 
#define trigPin 3 
#define usPowerPin 4 

#define irPowerPin 11
#define irGroundPin 12
#define irDataPin 7

#define motorEnablePin 6
#define motorPositivePin 8
#define motorNegativePin 9

#define roomCalibrationAvgCount 30
#define distanceAvgCount 1

int32_t motorRolldownValue = 1000;
int32_t motorRollupValue = 1000;
double detectionSensitivity = 0.5;


long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement
bool calibration = true;

MotorController motorController(motorEnablePin, motorPositivePin, motorNegativePin);
ObjectDetector objectDetector(trigPin,echoPin);

movingAvg roomCalibrationMovingAvg(roomCalibrationAvgCount);
movingAvg currentDistanceMovingAvg(distanceAvgCount);

IRrecv irrecv(irDataPin);
decode_results signals;

void setup() {

  Serial.begin(9600); 

  pinMode(usPowerPin, OUTPUT); //used as a 5v supplier pin
  digitalWrite(usPowerPin, HIGH);

  pinMode(irPowerPin, OUTPUT);
  digitalWrite(irPowerPin, HIGH);
  irrecv.enableIRIn();

  objectDetector.setCallback(&callback);
}


void callback(){
  scare();
  objectDetector.calibrate();
}

void loop() {
 
  objectDetector.read();


  if (irrecv.decode(&signals)) {
        Serial.println(signals.value, HEX);
        handleIRCommand(signals.value);
        irrecv.resume(); // get the next signal
  }
}

void handleIRCommand(uint32_t command){

  switch(command){
    
    // Activate
    // Middle button
    case 0xFF02FD:
      scare();
    break;

    // Calibration motor up
    // Arrow up
    case 0xFF629D:
      motorController.moveMotorsUpFor(100);
      //moveMotorsUpFor(100);
    break;

    // Calibration motor down
    // Arrow down
    case 0xFFA857:
      motorController.moveMotorsDownFor(100);
      //moveMotorsDownFor(100);
    break;

    // Increase roll down length
    // 1 Button
    case 0xFF6897:
      motorRolldownValue += 50;
    break;

    // Decrease roll down length
    // 2 Button
    case 0xFF9867:
      motorRolldownValue -= 50;
    break;


    // Increase roll up length
    // 4 Button
    case 0xFF30CF:
      motorRollupValue += 50;
    break;

    // Decrease roll up length
    // 5 Button
    case 0xFF18E7:
      motorRollupValue -= 50;
    break;

    // Increase sensitivity
    // 7 Button
    case 0xFF10EF:
      objectDetector.changeSensitivity(0.05);
    break;

    // Decrease sensitivity
    // 8 Button
    case 0xFF38C7:
    objectDetector.changeSensitivity(-0.05);
    break;

  }
}

void scare(){
  moveMotors();
  delay(3000);

}

void moveMotors(){
  motorController.moveMotorsDownFor(motorRolldownValue);
  delay(2500);
  motorController.moveMotorsUpFor(motorRollupValue);
}
