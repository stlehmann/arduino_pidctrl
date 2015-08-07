/*
 PID controller
 Copyright © 2015 by Stefan Lehmann

 Features:
 ---------
    * antiwindup
    * overflow protection at runtime
    * use of 32 bit integers
    * cycletime measurement, so no need for constant cycletimes

*/

#ifndef PIDCTRL_H_
#define PIDCTRL_H_

class PIDCtrl {
    private:
        //! Maximum allowed error, avoid overflow
        long max_delta = 0;
        //! millis of last cycle
        long last_cycle = 0;

    public:
        //! proportional, integral and differential part
        long yp, yi, yd;
        //! difference between setpoint and current value
        long in = 0;
        //! cummulated difference between setpoint and current value
        long sum = 0;
        //! difference between setpoint and current value of last cycle
        long last_in = 0;
        //! controller output
        long y = 0;
        //! proportional gain
        long kp = 0;
        //! integral gain
        long ki = 0;
        //! differential gain
        long kd = 0;
        //! minimum y value
        long min = -1000;
        //! maximum y value
        long max = 1000;
        //! cycletime of last cycle
        long cycletime;

        //!antiwindup option.
        /*! i part will stop counting if ymax is reached
            default: true
         */
        bool antiwindup = false;

        //! Reset sum of integral part
        bool reset = false;

        PIDCtrl();
        PIDCtrl(long kp, long ki, long kd);

        //! Calculate controller output.
        /*!
         *  Call this function with a certain cycletime defined by the
         *  `CYCLETIME` constant.
         */
        long process();
        long process(long in);
};

/** @}*/

#endif