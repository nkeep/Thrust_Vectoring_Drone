#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <Adafruit_BMP085.h>

class BalanceController
{
public:
    int numBalances = 0;
    float prevXVal;
    float prevYVal;
    float prevAlt;
    float floorHeight;
    float hoverHeight;
    Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified();
    Adafruit_BMP085 bmp;

    BalanceController()
    {
        floorHeight = bmp.readAltitude();
        hoverHeight = floorHeight + 50;
    }

    void readAccelerometerValues(float &x, float &y)
    {
        sensors_event_t event;
        accel.getEvent(&event);
        x = event.acceleration.x;
        y = event.acceleration.y;
    }

    int detectImbalance(float &x, float &y) //Returns values based on how big the difference is from past acceleromter readings
    {
        sensors_event_t event;
        accel.getEvent(&event);
        float diffX = event.acceleration.x - prevXVal;
        float diffY = event.acceleration.y - prevYVal;
        prevXVal = event.acceleration.x;
        prevYVal = event.acceleration.y;
        x = diffX;
        y = diffY;
        if (abs(diffX) > .5 || abs(diffY) > .5)
        {
            return 2; //Critical
        }
        else if (abs(diffX) > .3 || abs(diffY) > .3)
        {
            return 1; //Minor
        }
        else
        {
            return 0; //Little to none
        }
    }
    int detectHeightImbalance()
    {
        float currentAlt = bmp.readAltitude();
        if (abs(prevAlt - currentAlt) > 5) //Rising or falling at a very high rate
        {
            return 4;
        }
        else if (currentAlt < (hoverHeight - 10)) //We are well below where we want to be
        {
            return 3;
        }
        else if (currentAlt > (hoverHeight + 10)) //We are well above where we want to be
        {
            return 2;
        }
        else if (currentAlt > 16000) //We are higher than our maximum height
        {
            return 1;
        }
        else //We are within the hover range
        {
            return 0;
        }
    }
    bool balance(float &x, float &y)
    {
        sensors_event_t event;
        accel.getEvent(&event);
        accelX = event.acceleration.x;
        accelY = event.acceleration.y;
        x = 40 + -(accelX * 4);
        y = 40 + -(accelY * 4);
        if (abs(accelX) < .25 && abs(accelY) < .25)
        {
            //We are near a stable state
            numBalances += 1;
        }
        else
        {
            numBalances = 0;
        }
        if (numBalances >= 5) //We need 5 consecutive stable readings to decide we are balanced
        {
            numBalances = 0;
            return true;
        }
        else
        {
            return false;
        }
    }
    float readHeight()
    {
        return bmp.readAltitude();
    }
    int balanceHeight() //Will loop and once we have been inside the range for 10 loops, we decide we are balanced
    {
        float currentAlt = bmp.readAltitude();
        if (numBalances >= 10) //We have achieved 10 balances
        {
            numBalances = 0;
            return 0;
        }
        if (currentAlt < (hoverHeight + 10) && currentAlt > (hoverHeight - 10))
        {
            numBalances += 1;
            return 3;
        }
        else if (currentAlt > (hoverHeight + 10))
        {
            numBalances = 0;
            return 2;
        }
        else if (currentAlt < (hoverHeight - 10))
        {
            numBalances = 0;
            return 1;
        }
    }
};