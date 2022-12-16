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

    void linearMove(vector<double> position, double feedRate)
    {
        if (isMoving())
        {
            return;
        }

        if (!canMove())
        {
            return;
        }

        x0Stepper.moveToWithSpeed(position[0], feedRate);
        x1Stepper.moveToWithSpeed(position[0], feedRate);
        // y0Stepper.moveToWithSpeed(position[1], feedRate);
        // z0Stepper.moveToWithSpeed(position[2], feedRate);
    }

    void arcMove(vector<double> centerOffset, vector<double> finalPosition, double feedRate, bool isClockWise)
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

        Arc arc = Arc(initialPosition, centerPosition, finalPosition, feedRate, isClockWise, ARC_TOLERANCE);

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

        if (!canMove())
        {
            return limitStop();
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
        bool x0IsMoving = x0Stepper.isMoving();
        bool x1IsMoving = x1Stepper.isMoving();
        // bool y0IsMoving = y0Stepper.isMoving();
        // bool z0IsMoving = z0Stepper.isMoving();

        bool isMoving = x0IsMoving || x1IsMoving /* || y0IsMoving || z0IsMoving */;

        return isMoving;
    }

    void limitStop()
    {
        x0Stepper.limitStop();
        x1Stepper.limitStop();
        // y0Stepper.limitStop();
        // z0Stepper.limitStop();
    }

    bool canMove()
    {
        bool x0CanMove = x0Stepper.canMove();
        bool x1CanMove = x1Stepper.canMove();
        // bool y0CanMove = y0Stepper.canMove();
        // bool z0CanMove = z0Stepper.canMove();

        bool canMove = x0CanMove && x1CanMove /* && y0CanMove && z0CanMove */;

        return canMove;
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