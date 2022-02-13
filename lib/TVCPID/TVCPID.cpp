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
    this->xPID->SetOutputLimits(0, 30);
    this->yPID->SetOutputLimits(0, 30);
}

void TVCPID::stabilize(){
    if(this->prevXVal < this->setPointX && *this->x >= this->setPointX){
        this->xPID->SetOutputLimits(-30,0);
        if(*this->x > this->setPointX + .2){
            this->xPID->Compute();
        }
    }else if(this->prevXVal > this->setPointX && *this->x <= this->setPointX){
        this->xPID->SetOutputLimits(0,30);
        if(*this->x < this->setPointX - .2){
            this->xPID->Compute();
        }
    }else if(abs(*this->x) > this->setPointX + .2){
        this->xPID->Compute();
        }
    if(this->prevYVal < this->setPointY && *this->y >= this->setPointY){
        this->yPID->SetOutputLimits(-30,0);
        if(*this->y > this->setPointY + .2){
            this->yPID->Compute();
        }
    }else if(this->prevYVal > this->setPointY && *this->y <= this->setPointY){
        this->yPID->SetOutputLimits(0,30);
        if(*this->y < this->setPointY - .2){
            this->yPID->Compute();
        }
    }else if(abs(*this->y) > this->setPointY + .2){
        this->yPID->Compute();
        }
    this->prevXVal = *this->x;
    this->prevYVal = *this->y;
}