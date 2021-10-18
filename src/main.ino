#include <Arduino.h>
#include <TVCServo.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

int upLED = 7;
int rightLED = 8;
int downLED = 9;
int leftLED = 10;
int servX = 2;
int servY = 3;
bool allowControls;
TVCServo servoX;
TVCServo servoY;
// Servo servoX;
// Servo servoY;
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
    // servoX.startPoint(40);
    // servoY.startPoint(40);
    servoX.self.attach(2);
    servoY.self.attach(3);

  //  servoX.attach(servX);
  //  servoY.attach(servY);
   servoX.moveTo(90);
   servoY.moveTo(90);
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
        servoX.moveTo(40);
        servoY.moveTo(40);
      }else if(t == "1"){
        servoY.moveTo(80);
      }else if(t == "4"){
        servoY.moveTo(0);
      }else if(t == "2"){
        servoX.moveTo(80);
      }else if(t == "3"){
        servoX.moveTo(0);
      }
    }
    else{
      //Algorithm to stabilize
      if(abs(event.acceleration.x) > 3){
        servoX.moveTo(40 + 4*event.acceleration.x);
      }
      if(abs(event.acceleration.y) > 3){
        servoY.moveTo(40 + 4*event.acceleration.y); 
      }
    }
}
