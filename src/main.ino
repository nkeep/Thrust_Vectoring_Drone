#include <Arduino.h>
#include <TVCServo.h>
#include <TVCMotor.h>
#include <BalanceController.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

bool allowControls;
bool isControlled;
bool hasTakenOff = false;
float x;
float y;
float height;

void setup()
{
  Serial.begin(9600);
  TVCServo servoX;
  TVCServo servoY;
  BalanceController balanceController;
  TVCMotor motors;
  motors = new TVCMotor(8, 9);

  servoX.self.attach(2);
  servoY.self.attach(3);

  servoX.moveTo(90);
  servoY.moveTo(90);
}

void loop()
{
  delay(30);
  sensors_event_t event;
  accel.getEvent(&event);
  String t;

  if (allowControls)
  {
    t = "-1";
    while (Serial.available())
    {
      t += (char)Serial.read();
      Serial.print(t);
    }
    if (t == "0")
    {
      isControlled = false;
      allowControls = false; //We'll likely need to stabilize after moving
      servoX.slowMoveToHover();
      servoY.slowMoveToHover();
    }
    else if (t == "1")
    {
      isControlled = true;
      servoY.moveTo(80);
      // Will need to speed up the rotor to not gradually fall
    }
    else if (t == "4")
    {
      isControlled = true;
      servoY.moveTo(0);
    }
    else if (t == "2")
    {
      isControlled = true;
      servoX.moveTo(80);
    }
    else if (t == "3")
    {
      isControlled = true;
      servoX.moveTo(0);
    }
    else if (t == "5")
    {
      if (!hasTakenOff)
      {
        hasTakenOff = true;
        takeOff();
      }
    }
    //Button is being held
    if (isControlled)
    {
      //need a function that handles our balance detection while moving in a direction, so we're not looking to stay perfectly balanced
    }
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
        rotors.increaseSpeed();
        break;
      case 2: //Lower speed
        rotors.lowerSpeed();
        break;
      case 1: //Above max altitude
        //Probably write some loop here in the balance controller?
        break;
      }
    }
  }
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
}

void takeOff()
{
  float currentHeight = balanceController.readHeight();
  while (balanceController.readHeight() - currentHeight < 5) //Slowly increase speed until we have lift
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
}

void makeMicroAdjustments()
{
  balanceController.readAccelerometerValues(x, y);
  if (x > .3)
  {
    servoX.adjust(x);
  }
  if (y > .2)
  {
    servoY.adjust(y);
  }
}
