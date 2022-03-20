#include <PID_v1.h>

class TVCPID{
    public:
        double setPointX, setPointY, *outputX, *outputY, *x, *y;
        double kp=6,ki=0,kd=2;
        PID *xPID;
        double prevXVal = 0, prevYVal = 0;
        double zeroZone = .4;
        int outputLimits = 84;
        PID *yPID;

        TVCPID(double setPointX, double setPointY, double *outputX, double *outputY, double *x, double *y);
        void begin();
        void stabilize();
};