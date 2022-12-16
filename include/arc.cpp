#include <math.h>
#include <vector>

#define PI_ 3.14159265358979323846

using namespace std;

class Arc
{

public:
    struct Point
    {
        vector<double> position;
        vector<double> velocity;
    };

    Arc()
    {
    }

    Arc(vector<double> initialPosition, vector<double> centerPosition, vector<double> finalPosition, double velocity, bool isClockWise, double arcTolerance)
        : initialPosition{initialPosition}, centerPosition{centerPosition}, finalPosition{finalPosition}, velocity{velocity}
    {
        radius = sqrt(pow(initialPosition[0] - centerPosition[0], 2) + pow(initialPosition[1] - centerPosition[1], 2));

        angle = atan2(
            (initialPosition[0] - centerPosition[0]) * (finalPosition[1] - centerPosition[1]) - (initialPosition[1] - centerPosition[1]) * (finalPosition[0] - centerPosition[0]),
            (initialPosition[0] - centerPosition[0]) * (finalPosition[0] - centerPosition[0]) + (initialPosition[1] - centerPosition[1]) * (finalPosition[1] - centerPosition[1]));

        angle = isClockWise && angle >= 0 ? angle - 2 * PI_ : angle;
        angle = !isClockWise && angle <= 0 ? angle + 2 * PI_ : angle;

        pointsLenght = floor(fabs(0.5 * angle * radius) / sqrt(arcTolerance * (2 * radius - arcTolerance)));

        segmentAngle = angle / pointsLenght;
    }

    uint64_t getPointsLenght()
    {
        return pointsLenght;
    };

    Arc::Point getPoint(uint64_t index)
    {
        double cosSegment = cos(index * segmentAngle);
        double sinSegment = sin(index * segmentAngle);

        vector<double> pointPosition = {
            centerPosition[0] + ((initialPosition[0] - centerPosition[0]) * cosSegment - (initialPosition[1] - centerPosition[1]) * sinSegment),
            centerPosition[1] + ((initialPosition[0] - centerPosition[0]) * sinSegment + (initialPosition[1] - centerPosition[1]) * cosSegment),
        };

        vector<double> pointVelocity = {
            fabs(sinSegment) * velocity,
            fabs(cosSegment) * velocity,
        };

        return {
            .position = pointPosition,
            .velocity = pointVelocity,
        };
    }

private:
    uint64_t pointsLenght;

    vector<double> initialPosition;
    vector<double> centerPosition;
    vector<double> finalPosition;

    double angle;
    double radius;
    double velocity;
    double segmentAngle;
};
