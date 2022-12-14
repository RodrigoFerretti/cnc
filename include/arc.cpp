#include <math.h>

#define _PI 3.14159265358979323846
#define ARC_PRECISION 0.01

class Arc
{
public:
    struct Segment
    {
        double position[2];
        double speed[2];
    };

    Arc()
    {
    }

    Arc(double initialPosition[2], double initialDistanceToCenter[2], double finalPosition[2], double speedMagnitude, bool isClockWise, double arcPrecision = ARC_PRECISION)
    {
        this->initialPosition[0] = initialPosition[0];
        this->initialPosition[1] = initialPosition[1];

        this->centerToInitialPosition[0] = -initialDistanceToCenter[0];
        this->centerToInitialPosition[1] = -initialDistanceToCenter[1];

        this->centerPosition[0] = this->initialPosition[0] + initialDistanceToCenter[0];
        this->centerPosition[1] = this->initialPosition[1] + initialDistanceToCenter[1];

        this->centerToFinalPosition[0] = finalPosition[0] - this->centerPosition[0];
        this->centerToFinalPosition[1] = finalPosition[1] - this->centerPosition[1];

        this->finalPosition[0] = finalPosition[0];
        this->finalPosition[1] = finalPosition[1];

        this->rotation = isClockWise ? CLOCKWISE : COUNTER_CLOCKWISE;
        this->speedMagnitude = speedMagnitude;
        this->radius = sqrt(pow(this->centerToInitialPosition[0], 2) + pow(this->centerToInitialPosition[1], 2));

        this->lenght = atan2(
            this->centerToInitialPosition[0] * this->centerToFinalPosition[1] - this->centerToInitialPosition[1] * this->centerToFinalPosition[0],
            this->centerToInitialPosition[0] * this->centerToFinalPosition[0] + this->centerToInitialPosition[1] * this->centerToFinalPosition[1]);

        this->lenght = this->rotation == CLOCKWISE && lenght >= 0 ? lenght - 2 * _PI : lenght;
        this->lenght = this->rotation == COUNTER_CLOCKWISE && lenght <= 0 ? lenght + 2 * _PI : lenght;

        this->segmentCount = floor(fabs(0.5 * this->lenght * this->radius) / sqrt(arcPrecision * (2 * this->radius - arcPrecision)));

        this->segmentLenght = this->lenght / this->segmentCount;
    }

    int getSegmentCount()
    {
        return this->segmentCount;
    };

    Arc::Segment getSegment(int index)
    {
        double cosSegment = cos(index * this->segmentLenght);
        double sinSegment = sin(index * this->segmentLenght);

        double centerToSegmentPosition[2] = {
            this->centerToInitialPosition[0] * cosSegment - this->centerToInitialPosition[1] * sinSegment,
            this->centerToInitialPosition[0] * sinSegment + this->centerToInitialPosition[1] * cosSegment,
        };

        return {
            .position = {
                this->centerPosition[0] + centerToSegmentPosition[0],
                this->centerPosition[1] + centerToSegmentPosition[1],
            },
            .speed = {
                fabs(sinSegment) * this->speedMagnitude,
                fabs(cosSegment) * this->speedMagnitude,
            },
        };
    }

private:
    enum Rotation
    {
        CLOCKWISE,
        COUNTER_CLOCKWISE,
    };

    int segmentCount;

    double lenght;
    double radius;
    double segmentLenght;
    double speedMagnitude;
    double finalPosition[2];
    double centerPosition[2];
    double initialPosition[2];
    double currentPosition[2];
    double centerToFinalPosition[2];
    double centerToInitialPosition[2];

    Arc::Rotation rotation;
};