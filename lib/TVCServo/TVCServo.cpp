#include <Servo.h>
#include <TVCServo.h>

TVCServo::TVCServo(int pin, int hoverValue){
    this->pin = pin;
    this->hoverSetPoint = hoverValue;
}

void TVCServo::begin(){
    this->self.attach(this->pin);
    this->self.write(this->hoverSetPoint);

}

void TVCServo::moveTo(int loc){
    this->self.write(loc);
}

void TVCServo::moveToHover(){
    this->self.write(hoverSetPoint);
}

void TVCServo::moveForward(){
    this->self.write(this->hoverSetPoint + 15);
}

void TVCServo::moveBackward(){
    this->self.write(this->hoverSetPoint - 15);
}