#include <PID_v1.h>
#include <TVCPID.h>
#include <Arduino.h>

TVCPID::TVCPID(double setPointX, double setPointY, double *outputX, double *outputY, double *x, double *y){
    this->setPointX = setPointX;
    this->setPointY = setPointY;
    this->outputX = outputX;
    this->outputY = outputY;
    this->x = x;
    this->y = y;
}

void TVCPID::begin(){
    this->xPID = new PID(this->x, this->outputX, &this->setPointX, this->kp,this->ki,this->kd, DIRECT);
    this->yPID = new PID(this->y, this->outputY, &this->setPointY, this->kp,this->ki,this->kd, DIRECT);
    this->xPID->SetMode(AUTOMATIC);
    this->yPID->SetMode(AUTOMATIC);
    this->xPID->SetOutputLimits(0, this->outputLimits);
    this->yPID->SetOutputLimits(0, this->outputLimits);
    this->xPID->SetControllerDirection(REVERSE);
}

void TVCPID::stabilize(){
    if(fabs(*this->x) > 4.0){ //More aggressive tuning when angle is large
        this->xPID->SetTunings(10,0,4);
    }
    else{
        this->xPID->SetTunings(6,0,2);
    }
    // if(this->prevXVal < this->setPointX && *this->x > this->setPointX){
    if(*this->x > this->setPointX){
        this->xPID->SetOutputLimits(0,this->outputLimits);
        if(*this->x > this->setPointX - this->zeroZone){
            this->xPID->Compute();
        }
    // }else if(this->prevXVal > this->setPointX && *this->x < this->setPointX){
    } else if(*this->x < this->setPointX){
        this->xPID->SetOutputLimits(-this->outputLimits,0);
        if(*this->x < this->setPointX + this->zeroZone){
            this->xPID->Compute();
        }
    }else if(abs(*this->x) > this->setPointX + this->zeroZone){
        this->xPID->Compute();
        }
    else{
        *this->outputX = 0;
    }

    if(fabs(*this->y) > 4.0){
        this->yPID->SetTunings(10,0,4);
    }
    else{
        this->yPID->SetTunings(6,0,2);
    }
    // if(this->prevYVal < this->setPointY && *this->y > this->setPointY){
    if(*this->y > this->setPointY){
        this->yPID->SetOutputLimits(-this->outputLimits,0);
        if(*this->y > this->setPointY + this->zeroZone){
            this->yPID->Compute();
        }
    // }else if(this->prevYVal > this->setPointY && *this->y < this->setPointY){
    }else if(*this->y < this->setPointY){
        this->yPID->SetOutputLimits(0,this->outputLimits);
        if(*this->y < this->setPointY - this->zeroZone){
            this->yPID->Compute();
        }
    }else if(abs(*this->y) > this->setPointY + this->zeroZone){
        this->yPID->Compute();
        }
    else{
        *this->outputY = 0;
    }
    this->prevXVal = *this->x;
    this->prevYVal = *this->y;
}