#include <Servo.h>
#include <Arduino.h>
#include <TVCMotor.h>


TVCMotor::TVCMotor(int pin1, int pin2){
    this->xPin = pin1;
    this->yPin = pin2;
}

void TVCMotor::begin(){
    this->cwESC.attach(this->xPin, 1000, 2000);
    this->ccwESC.attach(this->yPin, 1000, 2000);
}

void TVCMotor::setSpeed(int val){
    this->cwESC.write(val);
    this->ccwESC.write(val);
    this->currentSpeed = val;
}

void TVCMotor::increaseSpeed(int ticks = 2){
    //Increase the speed by a little
    for (int i = 0; i < ticks; i++)
    {
        this->cwESC.write(this->currentSpeed + i + 1);
        this->ccwESC.write(this->currentSpeed + i + 1);
        delay(10);
    }
    this->currentSpeed = this->cwESC.read();
}

void TVCMotor::decreaseSpeed(int ticks = 2){
    //Decrease the speed by a little
    for (int i = 0; i < ticks; i++)
    {
        this->cwESC.write(this->currentSpeed - i - 1);
        this->ccwESC.write(this->currentSpeed - i - 1);
        delay(10);
    }
    this->currentSpeed = this->cwESC.read();
}

void TVCMotor::kill(){
    this->ccwESC.write(0);
    this->cwESC.write(0);
}