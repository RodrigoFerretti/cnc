#include "Arduino.h"

#define X0_BACK_SWITCH_PIN 34
#define X0_FRONT_SWITCH_PIN 35

#define X1_BACK_SWITCH_PIN 36
#define X1_FRONT_SWITCH_PIN 39

#define DEBOUNCE_TIME 100

class Switch
{
public:
    Switch() {}

    Switch(int pin)
    {
        this->pin = pin;
        pinMode(this->pin, INPUT_PULLDOWN);
    }

    void setRead()
    {
        bool currentRead = digitalRead(this->pin);
        bool isNotBouncing = millis() - this->previousReadTime > DEBOUNCE_TIME;

        this->read = isNotBouncing ? currentRead : this->previousRead;
        this->previousRead = isNotBouncing ? currentRead : this->previousRead;
        this->previousReadTime = isNotBouncing ? millis() : this->previousReadTime;
    }

    bool getRead()
    {
        return this->read;
    }

private:
    int pin;
    int previousReadTime;

    bool read;
    bool previousRead;
};

Switch x0BackSwitch(X0_BACK_SWITCH_PIN);
Switch x0FrontSwitch(X0_FRONT_SWITCH_PIN);
Switch x1BackSwitch(X1_BACK_SWITCH_PIN);
Switch x1FrontSwitch(X1_FRONT_SWITCH_PIN);