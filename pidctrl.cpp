/*
 PID controller
 Copyright © 2015 by Stefan Lehmann

*/

#include "pidctrl.h"
#include <Arduino.h>
#include <limits.h>


long limit(long val, long min, long max)
{
    if (val > max)
        return max;
    if (val < min)
        return min;
    return val;
}


PIDCtrl::PIDCtrl() {
    max_delta = 0;
    last_cycle = 0;
    in = 0;
    sum = 0;
    last_in = 0;
    y = 0;
    kp = 0;
    ki = 0;
    kd = 0;
    min = -1000;
    max = 1000;
    antiwindup = false;
    reset = false;
}


long PIDCtrl::process() {
    cycletime = millis() - last_cycle;

    //Set limits to avoid overflow
    max_delta = kp ? LONG_MAX / kp : LONG_MAX;

    //proportional part
    //-----------------
    if (in > max_delta) yp = max;
    else if (in < -max_delta) yp = min;
    else yp = in * kp;

    //integral part
    //-------------
    if ((!antiwindup) || (in > 0 && y < max) || (in < 0 && y > min)) {
        long l = ki * in * cycletime / 1E3L;
        if (abs(sum) < LONG_MAX - abs(l)) {
            sum += l;
        }
    }

    if (reset) {
        sum = 0;
    }

    yi = sum;

    //differential part
    //-----------------
    yd = (in - last_in) * kd / cycletime;
    last_in = in;
    y = limit(yp + yi + yd, min, max);

    last_cycle = millis();

    return y;
}


long PIDCtrl::process(long in) {
    this->in = in;
    return this->process();
}