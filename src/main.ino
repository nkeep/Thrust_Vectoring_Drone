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

TVCServo *servoX;
TVCServo *servoY;
BalanceController *balanceController;
TVCMotor *motors;
TVCLED *LED;
TVCPID *PID;
TVCBuzzer *buzzer;
double outputX, outputY;
Ewma xFilter(0.3);
Ewma yFilter(0.3);

void setup()
{
  Serial.begin(9600);

  balanceController = new BalanceController(x,y,z);
  balanceController->begin();

  Serial.println("1");
  balanceController->readAccelerometerValues();
  Serial.println("2");

  digitalWrite(esc1_m, LOW);
  digitalWrite(esc2_m, LOW);
  delay(500);
  

  PID = new TVCPID(x, y, &outputX, &outputY, &x, &y);
  PID->begin();

  LED = new TVCLED(rLED, gLED, bLED);
  LED->begin();

  motors = new TVCMotor(esc1_m, esc2_m);
  motors->begin();

  servoX = new TVCServo(servoXPin, 100);
  servoX->begin();

  servoY = new TVCServo(servoYPin, 67);
  servoY->begin();

  buzzer = new TVCBuzzer(buzzerPin);


  int batteryValue = analogRead(batteryPin);
  Serial.print("battery value: ");
  Serial.print(batteryValue);
  // if(batteryValue < 400){
  //   while(true){
  //     Serial.println("Battery is dying.");
  //   }
  // }

}

void loop()
{
  //LED->changeColor(RED);
  //Serial.println("got to the loop");

  allowControls = true;
  delay(300);
  if (allowControls)
  {
    Serial.print(t);
    if(Serial.available())
    {
      t = (int)Serial.read() - 48;
      Serial.println(t);
    }
    switch(t){
      case -1:
        Serial.println("PID stabilizing");
        isControlled = false;
        //PIDStabilize();
        break;
      case 0:
        t = -1;
        break;
      case 1:
        Serial.print("1 is pressed");
        isControlled = true;
        LED->changeColor(BLUE);
        servoY->moveForward();
        // Will need to speed up the rotor to not gradually fall
        break;
      case 2:
        Serial.print("2 is pressed");
        isControlled = true;
        LED->changeColor(GREEN);
        servoX->moveBackward();
        break;
      case 3:
        Serial.print("3 is pressed");
        isControlled = true;
        LED->changeColor(RED);
        servoX->moveForward();
        break;
      case 4:
        Serial.print("4 is pressed");
        isControlled = true;
        LED->changeColor(CYAN);
        servoY->moveBackward();
        break;
      case 5:
        motors->kill();
        buzzer->playIndianaJones();
        break;
      case 6:
        motors->increaseSpeed();
        break;
      case 7:
        motors->decreaseSpeed();
        break;
      case 8:
        //buzzer->playIndianaJones();
        break;
      default:
        
        ;

    }
    //Button is being held
    if (isControlled)
    {
      //need a function that handles our balance detection while moving in a direction, so we're not looking to stay perfectly balanced
    }
  }
}
void PIDStabilize(){
  balanceController->readAccelerometerValues();
  PID->stabilize();
  float filteredX = xFilter.filter(servoX->hoverSetPoint - (outputX/2));
  float filteredY = yFilter.filter(servoY->hoverSetPoint - (outputY/2));
  servoX->moveTo((int)filteredX);
  servoY->moveTo((int)filteredY);
  Serial.print("x output:");
  Serial.print(outputX);
  Serial.println(x);
  delay(200);
}



void takeOff()
{
  motors->kill();
  
}
