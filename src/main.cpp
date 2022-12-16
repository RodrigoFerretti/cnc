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
    multiStepper.step();
    webSocket.cleanupClients();
}
