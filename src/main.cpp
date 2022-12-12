#include "Arduino.h"
#include "web-server.cpp"

void setup()
{
    Serial.begin(115200);

    wifi.setup();
    webSocket.setup();
    webServer.setup();
}

void loop()
{
    x0BackSwitch.setRead();
    x0FrontSwitch.setRead();
    x1BackSwitch.setRead();
    x1FrontSwitch.setRead();

    x0Stepper.step();
    x1Stepper.step();

    webSocket.cleanupClients();
}
