#include "unity.h"
#include "arc.cpp"

using namespace std;

void clockWiseCircle()
{
    vector<double> initialPosition = {0, 0};
    vector<double> centerPosition = {50000, 0};
    vector<double> finalPosition = {0, 0};

    double speedMagnitude = 15000;
    double arcPrecision = 0.02;
    bool isClockWise = true;

    Arc arc = Arc(initialPosition, centerPosition, finalPosition, speedMagnitude, isClockWise, arcPrecision);

    int segmentCount = arc.getPointCount();

    Arc::Point segment = arc.getPoint(segmentCount / 2);

    TEST_ASSERT_EQUAL(segmentCount, 3512);
    TEST_ASSERT_EQUAL(segment.position[0], 100000);
    TEST_ASSERT_EQUAL(segment.position[1], 0);
}

void counterClockWiseHalfCircle()
{
    vector<double> initialPosition = {20000, 20000};
    vector<double> centerPosition = {70000, 20000};
    vector<double> finalPosition = {120000, 20000};

    double speedMagnitude = 15000;
    double arcPrecision = 0.02;
    bool isClockWise = false;

    Arc arc = Arc(initialPosition, centerPosition, finalPosition, speedMagnitude, isClockWise, arcPrecision);

    int segmentCount = arc.getPointCount();

    Arc::Point segment = arc.getPoint(segmentCount / 2);

    TEST_ASSERT_EQUAL(segmentCount, 1756);
    TEST_ASSERT_EQUAL(segment.position[0], 70000);
    TEST_ASSERT_EQUAL(segment.position[1], -30000);
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(clockWiseCircle);
    RUN_TEST(counterClockWiseHalfCircle);
    UNITY_END();

    return 0;
}