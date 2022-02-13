#include <Servo.h>

class TVCServo
{
public:
  Servo self;
  int pin, hoverSetPoint;
  TVCServo(int pin, int hoverValue);

  void begin();

  void moveTo(int loc);

  void moveToHover();

  void moveForward();
  void moveBackward();

};
