#include "Arduino.h"
#include "wifi.cpp"
#include "web-server.cpp"

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    stepper_x0.readSwitches();
    stepper_x1.readSwitches();

    stepper_x0.generateArcMove();
    stepper_x1.generateArcMove();

    stepper_x0.limitStop();
    stepper_x1.limitStop();

    stepper_x0.runSpeedToPosition();
    stepper_x1.runSpeedToPosition();

    webSocket.cleanupClients();
}
