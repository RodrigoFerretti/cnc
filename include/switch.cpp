#include "Arduino.h"

#define DEBOUNCE_TIME 100

class Switch
{
public:
    Switch()
    {
    }

    Switch(int pin)
    {
        this->pin = pin;
        pinMode(this->pin, INPUT_PULLDOWN);
    }

    bool read()
    {
        bool currentRead = digitalRead(this->pin);
        this->changeTime = currentRead == this->previousRead ? this->changeTime : millis();
        this->previousRead = currentRead;
        bool read = this->changeTime - millis() > DEBOUNCE_TIME ? currentRead : 1;

        return read;
    }

private:
    int pin;
    int changeTime;

    bool previousRead;
};
