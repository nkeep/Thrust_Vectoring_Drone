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

  /*
  void slowMoveToHover()
  {
    //Slowly moves the servo back to the hover point, hopefully to avoid too much oscillation
    int currentVal = self.read();
    int direction = (currentVal > hoverSetPoint) ? -1 : 1;

    for (int i = 0; i < abs(currentVal - hoverSetPoint); i++)
    {
      currentVal += direction;
      self.write(currentVal);
      delay(30);
    }
  }
  void adjust(float val)
  {
    //Function to make a micro adjustment due to a slight imbalance (should make between 1-3 ticks I think)
    int currentVal = self.read();
    int direction = (val < 0) ? 1 : -1;
    int numTicks = ceil((val - (.1 * direction)) * 2.5);

    for (int i = 0; i < numTicks; i++)
    {
      currentVal += direction;
      self.write(currentVal);
      delay(10);
    }
  }
  */
};
