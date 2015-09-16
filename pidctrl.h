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


#include <stdint.h>


class PIDCtrl {
    private:
        //! Maximum allowed error, avoid overflow
        int32_t max_delta;
        //! millis of last cycle
        int32_t last_cycle;

    public:
        //! proportional, integral and differential part
        int32_t yp, yi, yd;
        //! difference between setpoint and current value
        int32_t in;
        //! cummulated difference between setpoint and current value
        int32_t sum;
        //! difference between setpoint and current value of last cycle
        int32_t last_in;
        //! controller output
        int32_t y;
        //! proportional gain
        int32_t kp;
        //! integral gain
        int32_t ki;
        //! differential gain
        int32_t kd;
        //! minimum y value
        int32_t min;
        //! maximum y value
        int32_t max;
        //! cycletime of last cycle
        int32_t cycletime;

        //!antiwindup option.
        /*! i part will stop counting if ymax is reached
            default: true
         */
        bool antiwindup;

        //! Reset sum of integral part
        bool reset;

        PIDCtrl();

        //! Calculate controller output.
        /*!
         *  Call this function with a certain cycletime defined by the
         *  `CYCLETIME` constant.
         */
        int32_t process();
        int32_t process(int32_t in);
};

/** @}*/

#endif