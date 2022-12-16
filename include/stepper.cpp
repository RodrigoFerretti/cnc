#include <AccelStepper.h>
#include "digital-input.cpp"
#include "arc.cpp"

#define X0_PUL_PIN 26
#define X0_DIR_PIN 25
#define X0_ENA_PIN 00
#define X0_BSW_PIN 34
#define X0_FSW_PIN 35

#define X1_PUL_PIN 14
#define X1_DIR_PIN 27
#define X1_ENA_PIN 00
#define X1_BSW_PIN 36
#define X1_FSW_PIN 39

#define MAX_SPEED 15000
#define ACCELERATION 20000
#define MIN_VELOCITY_STEP 0.01

#define DEBOUNCE_TIME 100
#define SWITCH_SAFE_VALUE 1

class Stepper : public AccelStepper
{
public:
    Stepper()
    {
    }

    Stepper(uint8_t pulPin, uint8_t dirPin, uint8_t enaPin, uint8_t backSwitchPin, uint8_t frontSwitchPin)
        : AccelStepper(AccelStepper::FULL3WIRE, dirPin, pulPin, enaPin)
    {
        setMaxSpeed(MAX_SPEED);
        setAcceleration(ACCELERATION);

        backSwitch = DigitalInput(backSwitchPin, INPUT_PULLDOWN, SWITCH_SAFE_VALUE, DEBOUNCE_TIME);
        frontSwitch = DigitalInput(frontSwitchPin, INPUT_PULLDOWN, SWITCH_SAFE_VALUE, DEBOUNCE_TIME);
    }

    void moveToWithSpeed(long position, double speed)
    {
        if (speed < MIN_VELOCITY_STEP)
        {
            return;
        }

        long distance = position - currentPosition();

        if (distance == 0)
        {
            return;
        }

        bool canMove = distance > 0 ? canMoveBackwards : canMoveForwards;

        if (!canMove)
        {
            return;
        }

        moveTo(position);
        setSpeed(distance > 0 ? speed : -speed);
        state = distance > 0 ? MOVING_BACKWARDS : MOVING_FORWARDS;
    }

    void arcMove(Arc arc, uint8_t arcAxis)
    {
        arc = arc;
        arcAxis = arcAxis;
        arcPointIndex = 0;
    }

    void limitStop()
    {
        state = LIMITED;
        moveTo(currentPosition());
    }

    bool isMoving()
    {
        return state == MOVING_BACKWARDS || state == MOVING_FORWARDS;
    }

    bool canMove()
    {
        return canMoveBackwards && canMoveForwards;
    }

    void readSwitches()
    {
        canMoveBackwards = backSwitch.safeRead();
        canMoveForwards = frontSwitch.safeRead();
    }

    void pause()
    {
        if (state == LIMITED)
        {
            return;
        }

        state = PAUSED;
    }

    void resume()
    {
        if (state != PAUSED)
        {
            return;
        }

        if (distanceToGo() == 0)
        {
            state = IDLE;
            return;
        }

        state = distanceToGo() > 0 ? MOVING_BACKWARDS : MOVING_FORWARDS;
    }

    void step()
    {
        if (state == PAUSED)
        {
            return;
        }

        handleArcMove();

        runSpeedToPosition();

        if (state == IDLE)
        {
            return;
        }

        if (distanceToGo() == 0)
        {
            state = IDLE;
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

    bool canMoveForwards;
    bool canMoveBackwards;

    uint8_t arcAxis;
    uint8_t state = IDLE;
    uint64_t arcPointIndex;

    Arc arc;
    DigitalInput backSwitch;
    DigitalInput frontSwitch;

    void handleArcMove()
    {
        if (arc.getPointsLenght() == arcPointIndex)
        {
            return;
        }

        if (state != IDLE)
        {
            return;
        }

        Arc::Point arcPoint = arc.getPoint(arcPointIndex);
        moveToWithSpeed(arcPoint.position[arcAxis], arcPoint.velocity[arcAxis]);
        arcPointIndex += 1;
    }
};

Stepper x0Stepper(X0_PUL_PIN, X0_DIR_PIN, X0_ENA_PIN, X0_BSW_PIN, X0_FSW_PIN);
Stepper x1Stepper(X1_PUL_PIN, X1_DIR_PIN, X1_ENA_PIN, X1_BSW_PIN, X1_FSW_PIN);