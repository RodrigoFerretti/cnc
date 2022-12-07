#include "Arduino.h"

class Switch
{
public:
    Switch() {}

    Switch(int pin, int debounceTime)
    {
        this->pin = pin;
        this->debounceTime = debounceTime;
        pinMode(this->pin, INPUT_PULLDOWN);
    }

    bool read()
    {
        bool currentRead = digitalRead(this->pin);
        bool isNotBouncing = millis() - this->previousReadTime > this->debounceTime;

        this->previousRead = isNotBouncing ? currentRead : this->previousRead;
        this->previousReadTime = isNotBouncing ? millis() : this->previousReadTime;

        return currentRead;
    }

private:
    int pin;
    int debounceTime;
    int previousReadTime;

    bool previousRead;
};
