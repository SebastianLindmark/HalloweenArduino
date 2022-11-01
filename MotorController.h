#include <Arduino.h>

class MotorController {
  
  public:
  
    MotorController(int motorEnable, int motorPositive, int motorNegative);
    
    void moveMotorsDownFor(int32_t time);

    void moveMotorsUpFor(int32_t time);

    void moveMotorDirectionFor(bool rollDown, int32_t time);

  private:

    int motorEnablePin;

    int motorPositivePin;

    int motorNegativePin;

};




