#include <Arduino.h>
#include <TVCServo.h>
#include <TVCMotor.h>
#include <BalanceController.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <TVCLED.h>
#include <TVCPID.h>
#include <Ewma.h>
#include <TVCBuzzer.h>

bool allowControls = true;
bool isControlled = false;
bool hasTakenOff = false;
bool inCalibration = true;
int t = -1;


double x, y, z, height;
int esc1_m = 11;
int esc2_m = 12;
int rLED = 5;
int gLED = 6;
int bLED = 7;
int buzzerPin = 8;
int servoXPin = 3;
int servoYPin = 4;
int batteryPin = A3;

String readString;
int readX;
int readY;

TVCServo *servoX;
TVCServo *servoY;
BalanceController *balanceController;
TVCMotor *motors;
TVCLED *LED;
TVCPID *PID;
TVCBuzzer *buzzer;
double outputX, outputY;
Ewma xFilter(0.6);
Ewma yFilter(0.6);

void setup()
{
  Serial.begin(9600);

  balanceController = new BalanceController(x,y,z);
  balanceController->begin();

  // balanceController->readAccelerometerValues();
  
  // PID = new TVCPID(x, y, &outputX, &outputY, &x, &y);
  // PID->begin();

  LED = new TVCLED(rLED, gLED, bLED);
  LED->begin();

  motors = new TVCMotor(esc1_m, esc2_m);
  motors->begin();

  servoX = new TVCServo(servoXPin, 124, "X"); //80, 115
  servoX->begin();

  servoY = new TVCServo(servoYPin, 82, "Y"); //67, 87
  servoY->begin();

  buzzer = new TVCBuzzer(buzzerPin);


  int batteryValue = analogRead(batteryPin);
  Serial.print("battery value: ");
  Serial.println(batteryValue);
  // if(batteryValue < 400){
  //   buzzer->playIndianaJones();
  // }

}

void loop()
{
  delay(50);
  LED->changeColor(RED);
  readString = "";
  bool readServos = false; // Tells us to read the Y value if X is read during the loop

  while(inCalibration){ // Loop for calibrating servos
    while(Serial.available()){
      char c = Serial.read();
      if(c == '1'){ // Left
        servoX->calibrate(-1);
      } else if(c == '2'){ // Right
        servoX->calibrate(1);
      } else if(c == '3'){ // Up
        servoY->calibrate(1);
      } else if(c == '4'){ // Down
        servoY->calibrate(-1);
      } else if(c == 'C'){
        continue;
      } else if(c == 'S'){
        Serial.println("End of the calibration phase");
        balanceController->readAccelerometerValues();
        PID = new TVCPID(0, 0, &outputX, &outputY, &x, &y);
        PID->begin();
        inCalibration = false;
        break;
      }
    }
  }

  while(Serial.available()) //General loop on controller page of app
  {
    char c = Serial.read();
    switch(c){
      case ':':
        readX = readString.toInt();
        readString = "";
        readServos = true;
        break;
      case 'R':
        motors->increaseSpeed();
        break;
      case 'L':
        motors->decreaseSpeed();
        break;
      case 'T':
        takeOff();
        buzzer->playIndianaJones();
        break;
      case 'D':
        isControlled = false;
        break;
      case ',':
        if(readServos){
        readY = readString.toInt();
        readString = "";
        readServos = false;
        servoX->moveTo(readX*-1);
        servoY->moveTo(readY*-1);
        isControlled = true;
        break;
      default:
        readString += c;
      }
    }
  }

  //Button is being held
  if (!isControlled)
  {
    PIDStabilize();
    //need a function that handles our balance detection while moving in a direction, so we're not looking to stay perfectly balanced
  }
}
void PIDStabilize(){
  balanceController->readAccelerometerValues();
  PID->stabilize();
  Serial.print("Output X: "); Serial.println(outputX);
  Serial.print("Output Y: "); Serial.println(outputY);
  //Truncate values to our servo range (12 for now)
  float filteredX = xFilter.filter((outputX/PID->outputLimits * 12));
  float filteredY = yFilter.filter((outputY/PID->outputLimits * 12));
  servoX->moveTo((int)filteredX);
  servoY->moveTo((int)filteredY);
  // servoX->moveTo((int)(outputX/PID->outputLimits * 12));
  // servoY->moveTo((int)(outputY/PID->outputLimits * 12));
  // Serial.print("Filtered X: "); Serial.println(filteredX);
  // Serial.print(filteredX);
  // Serial.print("Filtered Y: "); Serial.println(filteredY);
  // char buff[16];

  // Apparently you don't need this for writing to the serial, print works just fine.
  // String outStream = "X: " + String(x, 2) + "\nY: " + String(y, 2);
  // outStream.toCharArray(buff, sizeof(buff));
  // Serial.write(buff);
  delay(10);
}



void takeOff()
{
  motors->kill();
  
}
