#include <Servo.h>
#include <TVCServo.h>
#include <Arduino.h>

TVCServo::TVCServo(int pin, int hoverValue, String id){
    this->pin = pin;
    this->hoverSetPoint = hoverValue;
    this->currentVal = hoverValue;
    this->id = id;
}

void TVCServo::begin(){
    this->self.attach(this->pin);
    this->self.write(this->hoverSetPoint);
}

void TVCServo::calibrate(int deg){
    this->self.write(this->hoverSetPoint + deg);
    this->hoverSetPoint = this->self.read();
    Serial.print(this->id + " Calib: ");Serial.println(this->self.read());
}

void TVCServo::moveTo(int loc){
    if(this->currentVal != this->hoverSetPoint + loc){ //If it's the same value, don't do anything
        //I wrote this for loop in an attempt to smooth the gimbals by only moving the servos 1 tick at a time.
        int direction = 1 ? (this->currentVal - this->hoverSetPoint) < loc : -1; // 1 = positive, -1 = negative direction
        for(int i = 1; i < abs(loc - (this->currentVal - this->hoverSetPoint)) + 1; i++){
            this->self.write(this->currentVal + i*direction);
        }
        this->currentVal = this->hoverSetPoint + loc;

        Serial.print(this->id + " Servo: "); Serial.println(loc);

        //Serial.println("new val");
        // this->self.write(this->hoverSetPoint + loc);
        // this->currentVal = (this->hoverSetPoint + loc);
    }
}

void TVCServo::moveToHover(){
    this->self.write(hoverSetPoint);
}

void TVCServo::moveForward(){
    this->self.write(this->hoverSetPoint + 12);
}

void TVCServo::moveBackward(){
    this->self.write(this->hoverSetPoint - 12);
}