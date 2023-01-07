#include <math.h>
#include <vector>

#define _PI 3.14159265358979323846

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

        angle = isClockWise && angle >= 0 ? angle - 2 * _PI : angle;
        angle = !isClockWise && angle <= 0 ? angle + 2 * _PI : angle;

        _pointsLenght = floor(fabs(0.5 * angle * radius) / sqrt(arcTolerance * (2 * radius - arcTolerance)));

        segmentAngle = angle / _pointsLenght;
    }

    uint64_t pointsLenght()
    {
        return _pointsLenght;
    };

    Arc::Point point(uint64_t index)
    {
        double segmentCos = cos(index * segmentAngle);
        double segmentSin = sin(index * segmentAngle);

        vector<double> pointPosition = {
            centerPosition[0] + ((initialPosition[0] - centerPosition[0]) * segmentCos - (initialPosition[1] - centerPosition[1]) * segmentSin),
            centerPosition[1] + ((initialPosition[0] - centerPosition[0]) * segmentSin + (initialPosition[1] - centerPosition[1]) * segmentCos),
        };

        vector<double> pointVelocity = {
            fabs(segmentSin) * velocity,
            fabs(segmentCos) * velocity,
        };

        return {
            .position = pointPosition,
            .velocity = pointVelocity,
        };
    }

private:
    uint64_t _pointsLenght;

    vector<double> initialPosition;
    vector<double> centerPosition;
    vector<double> finalPosition;

    double angle;
    double radius;
    double velocity;
    double segmentAngle;
};
