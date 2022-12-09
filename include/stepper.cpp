#include "AccelStepper.h"
#include "switch.cpp"
#include "arc.cpp"

#define STEPPER_X0_PUL_PIN 26
#define STEPPER_X0_DIR_PIN 25
#define STEPPER_X0_BACK_SWITCH_PIN 34
#define STEPPER_X0_FRONT_SWITCH_PIN 35

#define STEPPER_X1_PUL_PIN 14
#define STEPPER_X1_DIR_PIN 27
#define STEPPER_X1_BACK_SWITCH_PIN 36
#define STEPPER_X1_FRONT_SWITCH_PIN 39

#define MAX_SPEED 15000
#define ACCELERATION 20000
#define MIN_VELOCITY_STEP 0.01

#define DEBOUNCE_TIME 100

class Stepper : public AccelStepper
{
public:
    Stepper(int pulPin, int dirPin, int backSwitchPin, int frontSwitchPin)
        : AccelStepper(AccelStepper::FULL2WIRE, dirPin, pulPin)
    {
        this->setMaxSpeed(MAX_SPEED);
        this->setAcceleration(ACCELERATION);

        this->backSwitch = Switch(backSwitchPin, DEBOUNCE_TIME);
        this->frontSwitch = Switch(frontSwitchPin, DEBOUNCE_TIME);
    }

    void moveToWithSpeed(long position, double speed)
    {
        if (speed < MIN_VELOCITY_STEP)
        {
            return;
        }

        long distance = position - this->currentPosition();
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

    void pause()
    {
        if (!this->isMoving())
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

        this->state = this->distanceToGo() > 0 ? MOVING_BACKWARDS : MOVING_FORWARDS;
    }

    void loop()
    {
        this->checkSwitches();
        this->handleArcMove();

        if (!this->isMoving())
        {
            return;
        }

        bool stepped = this->runSpeedToPosition();

        if (!stepped)
        {
            this->state = IDLE;
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

    bool isMoving()
    {
        return this->state == MOVING_BACKWARDS || this->state == MOVING_FORWARDS;
    }

    void checkSwitches()
    {
        this->canMoveForwards = frontSwitch.read();
        this->canMoveBackwards = backSwitch.read();

        if (this->state == IDLE || this->state == PAUSED || this->state == LIMITED)
        {
            return;
        }

        if ((this->state == MOVING_FORWARDS) & this->canMoveForwards)
        {
            return;
        }

        if ((this->state == MOVING_BACKWARDS) & this->canMoveBackwards)
        {
            return;
        }

        this->state = LIMITED;
        this->moveTo(this->currentPosition());
    }

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

Stepper stepper_x0(STEPPER_X0_PUL_PIN, STEPPER_X0_DIR_PIN, STEPPER_X0_BACK_SWITCH_PIN,
                   STEPPER_X0_FRONT_SWITCH_PIN);

Stepper stepper_x1(STEPPER_X1_PUL_PIN, STEPPER_X1_DIR_PIN, STEPPER_X1_BACK_SWITCH_PIN,
                   STEPPER_X1_FRONT_SWITCH_PIN);