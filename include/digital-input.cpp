#include <Arduino.h>

class DigitalInput
{
public:
    DigitalInput()
    {
    }

    DigitalInput(uint8_t pin, uint8_t mode, uint8_t safeValue, uint8_t debounceTime)
        : pin{pin}, safeValue{safeValue}, debounceTime{debounceTime}
    {
        pinMode(pin, mode);
    }

    uint8_t safeRead()
    {
        bool currentRead = digitalRead(pin);

        changeTime = currentRead == previousRead ? changeTime : millis();
        previousRead = currentRead;

        return changeTime - millis() > debounceTime ? currentRead : safeValue;
    }

private:
    uint8_t pin;
    uint8_t safeValue;
    uint8_t debounceTime;
    uint8_t previousRead;

    uint64_t changeTime;
};
