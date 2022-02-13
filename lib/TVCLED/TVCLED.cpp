#include <Arduino.h>
#include <string.h>
#include "TVCLED.h"


TVCLED::TVCLED(int pin1, int pin2, int pin3){
    this->pin1 = pin1;
    this->pin2 = pin2;
    this->pin3 = pin3;
}

void TVCLED::begin(){
    pinMode(this->pin1, OUTPUT);
    pinMode(this->pin2, OUTPUT);
    pinMode(this->pin3, OUTPUT);
}

void TVCLED::changeColor(String color){
    digitalWrite(this->pin1, (int)color[0]-48);
    digitalWrite(this->pin2, (int)color[1]-48);
    digitalWrite(this->pin3, (int)color[2]-48);
}