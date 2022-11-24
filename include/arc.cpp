#include "math.h"

#define PI 3.14159265358979323846
#define PRECISION 0.1

class Arc
{
    enum Rotation
    {
        CLOCKWISE = 0,
        COUNTER_CLOCKWISE = 1,
    };

public:
    Arc() {}

    Arc(float initialPosition[2], float finalPosition[2], float centerToInitialPosition[2], Rotation rotation)
    {
        this->initialPosition[0] = initialPosition[0];
        this->initialPosition[1] = initialPosition[1];

        this->finalPosition[0] = finalPosition[0];
        this->finalPosition[1] = finalPosition[1];

        this->centerToInitialPosition[0] = centerToInitialPosition[0];
        this->centerToInitialPosition[1] = centerToInitialPosition[1];

        this->centerPosition[0] = this->initialPosition[0] - centerToInitialPosition[0];
        this->centerPosition[1] = this->initialPosition[1] - centerToInitialPosition[1];

        this->centerToFinalPosition[0] = this->finalPosition[0] - this->centerPosition[0];
        this->centerToFinalPosition[1] = this->finalPosition[1] - this->centerPosition[1];

        this->rotation = rotation;
        this->radius = sqrt(pow(centerToInitialPosition[0], 2) + pow(centerToInitialPosition[1], 2));

        this->lenght = this->calculateLenght();
    }

private:
    float radius;
    float lenght;

    Rotation rotation;

    float finalPosition[2];
    float centerPosition[2];
    float initialPosition[2];
    float currentPosition[2];

    float centerToFinalPosition[2];
    float centerToInitialPosition[2];

    float calculateLenght()
    {
        float lenght = atan2(
            this->centerToInitialPosition[0] * this->centerToFinalPosition[1] - this->centerToInitialPosition[1] * this->centerToFinalPosition[0],
            this->centerToInitialPosition[0] * this->centerToFinalPosition[0] + this->centerToInitialPosition[1] * this->centerToFinalPosition[1]);

        lenght = this->rotation == Rotation::CLOCKWISE && lenght >= 0 ? lenght - 2 * PI : lenght;
        lenght = this->rotation == Rotation::COUNTER_CLOCKWISE && lenght <= 0 ? lenght + 2 * PI : lenght;

        return lenght;
    }

    float calculateSegments()
    {
        return floor(fabs(0.5 * this->lenght * this->radius) / sqrt(PRECISION * (2 * this->radius - PRECISION)));
    }
};