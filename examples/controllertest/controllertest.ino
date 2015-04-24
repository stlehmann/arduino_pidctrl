#include <pidctrl.h>

PIDCtrl pid;
String inbuf;


void help() {
    Serial.println("PID Controller Text Command list:");
    Serial.println("------------------------------------");
    Serial.println("x10 - change input to 10");
    Serial.println("p100 - change p part to 100");
    Serial.println("i101 - change i part to 101");
    Serial.println("d102 - change d part to 102");
    Serial.println("w1 - switch antiwindup on");
    Serial.println("max1000 - set ymax to 1000");
    Serial.println("min-1000 - set ymin to -1000");
    Serial.println("r - reset I part");
    Serial.println("h - show this help");
    Serial.println("s - show current controller state");
    Serial.println("------------------------------------");
}

void state() {
    Serial.println("Controller configuration:");
    Serial.println("-------------------------");
    Serial.println("P: " + String(pid.kp));
    Serial.println("I: " + String(pid.ki));
    Serial.println("D: " + String(pid.kd));
    Serial.println("ymax: " + String(pid.max));
    Serial.println("ymin: " + String(pid.min));
    Serial.println("antiwindup: " + String(pid.antiwindup));
    Serial.println("-------------------------");
}


void setup() {
    Serial.begin(9600);
    pid.kp = 0L;
    pid.ki = 0L;
    pid.kd = 0L;
    pid.in = 0;
    help();
}


void loop() {
    // Wait for Serial data and read buffer
    char inbyte;
    while (Serial.available()) {
        inbyte = Serial.read();
        inbuf.concat(inbyte);
    }
    if (inbyte == '\n') {
        if (inbuf == "h\n") {
            help();
        }
        else if (inbuf == "s\n") {
            state();
        }
        else if (inbuf[0]=='x') {
            String s = inbuf.substring(1);
            pid.in = s.toInt();
            Serial.println("in = " + String(pid.in));
        }
        else if (inbuf[0] == 'p') {
            String s = inbuf.substring(1);
            pid.kp = s.toInt();
            Serial.println("P = " + String(pid.kp));
        }
        else if (inbuf[0] == 'i') {
            String s = inbuf.substring(1);
            pid.ki = s.toInt();
            Serial.println("I = " + String(pid.ki));
        }
        else if (inbuf[0] == 'd') {
            String s = inbuf.substring(1);
            pid.kd = s.toInt();
            Serial.println("D = " + String(pid.kd));
        }
        else if (inbuf[0] == 'w') {
            pid.antiwindup = inbuf[1] != '0';
            Serial.println("antiwindup = " + String(pid.antiwindup));
        }
        else if (inbuf[0] == 'r') {
            pid.reset();
            Serial.println("reset");
        }
        else if (inbuf.substring(0, 3) == "max") {
            String s = inbuf.substring(3);
            pid.max = s.toInt();
            Serial.println("ymax = " + String(pid.max));
        }
        else if (inbuf.substring(0, 3) == "min") {
            String s = inbuf.substring(3);
            pid.min = s.toInt();
            Serial.println("ymin = " + String(pid.min));
        }
        inbuf = "";
    }

    long t0 = micros();
    pid.process();
    long dt = micros() - t0;

    Serial.print("in:");
    Serial.print(pid.in);
    Serial.print(", y:");
    Serial.print(pid.y);
    Serial.print(", T:" + String(pid.cycletime) + "ms");
    Serial.print(" (yp:");
    Serial.print(pid.yp);
    Serial.print(", yi:");
    Serial.print(pid.yi);
    Serial.print(", yd:");
    Serial.print(pid.yd);
    Serial.print(")");
    Serial.println(" " + String(dt) + "us");


    delay(250);
}
