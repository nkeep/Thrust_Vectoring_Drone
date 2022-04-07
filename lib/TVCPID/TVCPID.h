#include <PID_v1.h>

class TVCPID{
    public:
        double setPointX, setPointY, *outputX, *outputY, *x, *y;
        double kp=80,ki=8,kd=0; //Starting Values
        double zeroZone = .4; //Dead Zone
        int outputLimits = 1000; //Range of values
        PID *xPID;
        double prevXVal = 0, prevYVal = 0;
        PID *yPID;

        TVCPID(double setPointX, double setPointY, double *outputX, double *outputY, double *x, double *y);
        void begin();
        void stabilize();
};