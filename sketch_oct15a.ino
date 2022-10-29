#include <movingAvg.h> 
#include <IRremote.h>

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

movingAvg roomCalibrationMovingAvg(roomCalibrationAvgCount);
movingAvg currentDistanceMovingAvg(distanceAvgCount);

IRrecv irrecv(irDataPin);
decode_results signals;

void setup() {

  Serial.begin(9600); 

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  pinMode(usPowerPin, OUTPUT);

  pinMode(irPowerPin, OUTPUT);

  pinMode(motorEnablePin, OUTPUT);
  pinMode(motorPositivePin, OUTPUT);
  pinMode(motorNegativePin, OUTPUT);


  roomCalibrationMovingAvg.begin();
  currentDistanceMovingAvg.begin();

  digitalWrite(usPowerPin, HIGH);

  digitalWrite(irPowerPin, HIGH);
  irrecv.enableIRIn();

  digitalWrite(motorEnablePin, LOW);
  digitalWrite(motorPositivePin, LOW);
  digitalWrite(motorNegativePin, LOW);

}

void loop() {
 
  int currentDistance = readCurrentDistance();

  int roomCalibrationAvgDistance = roomCalibrationMovingAvg.reading(currentDistance);
  int avgCurrentDistance = currentDistanceMovingAvg.reading(currentDistance);

  if(calibration){
    Serial.println("Calibrating room");   
    calibration = roomCalibrationAvgCount > roomCalibrationMovingAvg.getCount();
    return;
  }

  if (irrecv.decode(&signals)) {
        Serial.println(signals.value, HEX);
        handleIRCommand(signals.value);
        irrecv.resume(); // get the next signal
  }
  

  if(inTargetDistance(roomCalibrationAvgDistance, avgCurrentDistance)){
    scare();
  }

}


int readCurrentDistance(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
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
      moveMotorsUpFor(100);
    break;

    // Calibration motor down
    // Arrow down
    case 0xFFA857:
      moveMotorsDownFor(100);
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
      detectionSensitivity += 0.05;
    break;

    // Decrease sensitivity
    // 8 Button
    case 0xFF38C7:
      detectionSensitivity -= 0.05;
    break;

  }


}

void scare(){
  moveMotors();
  roomCalibrationMovingAvg.reset();
  calibration = true;
  delay(3000);
}

void moveMotors(){
  
  moveMotorsDownFor(motorRolldownValue);
  delay(2500);
  moveMotorsUpFor(motorRollupValue);
}

void moveMotorsDownFor(int32_t time){
  moveMotorDirectionFor(true, time);
}

void moveMotorsUpFor(int32_t time){
  moveMotorDirectionFor(false, time);
}

void moveMotorDirectionFor(bool rollDown, int32_t time){
  digitalWrite(motorEnablePin, HIGH);
  digitalWrite(motorPositivePin, !rollDown);
  digitalWrite(motorNegativePin, rollDown);

  delay(time);
  digitalWrite(motorEnablePin, LOW);
  digitalWrite(motorPositivePin, LOW);
  digitalWrite(motorNegativePin, LOW);
}



bool inTargetDistance(int baselineDistance, int currentDistance){
  return baselineDistance * detectionSensitivity > currentDistance;
}
