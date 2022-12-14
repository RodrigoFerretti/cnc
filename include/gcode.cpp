#include <Arduino.h>
#include "multi-stepper.cpp"

class GCode
{
public:
    GCode(){};

    GCode(String command)
    {
        this->command = command;
    }

    void run()
    {
        String G = this->letterValue("G");
        String M = this->letterValue("M");

        double X = this->letterValue("X").toFloat();
        double Y = this->letterValue("Y").toFloat();
        double Z = this->letterValue("Z").toFloat();
        double I = this->letterValue("I").toFloat();
        double J = this->letterValue("J").toFloat();
        double K = this->letterValue("K").toFloat();
        double F = this->letterValue("F").toFloat();

        double finalPosition[3] = {X, Y, Z};
        double distanceToCenter[3] = {I, J, K};
        double feedRate = F;

        bool isClockWise = G == "02";

        if (M == "00")
        {
            multiStepper.pause();
            return;
        }

        if (M == "100")
        {
            multiStepper.resume();
            return;
        }

        if (G == "01")
        {
            multiStepper.linearMove(finalPosition, feedRate);
            return;
        }

        if (G == "02" || G == "03")
        {
            multiStepper.arcMove(finalPosition, distanceToCenter, feedRate, isClockWise);
            return;
        }
    };

private:
    String command;

    String letterValue(String letter)
    {
        this->command = this->command + " ";
        int letterIndex = this->command.indexOf(letter);
        return ((letterIndex != -1) ? this->command.substring(letterIndex + 1, this->command.substring(letterIndex, this->command.length()).indexOf(" ") + letterIndex) : "");
    }
};