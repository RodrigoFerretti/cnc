#include "Arduino.h"
#include "wifi.cpp"
#include "web-server.cpp"

void setup()
{
    Serial.begin(9600);

    wifi.setup();

    web_socket.setup();
    web_server.setup();
}

void loop()
{
    stepper_x0.readSwitches();
    stepper_x1.readSwitches();

    if (!stepper_x0.distanceToGo() && circular_motion.can_calculate_next_move(0))
    {
        move_t move = circular_motion.calculate_next_move(0);
        stepper_x0.moveWithSpeed(move.distance, move.velocity);
    }

    if (!stepper_x1.distanceToGo() && circular_motion.can_calculate_next_move(1))
    {
        move_t move = circular_motion.calculate_next_move(1);
        stepper_x1.moveWithSpeed(move.distance, move.velocity);
    }

    stepper_x0.limitStop();
    stepper_x1.limitStop();

    stepper_x0.runSpeedToPosition();
    stepper_x1.runSpeedToPosition();

    wss.cleanupClients();
}
