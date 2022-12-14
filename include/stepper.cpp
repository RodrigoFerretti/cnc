#include "AccelStepper.h"
#include "switch.cpp"
#include "arc.cpp"

#define X0_PUL_PIN 26
#define X0_DIR_PIN 25
// #define X0_ENA_PIN 00
#define X0_BSW_PIN 34
#define X0_FSW_PIN 35

#define X1_PUL_PIN 14
#define X1_DIR_PIN 27
// #define X1_ENA_PIN 00
#define X1_BSW_PIN 36
#define X1_FSW_PIN 39

#define MAX_SPEED 15000
#define ACCELERATION 20000
#define MIN_VELOCITY_STEP 0.01

class Stepper : public AccelStepper
{
public:
    Stepper() {}

    Stepper(int pulsePin, int directionPin, int backSwitchPin, int frontSwitchPin)
        : AccelStepper(AccelStepper::FULL2WIRE, directionPin, pulsePin)
    {
        this->setMaxSpeed(MAX_SPEED);
        this->setAcceleration(ACCELERATION);

        this->backSwitch = Switch(backSwitchPin);
        this->frontSwitch = Switch(frontSwitchPin);
    }

    void moveToWithSpeed(long position, double speed)
    {
        if (speed < MIN_VELOCITY_STEP)
        {
            return;
        }

        long distance = position - this->currentPosition();

        if (distance == 0)
        {
            return;
        }

        bool canMove = distance > 0 ? this->canMoveBackwards : this->canMoveForwards;

        if (!canMove)
        {
            return;
        }

        this->moveTo(position);
        this->setSpeed(distance > 0 ? speed : -speed);
        this->state = distance > 0 ? MOVING_BACKWARDS : MOVING_FORWARDS;
    }

    void arcMove(Arc arc, bool axis)
    {
        this->arc = arc;
        this->arcAxis = axis;
        this->currentArcSegmentNumber = 0;
    }

    void limitStop()
    {
        this->state = LIMITED;
        this->moveTo(this->currentPosition());
    }

    bool isMoving()
    {
        return this->state == MOVING_BACKWARDS || this->state == MOVING_FORWARDS;
    }

    bool canMove()
    {
        return this->canMoveBackwards && this->canMoveForwards;
    }

    void readSwitches()
    {
        this->canMoveBackwards = this->backSwitch.read();
        this->canMoveForwards = this->frontSwitch.read();
    }

    void pause()
    {
        if (this->state == LIMITED)
        {
            return;
        }

        this->state = PAUSED;
    }

    void resume()
    {
        if (this->state != PAUSED)
        {
            return;
        }

        if (this->distanceToGo() == 0)
        {
            this->state = IDLE;
            return;
        }

        this->state = this->distanceToGo() > 0 ? MOVING_BACKWARDS : MOVING_FORWARDS;
    }

    void step()
    {
        if (this->state == PAUSED)
        {
            return;
        }

        this->handleArcMove();

        this->runSpeedToPosition();

        if (this->state == IDLE)
        {
            return;
        }

        if (this->distanceToGo() == 0)
        {
            this->state = IDLE;
            return;
        }
    }

private:
    enum State
    {
        IDLE,
        PAUSED,
        LIMITED,
        MOVING_FORWARDS,
        MOVING_BACKWARDS,
    };

    int state = IDLE;
    int currentArcSegmentNumber;

    bool arcAxis;
    bool canMoveForwards;
    bool canMoveBackwards;

    Arc arc;

    Switch backSwitch;
    Switch frontSwitch;

    void handleArcMove()
    {
        if (this->arc.getSegmentCount() == this->currentArcSegmentNumber)
        {
            return;
        }

        if (this->state != IDLE)
        {
            return;
        }

        Arc::Segment segment = this->arc.getSegment(this->currentArcSegmentNumber);
        this->moveToWithSpeed(segment.position[this->arcAxis], segment.speed[this->arcAxis]);
        this->currentArcSegmentNumber += 1;
    }
};

Stepper x0Stepper(X0_PUL_PIN, X0_DIR_PIN, X0_BSW_PIN, X0_FSW_PIN);
Stepper x1Stepper(X1_PUL_PIN, X1_DIR_PIN, X1_BSW_PIN, X1_FSW_PIN);