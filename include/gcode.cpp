#include "multi-stepper.cpp"

class GCode
{
public:
    GCode()
    {
    }

    GCode(String command) : command{command}
    {
    }

    void run()
    {
        String G = letterValue("G");
        String M = letterValue("M");

        double X = letterValue("X").toFloat();
        double Y = letterValue("Y").toFloat();
        double Z = letterValue("Z").toFloat();
        double I = letterValue("I").toFloat();
        double J = letterValue("J").toFloat();
        double K = letterValue("K").toFloat();
        double F = letterValue("F").toFloat();

        vector<double> finalPosition = {
            X,
            Y,
            Z,
        };

        vector<double> centerOffset = {
            I,
            J,
            K,
        };

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
            multiStepper.arcMove(finalPosition, centerOffset, feedRate, isClockWise);
            return;
        }
    };

private:
    String command;

    String letterValue(String letter)
    {
        command = command + " ";
        int letterIndex = command.indexOf(letter);
        return ((letterIndex != -1) ? command.substring(letterIndex + 1, command.substring(letterIndex, command.length()).indexOf(" ") + letterIndex) : "");
    }
};