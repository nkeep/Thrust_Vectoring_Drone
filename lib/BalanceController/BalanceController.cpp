#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <Adafruit_BMP085.h>
#include <BalanceController.h>
#include <Arduino.h>

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
    sensors_event_t event;
    this->accel.getEvent(&event);

    this->sumX = this->sumX - this->windowX[this->windowElementX];
    this->windowX[this->windowElementX] = (double)event.acceleration.x;
    this->sumX = this->sumX + this->windowX[this->windowElementX];
    this->windowElementX = (this->windowElementX + 1) % (sizeof(this->windowX)/sizeof(this->windowX[0]));

    *this->x = this->sumX / sizeof(this->windowX);

    this->sumY = this->sumY - this->windowY[this->windowElementY];
    this->windowY[this->windowElementY] = (double)event.acceleration.y;
    this->sumY = this->sumY + this->windowY[this->windowElementY];
    this->windowElementY = (this->windowElementY + 1) % (sizeof(this->windowY)/sizeof(this->windowY[0]));

    *this->y = this->sumY / sizeof(this->windowY);

    *this->x = floor(abs((double)*this->x/this->resolution)) * this->resolution;
    *this->y = floor(abs((double)*this->y/this->resolution)) * this->resolution;

    // for(int i = 0; i < 5; i++){
    //     sensors_event_t event;
    //     this->accel.getEvent(&event);

    //     *this->x += (double)event.acceleration.x;
    //     *this->y += (double)event.acceleration.y;
    //     *this->z += (double)event.acceleration.z;
    //     delay(5);
    // }
    // *this->x = floor(*this->x/5.0/this->resolution) * this->resolution;
    // *this->y = floor(*this->y/5.0/this->resolution) * this->resolution;
    // *this->z /= 5;

    // sensors_event_t event;
    // this->accel.getEvent(&event);
    // *this->x = floor((double)event.acceleration.x/this->resolution) * this->resolution;
    // *this->y = floor((double)event.acceleration.y/this->resolution) * this->resolution;
    // *this->z = (double)event.acceleration.z;
    Serial.print("Y: "); Serial.println(*this->y);
    Serial.print("X: "); Serial.println(*this->x);


}