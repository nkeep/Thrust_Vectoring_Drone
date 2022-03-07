#include <ESC.h>
#include <Arduino.h>
#include <TVCMotor.h>

TVCMotor::TVCMotor(int pin1, int pin2){
    this->cwPin = pin1;
    this->ccwPin = pin2;
}

void TVCMotor::begin(){
    this->cwESC = new ESC(this->cwPin, 1100, 1550, 500);
    this->ccwESC = new ESC(this->ccwPin, 1100, 1550, 500);
    // this->cwESC->calib();
    // this->cwESC->stop();
    this->cwESC->arm();
    // this->ccwESC->calib();
    // this->ccwESC->stop();
    this->ccwESC->arm();

    this->cwESC->speed(1550);
    this->ccwESC->speed(1550);
    delay(4000);
    this->cwESC->speed(1100);
    this->ccwESC->speed(1100);
    delay(4000);
}

void TVCMotor::setSpeed(int val){
    this->cwESC->speed(val);
    this->ccwESC->speed(val);
    this->currentSpeed = val;
}

void TVCMotor::increaseSpeed(int ticks = 2){
    //Increase the speed by a little
    for (int i = 0; i < ticks; i++)
    {
        this->cwESC->speed(this->currentSpeed + (i + 1)*5);
        this->ccwESC->speed(this->currentSpeed + (i + 1)*5);
        delay(10);
    }
    this->currentSpeed = this->currentSpeed + ticks*5;
    Serial.print("Speed: "); Serial.println(this->currentSpeed);
}

void TVCMotor::decreaseSpeed(int ticks = 2){
    //Decrease the speed by a little
    for (int i = 0; i < ticks; i++)
    {
        this->cwESC->speed(this->currentSpeed - (i - 1)*5);
        this->ccwESC->speed(this->currentSpeed - (i - 1)*5);
        delay(10);
    }
    this->currentSpeed = this->currentSpeed - ticks*5;
    Serial.print("Speed: "); Serial.println(this->currentSpeed);
}



void TVCMotor::kill(){
    this->ccwESC->stop();
    this->cwESC->stop();
}