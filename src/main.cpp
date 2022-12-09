#include "Arduino.h"
#include "web-server.cpp"

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    stepper_x0.loop();
    stepper_x1.loop();

    webSocket.cleanupClients();
}
