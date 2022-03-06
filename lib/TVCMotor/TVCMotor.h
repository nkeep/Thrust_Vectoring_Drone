#include <ESC.h>

class TVCMotor
{
public:
    int cwPin;
    int ccwPin;
    ESC *cwESC;
    ESC *ccwESC;
    int maxValue;
    int hoverValue;
    int currentSpeed = 1000;
    int cwSpeed = 1000;
    int ccwSpeed = 1000;
    
    TVCMotor(int pin1, int pin2);
    void begin();
    void setSpeed(int val);
    void increaseSpeed(int ticks = 2);
    void decreaseSpeed(int ticks = 2);
    void increaseSpeedCW(int ticks =2);
    void increaseSpeedCCW(int ticks =2);
    void decreaseSpeedCW(int ticks =2);
    void decreaseSpeedCCW(int ticks =2);
    void kill();
};