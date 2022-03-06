#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <Adafruit_BMP085.h>
#include <BalanceController.h>

BalanceController::BalanceController(double &x, double &y, double &z){
    this->x = &x;
    this->y = &y;
    this->z = &z;
}

void BalanceController::begin(){
    this->accel = Adafruit_ADXL345_Unified();
}

void BalanceController::readAccelerometerValues(){
    if(!accel.begin()){
        while(1);
    }

    for(int i = 0; i < 5; i++){
        sensors_event_t event;
        this->accel.getEvent(&event);

        *this->x += (double)event.acceleration.x;
        *this->y += (double)event.acceleration.y;
        *this->z += (double)event.acceleration.z;
        delay(5);
    }
    *this->x /= 5;
    *this->y /= 5;
    *this->z /= 5;
}