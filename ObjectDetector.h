#include <Arduino.h>
#include <movingAvg.h>

class ObjectDetector {


public:
  ObjectDetector(int triggerPin, int echoPin) : triggerPin(triggerPin),
                                                echoPin(echoPin),
                                                roomCalibrationMovingAvg(roomCalibrationAvgCount), 
                                                currentDistanceMovingAvg(distanceAvgCount){
    
    pinMode(triggerPin, OUTPUT); // Sets the trigPin as an OUTPUT
    pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
    
    roomCalibrationMovingAvg.begin();
    currentDistanceMovingAvg.begin();
  }

  void setCallback(void (*func)());

  void changeSensitivity(double change);

  void calibrate();

  void read();


private:
  void (*detectionCallback)();

  const int roomCalibrationAvgCount = 30;
  const int distanceAvgCount = 1;
  movingAvg roomCalibrationMovingAvg; 
  movingAvg currentDistanceMovingAvg; 

  bool inTargetDistance(int baselineDistance, int currentDistance);
  void waitForAverageRoomDistance();
  int readDistance();
  int baselineDistance;

  double detectionSensitivity = 0.5;
  bool calibration = true;

  long duration;  // variable for the duration of sound wave travel

  int triggerPin;
  int echoPin;




};