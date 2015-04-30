#include <ArduinoJson.h>
#include <pidctrl.h>
#include <SoftwareSerial.h>

#define SERIAL_TIMEOUT 250

PIDCtrl pid;
String inbuf;
SoftwareSerial debug(10, 11);
int brackets = 0;

void decode_json(char *json) {
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(json);
    pid.kp = root["kp"];
    pid.ki = root["ki"];
    pid.kd = root["kd"];
    pid.in = root["in"];
}


void encode_json() {
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    root["kp"] = pid.kp;
    root["ki"] = pid.ki;
    root["kd"] = pid.kd;
    root["in"] = pid.in;
    root["y"] = pid.y;

    char buf[256];
    root.printTo(buf, sizeof(buf));
    Serial.print(buf);
}


void setup() {
    Serial.begin(9600);
    debug.begin(9600);
    pid.kp = 0L;
    pid.ki = 0L;
    pid.kd = 0L;
    pid.in = 0;
}


void loop() {
    while (Serial.available()) {
        char inchar = Serial.read();
        inbuf += inchar;

        if (inchar == '{') {
            brackets++;
        }
        else if (inchar == '}') {
            brackets--;
            if (brackets == 0) {
                char buf[200];
                inbuf.toCharArray(buf, sizeof(buf));
                decode_json(buf);
                inbuf = "";
            }
        }
    }

    pid.process();
    encode_json();
    delay(1000);
}
