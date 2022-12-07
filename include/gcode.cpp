#include <Arduino.h>
#include "arc.cpp"
#include "stepper.cpp"

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
        double I = this->letterValue("I").toFloat();
        double J = this->letterValue("J").toFloat();
        double F = this->letterValue("F").toFloat();

        if (M == "00")
        {
            stepper_x0.rapidStop();
            stepper_x1.rapidStop();

            return;
        }

        if (G == "01")
        {
            stepper_x0.moveToWithSpeed(X, F);
            stepper_x1.moveToWithSpeed(X, F);

            return;
        }

        if (G == "02" || G == "03")
        {
            double xCurrentPosition = (double)stepper_x0.currentPosition();
            double yCurrentPosition = (double)stepper_x1.currentPosition();

            double initialPosition[2] = {xCurrentPosition, yCurrentPosition};
            double centerToInitialPosition[2] = {-I, -J};
            double finalPosition[2] = {X, Y};
            double speedMagnitude = F;

            Arc::Rotation rotation = G == "02" ? Arc::Rotation::COUNTER_CLOCKWISE : Arc::Rotation::COUNTER_CLOCKWISE;
            Arc arc = Arc(initialPosition, centerToInitialPosition, finalPosition, speedMagnitude, rotation);

            int xArcAxis = 0;
            int yArcAxis = 1;

            stepper_x0.setupArcMove(arc, xArcAxis);
            stepper_x1.setupArcMove(arc, yArcAxis);

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