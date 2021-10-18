#include <Servo.h>

class TVCServo{
  public:
    Servo self;
    int maxLeft;
    int maxRight;
    int hoverSetPoint;
    void moveTo;
    TVCServo(int pin, int maxDegree){
      self.attach(pin);
      self.write(maxDegree/2);
    }
}
