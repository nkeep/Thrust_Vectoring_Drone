#include <PID_v1.h>

class TVCPID{
    public:
        double setPointX, setPointY, *outputX, *outputY, *x, *y;
        double kp=4,ki=1,kd=1;
        PID *xPID;
        double prevXVal = 0, prevYVal = 0;
        PID *yPID;

        TVCPID(double setPointX, double setPointY, double *outputX, double *outputY, double *x, double *y);
        void begin();
        void stabilize();
};