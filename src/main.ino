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
int esc1_m = 3;
int esc2_m = 4;
int rLED = 5;
int gLED = 6;
int bLED = 7;
int buzzerPin = 8;
int servoXPin = 11;
int servoYPin = 12;
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

  balanceController->readAccelerometerValues();

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
  // if(batteryValue < 400){
  //   while(true){
  //     Serial.println("Battery is dying.");
  //   }
  // }

}

void loop()
{
  LED->changeColor(RED);

  allowControls = true;
  delay(30);
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
        PIDStabilize();
        break;
      case 0:
        t = -1;
        break;
      case 1:
        Serial.print("1 is pressed");
        isControlled = true;
        servoY->moveForward();
        // Will need to speed up the rotor to not gradually fall
        break;
      case 2:
        Serial.print("2 is pressed");
        isControlled = true;
        servoX->moveBackward();
        break;
      case 3:
        Serial.print("3 is pressed");
        isControlled = true;
        servoX->moveForward();
        break;
      case 4:
        Serial.print("4 is pressed");
        isControlled = true;
        servoY->moveBackward();
        break;
      case 5:
        buzzer->playIndianaJones();
        //motors->kill();
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
    /*
    else //Just hovering
    {
      //We want to continue to stabilize if nothing is happening
      int balanceState = balanceController.detectImbalance(x, y);
      int heightState = balanceController.detectHeightImbalance();
      if (balanceState == 2 || heightState == 4)
      {
        //Critical state, have to attempt to stabilize
        servoX.moveToHover();
        servoY.moveToHover();
        allowControls = false;
      }
      else if (balanceState == 1)
      {
        //Make a microadjustment
        servoX.adjust(x);
        servoY.adjust(y);
      }
      switch (heightState)
      {
      case 3: //Increase speed
        motors.increaseSpeed();
        break;
      case 2: //Lower speed
        motors.decreaseSpeed();
        break;
      case 1: //Above max altitude
        //Probably write some loop here in the balance controller?
        break;
      }
    }*/
  }
  /*
  else
  {
    //If we're not allowing controls, we stay here until the drone is stabilized
    while (!balanceController.balance(x, y))
    {
      servoX.moveTo(x);
      servoY.moveTo(y);
      //Will also need to adjust the rotor here so it doesn't fly like crazy
    }
    allowControls = true;
    //Probably need to reset all values in the classes
  }
  */
}

// void takeOff(){

// }

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
  delay(100);
}



void takeOff()
{
  motors->kill();
  /*
  float currentHeight = balanceController.readHeight();
  while ((balanceController.readHeight() - currentHeight) < 5) //Slowly increase speed until we have lift
  {
    motors.increaseSpeed();
    delay(50);
  }
  while (balanceController.detectHeightImbalance()) //Continue rising at this speed until we are within our hover range
  {
    makeMicroAdjustments();
    delay(30);
  }
  int balanceStatus = balanceController.balanceHeight();
  motors.decreaseSpeed();
  while (balanceStatus) //Loop until we can decide we are within the allowable range (10 loops?)
  {
    balanceStatus = balanceController.balanceHeight();
    //Make adjustments to speed if we are outside the ranges
    if (balanceStatus == 2)
    {
      motors.decreaseSpeed();
    }
    else if (balanceStatus == 1)
    {
      motors.increaseSpeed();
    }
    //Make microadjustments to the servos as we rise
    makeMicroAdjustments();

    delay(30);
  }
  */
}

void makeMicroAdjustments()
{
  balanceController->readAccelerometerValues();
  if (x > .3)
  {
    //servoX.adjust(x);
  }
  if (y > .2)
  {
    //servoY.adjust(y);
  }
}
