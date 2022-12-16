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
        if (this->isMoving())
        {
            return;
        }

        if (!this->canMove())
        {
            return;
        }

        this->x0Stepper.moveToWithSpeed(position[0], feedRate);
        this->x1Stepper.moveToWithSpeed(position[0], feedRate);
        // this->y0Stepper.moveToWithSpeed(position[1], feedRate);
        // this->z0Stepper.moveToWithSpeed(position[2], feedRate);
    }

    void arcMove(vector<double> finalPosition, vector<double> centerOffset, double feedRate, bool isClockWise)
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

        this->x0Stepper.arcMove(arc, 0);
        this->x1Stepper.arcMove(arc, 0);
        // this->y0Stepper.arcMove(arc, 1);
        // this->z0Stepper.arcMove(arc, 2);
    }

    void pause()
    {
        this->x0Stepper.pause();
        this->x1Stepper.pause();
        // this->y0Stepper.pause();
        // this->z0Stepper.pause();
    }

    void resume()
    {
        this->x0Stepper.resume();
        this->x1Stepper.resume();
        // this->y0Stepper.resume();
        // this->z0Stepper.resume();
    }

    void step()
    {
        this->readSwitches();

        if (!this->canMove())
        {
            return this->limitStop();
        }

        this->x0Stepper.step();
        this->x1Stepper.step();
        // this->y0Stepper.step();
        // this->z0Stepper.step();
    }

private:
    Stepper x0Stepper;
    Stepper x1Stepper;
    // Stepper y0Stepper;
    // Stepper z0Stepper;

    bool isMoving()
    {
        bool x0IsMoving = this->x0Stepper.isMoving();
        bool x1IsMoving = this->x1Stepper.isMoving();
        // bool y0IsMoving = this->y0Stepper.isMoving();
        // bool z0IsMoving = this->z0Stepper.isMoving();

        bool isMoving = x0IsMoving || x1IsMoving /* || y0IsMoving || z0IsMoving */;

        return isMoving;
    }

    void limitStop()
    {
        this->x0Stepper.limitStop();
        this->x1Stepper.limitStop();
        // this->y0Stepper.limitStop();
        // this->z0Stepper.limitStop();
    }

    bool canMove()
    {
        bool x0CanMove = this->x0Stepper.canMove();
        bool x1CanMove = this->x1Stepper.canMove();
        // bool y0CanMove = this->y0Stepper.canMove();
        // bool z0CanMove = this->z0Stepper.canMove();

        bool canMove = x0CanMove && x1CanMove /* && y0CanMove && z0CanMove */;

        return canMove;
    }

    void readSwitches()
    {
        this->x0Stepper.readSwitches();
        this->x1Stepper.readSwitches();
        // this->y0Stepper.readSwitches();
        // this->z0Stepper.readSwitches();
    }
};

MultiStepper multiStepper(x0Stepper, x1Stepper);