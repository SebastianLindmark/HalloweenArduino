#include "ObjectDetector.h"



void ObjectDetector::setCallback(void (*func)()){
  detectionCallback = func;
}


void ObjectDetector::changeSensitivity(double change){
  detectionSensitivity += change;
}

void ObjectDetector::calibrate(){
  roomCalibrationMovingAvg.reset();
  calibration = true;
}

void ObjectDetector::read(){

  int currentDistance = readDistance();
  int roomCalibrationAvgDistance = roomCalibrationMovingAvg.reading(currentDistance);
  int avgCurrentDistance = currentDistanceMovingAvg.reading(currentDistance);

  

  if(calibration){
    return waitForAverageRoomDistance();
  }


  if(inTargetDistance(roomCalibrationAvgDistance, avgCurrentDistance)){
    detectionCallback();
  }

}

void ObjectDetector::waitForAverageRoomDistance(){
    Serial.println("Calibrating room");  
    calibration = roomCalibrationAvgCount > roomCalibrationMovingAvg.getCount();
    return;
}

int ObjectDetector::readDistance(){
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
}


bool ObjectDetector::inTargetDistance(int baselineDistance, int currentDistance){
  return baselineDistance * detectionSensitivity > currentDistance;
}


