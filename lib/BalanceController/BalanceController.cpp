#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <Adafruit_BMP085.h>
#include <BalanceController.h>
#include <Arduino.h>
#include <SimpleKalmanFilter.h>

BalanceController::BalanceController(double &x, double &y, double &z){
    this->x = &x;
    this->y = &y;
    this->z = &z;
    this->xkf = new SimpleKalmanFilter(.5, .5, .06);
    this->ykf = new SimpleKalmanFilter(.5, .5, .06);
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

    *this->x = this->xkf->updateEstimate((float)event.acceleration.x);
    *this->y = this->ykf->updateEstimate((float)event.acceleration.y);

    *this->x = (int)(*this->x/this->resolution) * this->resolution;
    *this->y = (int)(*this->y/this->resolution) * this->resolution;

/*
    //Ignore values higher than 15
    if(fabs((double)event.acceleration.x - *this->x) < 15){
        //Subtract value moving out from sum
        this->sumX = this->sumX - this->windowX[this->windowElementX];
        //Append new element in place of old element
        this->windowX[this->windowElementX] = (double)event.acceleration.x;
        //Recalculate the sum
        this->sumX = this->sumX + this->windowX[this->windowElementX];
        //Update the pointer to the next element to remove
        this->windowElementX = (this->windowElementX + 1) % (sizeof(this->windowX)/sizeof(this->windowX[0]));
        //Calculate the new average
        *this->x = this->sumX / (sizeof(this->windowX)/sizeof(this->windowX[0]));
        //Adjust value to our own resolution
        *this->x = (int)(*this->x/this->resolution) * this->resolution;
    }
    
    if(fabs((double)event.acceleration.y - *this->y) < 15){
        this->sumY = this->sumY - this->windowY[this->windowElementY];
        this->windowY[this->windowElementY] = (double)event.acceleration.y;
        this->sumY = this->sumY + this->windowY[this->windowElementY];
        this->windowElementY = (this->windowElementY + 1) % (sizeof(this->windowY)/sizeof(this->windowY[0]));

        *this->y = this->sumY / (sizeof(this->windowY)/sizeof(this->windowY[0]));

        *this->y = (int)(*this->y/this->resolution) * this->resolution;
    }
    */

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