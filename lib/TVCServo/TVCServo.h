#include <Servo.h>

class TVCServo
{
public:
  Servo self;
  int pin, hoverSetPoint, currentVal;
  TVCServo(int pin, int hoverValue);

  void begin();
  void calibrate(int deg);

  void moveTo(int loc);

  void moveToHover();

  void moveForward();
  void moveBackward();

};
