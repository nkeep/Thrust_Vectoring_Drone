#include <Arduino.h>
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

int upLED = 9;
int rightLED = 10;
int downLED = 11;
int leftLED = 12;
int servX = 2;
int servY = 3;
bool allowControls;
Servo servoX;
Servo servoY;
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified();

void setup() {
    Serial.begin(9600);

    pinMode(upLED, OUTPUT);
    pinMode(rightLED, OUTPUT);
    pinMode(downLED, OUTPUT);
    pinMode(leftLED, OUTPUT);
    digitalWrite(upLED, LOW);
    digitalWrite(rightLED, LOW);
    digitalWrite(downLED, LOW);
    digitalWrite(leftLED, LOW);

    servoX.attach(servX);
    servoY.attach(servY);
    if(!accel.begin())
    {
      Serial.println("No ADXL345 sensor detected.");
      while(1);
    }
}

void loop(){
    delay(30);
    sensors_event_t event; 
    accel.getEvent(&event);
    String t;

    while(Serial.available()){
        t += (char)Serial.read();
        Serial.print(t);
    }
//    Serial.print("t value:" + t + "\n");
    //Serial.print("somethign is happening");
    Serial.print(abs(event.acceleration.x)); Serial.print("\n");
    Serial.print(abs(event.acceleration.y)); Serial.print("\n");

    if(abs(event.acceleration.x) < 3 && abs(event.acceleration.y) < 3){
      allowControls = true;
    }
    else{
      allowControls = false;
    }

    if(allowControls){
      if(t == "0"){
        servoX.write(40);
        servoY.write(40);
      }else if(t == "1"){
        servoY.write(80);
      }else if(t == "4"){
        servoY.write(0);
      }else if(t == "2"){
        servoX.write(80);
      }else if(t == "3"){
        servoX.write(0);
      }
    }
    else{
      //Algorithm to stabilize
      if(abs(event.acceleration.x) > 3){
        servoX.write(40 + 4*event.acceleration.x);
      }
      if(abs(event.acceleration.y) > 3){
        servoY.write(40 + 4*event.acceleration.y); 
      }
    }
}
