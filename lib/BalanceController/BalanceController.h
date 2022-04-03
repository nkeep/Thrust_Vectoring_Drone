#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <Adafruit_BMP085.h>
#include <SimpleKalmanFilter.h>

class BalanceController
{
public:
    float resolution = 0.4;
    float prevXVal;
    float prevYVal;
    float prevAlt;
    float floorHeight;
    float hoverHeight;
    double *x, *y, *z;
    float windowX[5] = {1, 1, 1, 1, 1};
    int windowElementX = 0;
    float sumX = 5;
    float windowY[5] = {1, 1, 1, 1, 1};
    int windowElementY = 0;
    float sumY = 5;
    Adafruit_ADXL345_Unified accel;
    Adafruit_BMP085 bmp;
    SimpleKalmanFilter *xkf;
    SimpleKalmanFilter *ykf;


    BalanceController(double &x, double &y, double &z);

    void begin();

    void readAccelerometerValues();
};