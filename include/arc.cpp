#include <Arduino.h>

#define ARC_PRECISION 0.1

class Arc
{
public:
    enum Rotation
    {
        CLOCKWISE,
        COUNTER_CLOCKWISE,
    };

    struct Segment
    {
        double position[2];
        double speed[2];
    };

    Arc(){};

    Arc(double initialPosition[2], double centerToInitialPosition[2], double finalPosition[2], double speedMagnitude, Arc::Rotation rotation)
    {
        this->initialPosition[0] = initialPosition[0];
        this->initialPosition[1] = initialPosition[1];

        this->centerToInitialPosition[0] = centerToInitialPosition[0];
        this->centerToInitialPosition[1] = centerToInitialPosition[1];

        this->centerPosition[0] = this->initialPosition[0] - centerToInitialPosition[0];
        this->centerPosition[1] = this->initialPosition[1] - centerToInitialPosition[1];

        this->centerToFinalPosition[0] = this->finalPosition[0] - this->centerPosition[0];
        this->centerToFinalPosition[1] = this->finalPosition[1] - this->centerPosition[1];

        this->finalPosition[0] = finalPosition[0];
        this->finalPosition[1] = finalPosition[1];

        this->rotation = rotation;
        this->speedMagnitude = speedMagnitude;
        this->radius = sqrt(pow(centerToInitialPosition[0], 2) + pow(centerToInitialPosition[1], 2));

        this->lenght = atan2(
            this->centerToInitialPosition[0] * this->centerToFinalPosition[1] - this->centerToInitialPosition[1] * this->centerToFinalPosition[0],
            this->centerToInitialPosition[0] * this->centerToFinalPosition[0] + this->centerToInitialPosition[1] * this->centerToFinalPosition[1]);

        this->lenght = this->rotation == CLOCKWISE && lenght >= 0 ? lenght - 2 * PI : lenght;
        this->lenght = this->rotation == COUNTER_CLOCKWISE && lenght <= 0 ? lenght + 2 * PI : lenght;

        this->segmentCount = floor(fabs(0.5 * this->lenght * this->radius) / sqrt(ARC_PRECISION * (2 * this->radius - ARC_PRECISION)));

        this->segmentLenght = this->lenght / this->segmentCount > 0 ? this->segmentCount : 1;
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
