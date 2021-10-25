#include <Servo.h>

class TVCMotor
{
public:
    Servo cwESC;
    Servo ccwESC;
    int maxValue;
    int hoverValue;
    int currentSpeed;
    TVCMotor(int pin1, int pin2)
    {
        cwESC.attach(pin1);
        ccwESC.attach(pin2);
    }
    void setSpeed(int val)
    {
        cwESC.write(val);
        ccwESC.write(val);
        currentSpeed = val;
    }
    void increaseSpeed(int ticks = 2)
    {
        //Increase the speed by a little
        for (int i = 0; i < ticks; i++)
        {
            cwESC.write(currentSpeed + i + 1);
            ccwESC.write(currentSpeed + i + 1);
            delay(10);
        }
        currentSpeed = cwESC.read();
    }
    void lowerSpeed(int ticks = 2)
    {
        //Lower speed by a little
        for (int i = 0; i < ticks; i++)
        {
            cwESC.write(currentSpeed - i - 1);
            ccwESC.write(currentSpeed - i - 1);
            delay(10);
        }
        currentSpeed = cwESC.read();
    }
};