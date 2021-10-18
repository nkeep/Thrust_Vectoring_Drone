#include <Servo.h>

class TVCServo{
  public:
    Servo self;
    int maxLeft;
    int maxRight;
    int hoverSetPoint;
    // TVCServo(int pin){
    //   self.attach(pin);
    // }

    void moveTo(int loc){
      self.write(loc);
    }
};
