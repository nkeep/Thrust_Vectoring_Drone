#include <Servo.h>
#include <Arduino.h>

class TVCServo
{
public:
  Servo self;
  int pin, hoverSetPoint, currentVal;
  String id;
  TVCServo(int pin, int hoverValue, String id);

  void begin();
  void calibrate(int deg);

  void moveTo(int loc);

  void moveToHover();

  void moveForward();
  void moveBackward();

};
