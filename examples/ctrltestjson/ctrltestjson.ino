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
    if (root.containsKey("kp")) pid.kp = root["kp"];
    if (root.containsKey("ki")) pid.ki = root["ki"];
    if (root.containsKey("kd")) pid.kd = root["kd"];
    if (root.containsKey("in")) pid.in = root["in"];
    if (root.containsKey("reset")) pid.reset();
}


void encode_json() {
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();

    JsonObject& pv = root.createNestedObject("processvalues");
    pv["kp"] = pid.kp;
    pv["ki"] = pid.ki;
    pv["kd"] = pid.kd;
    pv["in"] = pid.in;
    pv["y"] = pid.y;

    root.printTo(Serial);
    Serial.print("\n");
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
    if (Serial.available()) {
        inbuf = Serial.readStringUntil('\n');
        char buf[200];
        inbuf.toCharArray(buf, sizeof(buf));
        decode_json(buf);
    }

    pid.process();
    encode_json();
    delay(1000);
}
