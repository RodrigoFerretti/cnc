#include "stepper.cpp"

#define ARC_TOLERANCE 0.01

class MultiStepper
{
public:
    MultiStepper()
    {
    }

    MultiStepper(Stepper x0Stepper, Stepper x1Stepper /*, Stepper y0Stepper, Stepper z0Stepper */)
        : x0Stepper{x0Stepper}, x1Stepper{x1Stepper} /*, y0Stepper{y0Stepper}, z0Stepper{z0Stepper} */
    {
    }

    bool canMove()
    {
        return !(isMoving() || switchHasTriggered());
    }

    void linearMove(vector<double> position, double speed)
    {
        x0Stepper.linearMove(position[0], speed);
        x1Stepper.linearMove(position[0], speed);
        // y0Stepper.linearMove(position[1], speed);
        // z0Stepper.linearMove(position[2], speed);
    }

    void arcMove(vector<double> centerOffset, vector<double> finalPosition, double speed, bool isClockWise)
    {
        vector<double> initialPosition = {
            (double)x0Stepper.currentPosition(),
            // y0Stepper.currentPosition(),
            // z0Stepper.currentPosition(),
        };

        vector<double> centerPosition = {
            initialPosition[0] + centerOffset[0],
            // initialPosition[1] + centerOffset[1],
            // initialPosition[2] + centerOffset[2],
        };

        Arc arc = Arc(initialPosition, centerPosition, finalPosition, speed, isClockWise, ARC_TOLERANCE);

        x0Stepper.arcMove(arc, 0);
        x1Stepper.arcMove(arc, 0);
        // y0Stepper.arcMove(arc, 1);
        // z0Stepper.arcMove(arc, 2);
    }

    void pause()
    {
        x0Stepper.pause();
        x1Stepper.pause();
        // y0Stepper.pause();
        // z0Stepper.pause();
    }

    void resume()
    {
        x0Stepper.resume();
        x1Stepper.resume();
        // y0Stepper.resume();
        // z0Stepper.resume();
    }

    void step()
    {
        readSwitches();

        if (switchHasTriggered())
        {
            return hardStop();
        }

        x0Stepper.step();
        x1Stepper.step();
        // y0Stepper.step();
        // z0Stepper.step();
    }

private:
    Stepper x0Stepper;
    Stepper x1Stepper;
    // Stepper y0Stepper;
    // Stepper z0Stepper;

    bool isMoving()
    {
        return (
            x0Stepper.isMoving() ||
            x1Stepper.isMoving() /* ||
            y0Stepper.isMoving() ||
            z0Stepper.isMoving() */
        );
    }

    void hardStop()
    {
        x0Stepper.hardStop();
        x1Stepper.hardStop();
        // y0Stepper.hardStop();
        // z0Stepper.hardStop();
    }

    bool switchHasTriggered()
    {
        return (
            x0Stepper.switchHasTriggered() +
            x1Stepper.switchHasTriggered() /* +
            y0Stepper.switchHasTriggered() +
            z0Stepper.switchHasTriggered() */
        );
    }

    void readSwitches()
    {
        x0Stepper.readSwitches();
        x1Stepper.readSwitches();
        // y0Stepper.readSwitches();
        // z0Stepper.readSwitches();
    }
};

MultiStepper multiStepper(x0Stepper, x1Stepper);