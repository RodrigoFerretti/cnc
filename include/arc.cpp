#include "math.h"

#define PI 3.14159265358979323846
#define ARC_PRECISION 0.1

class Arc
{
public:
    enum Rotation
    {
        CLOCKWISE = 0,
        CCLOCKWISE = 1,
    };

    Arc(float initialPosition[2], float finalPosition[2], float centerToInitialPosition[2], Rotation rotation)
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
        this->radius = sqrt(pow(centerToInitialPosition[0], 2) + pow(centerToInitialPosition[1], 2));

        this->lenght = atan2(
            this->centerToInitialPosition[0] * this->centerToFinalPosition[1] - this->centerToInitialPosition[1] * this->centerToFinalPosition[0],
            this->centerToInitialPosition[0] * this->centerToFinalPosition[0] + this->centerToInitialPosition[1] * this->centerToFinalPosition[1]);

        this->lenght = this->rotation == CLOCKWISE && lenght >= 0 ? lenght - 2 * PI : lenght;
        this->lenght = this->rotation == CCLOCKWISE && lenght <= 0 ? lenght + 2 * PI : lenght;

        this->segments = floor(fabs(0.5 * this->lenght * this->radius) / sqrt(ARC_PRECISION * (2 * this->radius - ARC_PRECISION)));

        this->segmentLenght = this->lenght / this->segments > 0 ? this->segments : 1;
    }

    float *getSegmentPosition(int segment)
    {
        float cosSegment = cos(segment * this->segmentLenght);
        float sinSegment = sin(segment * this->segmentLenght);

        float centerToSegmentPosition[2] = {
            this->centerToInitialPosition[0] * cosSegment - this->centerToInitialPosition[1] * sinSegment,
            this->centerToInitialPosition[0] * sinSegment + this->centerToInitialPosition[1] * cosSegment,
        };

        float *segmentPosition = new float[2];

        segmentPosition[0] = this->centerPosition[0] + centerToSegmentPosition[0];
        segmentPosition[1] = this->centerPosition[1] + centerToSegmentPosition[1];

        return segmentPosition;
    }

private:
    int segments;
    float radius;
    float lenght;
    Rotation rotation;
    float segmentLenght;

    float finalPosition[2];
    float centerPosition[2];
    float initialPosition[2];
    float currentPosition[2];

    float centerToFinalPosition[2];
    float centerToInitialPosition[2];
};
