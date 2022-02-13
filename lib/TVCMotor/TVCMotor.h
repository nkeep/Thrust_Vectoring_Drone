#include <Servo.h>

class TVCMotor
{
public:
    int xPin;
    int yPin;
    Servo cwESC;
    Servo ccwESC;
    int maxValue;
    int hoverValue;
    int currentSpeed = 0;
    
    TVCMotor(int pin1, int pin2);
    void begin();
    void setSpeed(int val);
    void increaseSpeed(int ticks = 2);
    void decreaseSpeed(int ticks = 2);
    void kill();
};