#include "AccelStepper.h"
#include "switch.cpp"

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
    enum State
    {
        stopped = 0,
        moovingForwards = 1,
        moovingBackwards = 2,
    };

public:
    Stepper(int pulPin, int dirPin, int backSwitchPin, int frontSwitchPin, int maxSpeed, int acceleration)
        : AccelStepper(AccelStepper::FULL2WIRE, dirPin, pulPin)
    {
        this->setMaxSpeed(maxSpeed);
        this->setAcceleration(acceleration);

        this->backSwitch = Switch(backSwitchPin, DEBOUNCE_TIME);
        this->frontSwitch = Switch(frontSwitchPin, DEBOUNCE_TIME);
    }

    void moveWithSpeed(long distance, float speed)
    {
        speed = fabs(speed);

        if (speed < MIN_VELOCITY_STEP)
        {
            return;
        }

        bool can_move = distance > 0 ? this->canMoveBackwards : this->canMoveForwards;

        if (!can_move)
        {
            return;
        }

        this->state = distance > 0 ? State::moovingBackwards : State::moovingForwards;
        this->move(distance);
        this->setSpeed(distance > 0 ? speed : -speed);
    }

    void readSwitches()
    {
        this->canMoveForwards = frontSwitch.read();
        this->canMoveBackwards = backSwitch.read();
    }

    void rapidStop()
    {
        this->state = State::stopped;
        this->moveTo(this->currentPosition());
    }

    void limitStop()
    {

        if ((this->state == State::moovingForwards) & this->canMoveForwards)
        {
            return;
        }

        if ((this->state == State::moovingBackwards) & this->canMoveBackwards)
        {
            return;
        }

        if (this->state == State::stopped)
        {
            return;
        }

        this->rapidStop();
    }

private:
    State state = State::stopped;

    Switch backSwitch;
    Switch frontSwitch;

    bool canMoveForwards;
    bool canMoveBackwards;
};

Stepper stepper_x0(STEPPER_X0_PUL_PIN, STEPPER_X0_DIR_PIN, STEPPER_X0_BACK_SWITCH_PIN,
                   STEPPER_X0_FRONT_SWITCH_PIN, MAX_SPEED, ACCELERATION);

Stepper stepper_x1(STEPPER_X1_PUL_PIN, STEPPER_X1_DIR_PIN, STEPPER_X1_BACK_SWITCH_PIN,
                   STEPPER_X1_FRONT_SWITCH_PIN, MAX_SPEED, ACCELERATION);