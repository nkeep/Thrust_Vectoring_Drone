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
    double *x, *y, *z;
    Adafruit_ADXL345_Unified accel;
    Adafruit_BMP085 bmp;

    BalanceController(double &x, double &y, double &z);

    void begin();

    void readAccelerometerValues();
};